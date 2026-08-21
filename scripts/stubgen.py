#!/usr/bin/env python3
# /// script
# requires-python = ">=3.11"
# dependencies = [
#     "griffe>=2.1,<3",
#     "lazy-loader",
#     "ruff",
# ]
# ///

"""Regenerate endstone/**/__init__.pyi and the lazy.attach maps from a built _python extension.

Run automatically by the build; see src/endstone/python/CMakeLists.txt.
"""

import argparse
import ast
import importlib.util
import re
import shutil
import subprocess
import sys
import types
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent

# The extension renders as the public package; pybind11 knows it as endstone._python.
MODULE = "endstone._python"
PACKAGE = "endstone"

# Rewritten before imports are registered, so the stubs never import these.
ALIASES = {
    "collections.abc.Sequence": "list",
    # An opaque std::vector bound so Python can mutate the event's list in place; devs
    # should still see and use it as a plain list.
    "endstone.block.BlockList": "list[endstone.block.Block]",
    "typing.SupportsFloat": "float",
    "typing.SupportsInt": "int",
    # pybind11 puts py::array_t<T>'s dtype in ndarray's shape slot.
    "numpy.ndarray": "numpy.typing.NDArray",
}

# A namespaced value the pattern file did not claim, i.e. an Identifier constant
# on a class its owner list does not cover.
_UNTYPED_CONST_RE = re.compile(r"""^ +[A-Z][A-Z0-9_]*(: str)? = (['"])[\w.]+:[\w.]+\2$""", re.MULTILINE)

# The only source files griffe needs statically: the extension is inspected, and
# everything else that reaches the stubs is hand-written in these three.
STAGED_SOURCES = [
    Path("__init__.py"),
    Path("event/__init__.py"),
    Path("plugin/__init__.py"),
]


def load_engine(path: Path) -> types.ModuleType:
    spec = importlib.util.spec_from_file_location("_endstone_stubgen", path)
    if spec is None or spec.loader is None:
        raise SystemExit(f"cannot load the stubgen engine from {path}")
    engine = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(engine)
    return engine


def stage(source_dir: Path, extension: Path, work_dir: Path) -> Path:
    """Build a minimal import root holding the package sources and the fresh extension.

    Deliberately excludes the committed .pyi (they would otherwise be an input to
    their own generation) and libendstone_runtime.so (griffe dlopens every
    extension module it finds, and the runtime segfaults outside a BDS process).
    """
    root = work_dir / PACKAGE
    if work_dir.exists():
        shutil.rmtree(work_dir)
    for rel in STAGED_SOURCES:
        (root / rel).parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source_dir / PACKAGE / rel, root / rel)
    shutil.copyfile(extension, root / extension.name)
    return work_dir


def public_members(engine: types.ModuleType, mod) -> list:
    return [
        name
        for name, member in mod.members.items()
        if name not in engine.SKIP_LIST and not engine.is_private(name) and member.kind != engine.Kind.MODULE
    ]


def bound_names(tree: ast.Module) -> set:
    """Names the module body binds itself; these must stay out of the attach map."""
    names = set()
    for node in tree.body:
        if isinstance(node, (ast.Import, ast.ImportFrom)):
            names.update(alias.asname or alias.name.split(".")[0] for alias in node.names)
        elif isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef, ast.ClassDef)):
            names.add(node.name)
        elif isinstance(node, ast.Assign):
            names.update(t.id for t in node.targets if isinstance(t, ast.Name))
    return names


def write_if_changed(path: Path, data: bytes) -> bool:
    if path.exists() and path.read_bytes() == data:
        return False
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)
    return True


def rewrite_attach(path: Path, key: str, names: list) -> None:
    """Replace submod_attrs[key] in a lazy.attach call, leaving the rest of the file alone."""
    raw = path.read_bytes()
    tree = ast.parse(raw)
    node = None
    for call in ast.walk(tree):
        if not (isinstance(call, ast.Call) and getattr(call.func, "attr", None) == "attach"):
            continue
        for kw in call.keywords:
            if kw.arg != "submod_attrs" or not isinstance(kw.value, ast.Dict):
                continue
            for k, v in zip(kw.value.keys, kw.value.values):
                if isinstance(k, ast.Constant) and k.value == key and isinstance(v, ast.List):
                    node = v
    if node is None:
        raise SystemExit(f"{path}: no lazy.attach(submod_attrs={{{key!r}: [...]}}) to update")

    names = sorted(set(names) - bound_names(tree))
    # col_offset is a byte offset into the line.
    offsets = [0]
    for line in raw.splitlines(keepends=True):
        offsets.append(offsets[-1] + len(line))
    start = offsets[node.lineno - 1] + node.col_offset
    end = offsets[node.end_lineno - 1] + node.end_col_offset
    # No trailing comma, so ruff format keeps short lists on one line.
    body = ", ".join(f'"{name}"' for name in names)
    write_if_changed(path, raw[:start] + f"[{body}]".encode() + raw[end:])


