# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "endstone-stubgen @ git+https://github.com/EndstoneMC/stubgen",
#     "ruff",
# ]
# ///
import re
import subprocess
import sys
from pathlib import Path

# Remove project directory from sys.path to ensure imports come from site-packages rather than the local endstone/ folder
_project_root = Path(__file__).resolve().parent.parent
sys.path = [p for p in sys.path if Path(p).resolve() != _project_root]


_CLASS_RE = re.compile(r"^(?P<indent> *)class (?P<name>\w+)(?:\([^)]*\))?\s*:")
# Matches either `NAME = Identifier(...)` (older repr) or the bare `NAME = ns:key`
# (current repr, which mirrors __str__). The bare form is recognised by the colon
# between two identifier-like halves.
_IDENTIFIER_CONST_RE = re.compile(
    r"^(?P<indent> +)(?P<name>\w+)\s*=\s*"
    r"(?:(?:endstone\.)?Identifier\((?P<wrapped>[^)]*)\)|(?P<bare>[\w.]+:[\w.]+))\s*$"
)


def _retype_identifier_constants(text: str) -> str:
    """Rewrite Identifier class constants to typed assignments
    `NAME: Identifier[<owner>] = "<key>"`. The value is the bare string literal
    for documentation; the runtime object is an `Identifier` constructed by the
    bindings, not what the stub literally shows. Class scope is tracked by
    indentation so nested classes pick up the innermost owner.
    """
    out: list[str] = []
    stack: list[tuple[int, str]] = []  # (indent_width, class_name)
    for line in text.splitlines(keepends=True):
        cls = _CLASS_RE.match(line)
        if cls:
            indent = len(cls.group("indent"))
            while stack and stack[-1][0] >= indent:
                stack.pop()
            stack.append((indent, cls.group("name")))
            out.append(line)
            continue
        const = _IDENTIFIER_CONST_RE.match(line)
        if const and stack:
            indent = len(const.group("indent"))
            owner = next((name for w, name in reversed(stack) if w < indent), None)
            if owner:
                key = (const.group("wrapped") or const.group("bare")).strip().strip("'\"")
                out.append(
                    f'{const.group("indent")}{const.group("name")}: '
                    f'Identifier[{owner}] = "{key}"\n'
                )
                continue
        out.append(line)
    return "".join(out)


def main() -> None:
    from endstone_stubgen import load, render

    module_name = "endstone._python"
    stubs_path = Path.cwd() / "stubs"
    module = load(module_name)
    package = module.package

    # endstone
    module.members = {"_T": package.get_member("_T"), **module.members}
    module.set_member("Registry", package.get_member("Registry"))
    module.exports.append("Registry")
    module.set_member("Identifier", package.get_member("Identifier"))
    module.exports.append("Identifier")
    module.exports = sorted(set(module.exports))
    module["Server.get_registry"] = package["Server.get_registry"]
    module.set_member("__version__", package.get_member("__version__"))
    module.imports.setdefault("__version__", package.imports.get("__version__"))
    module.exports = ["__version__"] + module.exports
    # endstone.event
    module["event"].members = {"_F": package.get_member("event._F"), **module["event"].members}
    module.set_member("event.event_handler", package.get_member("event.event_handler"))
    module["event"].exports.append("event_handler")
    # endstone.plugin
    module.set_member("plugin.Plugin", package.get_member("plugin.Plugin"))
    module["plugin.Plugin"].bases = None
    module["plugin.Plugin"].members.pop("_get_description")

    render(module, stubs_path)

    output_path = Path.cwd() / "endstone"
    output_path.mkdir(parents=True, exist_ok=True)

    source_path = stubs_path / "endstone" / "_python"
    for pyi in stubs_path.rglob("*.pyi"):
        if pyi == source_path.with_suffix(".pyi"):
            relative_path = Path(".") / "__init__.pyi"
        else:
            relative_path = pyi.relative_to(source_path).with_suffix("") / "__init__.pyi"

        text = pyi.read_text(encoding="utf-8")
        text = text.replace("endstone._python", "endstone")
        text = text.replace("collections.abc.Sequence", "list")
        text = text.replace("typing.SupportsFloat", "float")
        text = text.replace("typing.SupportsInt", "int")
        # Pybind11ImportFix injects a bogus `from . import endstone` in submodules
        # alongside the correct `import endstone`. Strip the relative-import form.
        text = text.replace("from . import endstone\n", "")
        if relative_path == Path(".") / "__init__.pyi":
            text = text.replace("from endstone._version import __version__", "from ._version import __version__")
        else:
            # Retype `NAME = Identifier(...)` class constants to `NAME: Identifier[<owner>]`
            # so the generic parameter survives. Tracks the enclosing class by indentation.
            text = _retype_identifier_constants(text)
            # Rewrite submodule refs to `endstone.Identifier` to the bare `Identifier`
            # name, importing it explicitly.
            if "endstone.Identifier" in text or re.search(r": Identifier\[", text):
                text = text.replace("endstone.Identifier", "Identifier")
                text = text.replace("import endstone\n", "from endstone import Identifier\n")
                if not re.search(r"^from endstone import Identifier$", text, flags=re.MULTILINE):
                    m = re.match(r'^"""[\s\S]*?"""\n', text)
                    insert_at = m.end() if m else 0
                    text = text[:insert_at] + "from endstone import Identifier\n" + text[insert_at:]

        if relative_path == Path("nbt") / "__init__.pyi":
            text = text.replace("endstone.nbt.", "")

        output_file = output_path / relative_path
        output_file.parent.mkdir(parents=True, exist_ok=True)
        output_file.write_text(text, encoding="utf-8")

    cmds = [
        ["ruff", "format", str(output_path)],
        ["ruff", "check", "--select", "I,E,F", "--ignore", "E501", "--fix", str(output_path)],
    ]
    for cmd in cmds:
        print(f"Running: {' '.join(cmd)}")
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode == 0:
            print(result.stdout)
        else:
            print(result.stderr)


if __name__ == "__main__":
    main()