def run_ruff(source_dir: Path, args: list) -> None:
    cmd = [sys.executable, "-m", "ruff", *args]
    result = subprocess.run(cmd, cwd=source_dir, capture_output=True, text=True)
    if result.returncode != 0:
        sys.stderr.write(result.stdout + result.stderr)
        raise SystemExit(f"{' '.join(cmd)} failed with exit code {result.returncode}")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--extension", type=Path, required=True, help="the built _python extension module")
    parser.add_argument("--engine", type=Path, default=REPO_ROOT / "src/endstone/python/stubgen.py")
    parser.add_argument("--pattern-file", type=Path, default=REPO_ROOT / "src/endstone/python/stubgen.pat")
    parser.add_argument("--source-dir", type=Path, default=REPO_ROOT)
    parser.add_argument("--work-dir", type=Path, default=REPO_ROOT / "build/stubgen")
    opt = parser.parse_args()

    engine = load_engine(opt.engine)
    patterns = engine.load_pattern_file(str(opt.pattern_file))

    import_root = stage(opt.source_dir, opt.extension, opt.work_dir)
    sys.path = [str(import_root)] + [p for p in sys.path if Path(p or ".").resolve() != opt.source_dir.resolve()]

    top = engine.load(MODULE)

    # Grafts that have a hand-written Python counterpart; must precede the detach.
    package = top.package
    top.set_member("event.event_handler", package.get_member("event.event_handler"))
    # Plugin is the hand-written subclass, presented flat: its own members first,
    # then everything it inherits from the binding it replaces.
    binding = top["plugin.Plugin"]
    plugin = package.get_member("plugin.Plugin")
    for name, member in binding.members.items():
        plugin.set_member(name, member)
    plugin.docstring = binding.docstring
    plugin.bases = []
    top.set_member("plugin.Plugin", plugin)

    # Present the extension as the package it is re-exported from; Object.path is
    # derived from the parent chain, so this renames the whole tree.
    top.parent = None
    top.name = PACKAGE

    attach = {}
    for mod in engine._iter_modules(top):
        real = public_members(engine, mod)
        sg = engine.StubGen(top, mod, patterns=patterns, rename=(MODULE, PACKAGE), aliases=ALIASES)
        sg.render()
        text = sg.get()
        if stray := [m.group(0).strip() for m in _UNTYPED_CONST_RE.finditer(text)]:
            raise SystemExit(
                f"{mod.path}: namespaced constant(s) not covered by {opt.pattern_file}, "
                "add the owning class to its list:\n " + "\n ".join(stray)
            )
        directory = opt.source_dir.joinpath(*mod.path.split("."))
        write_if_changed(directory / "__init__.pyi", text.encode())
        # The attach map re-exports what the stub exports, minus the facades the
        # pattern file introduces, which have no runtime counterpart.
        key = MODULE.rpartition(".")[2] if mod is top else mod.name
        attach[directory / "__init__.py"] = (key, sorted(set(mod.exports) & set(real) | set(mod.modules)))

    unmatched = [p.query.pattern for p in patterns if not p.matches]
    if unmatched:
        raise SystemExit(f"{opt.pattern_file}: pattern(s) never matched, binding renamed?\n  " + "\n  ".join(unmatched))

    for path, (key, names) in attach.items():
        rewrite_attach(path, key, names)

    stubs = sorted(str(p.relative_to(opt.source_dir)) for p in (opt.source_dir / PACKAGE).rglob("__init__.pyi"))
    run_ruff(opt.source_dir, ["format", *stubs, *sorted(str(p.relative_to(opt.source_dir)) for p in attach)])
    # PYI021 and PYI011 keep typeshed-style stubs bare; these ones document the API,
    # so the docstrings and the enum defaults stay.
    ruff_ignore = "E501,PYI011,PYI021"
    run_ruff(opt.source_dir, ["check", "--select", "I,E,F,UP,PYI", "--ignore", ruff_ignore, "--fix", *stubs])


if __name__ == "__main__":
    main()
