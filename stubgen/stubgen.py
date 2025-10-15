import argparse
from pathlib import Path

__all__ = ["main"]

import griffe
import jinja2

from extensions import (
    MemberOrderFix,
    Pybind11DocstringParser,
    Pybind11ExportFix,
    Pybind11ImportFix,
    Pybind11InternalsFilter,
    Pybind11NativeEnumSupport,
    Pybind11PropertySupport,
    Pybind11SubmoduleSupport,
)


def main():
    parser = argparse.ArgumentParser("pybind11-stubgen", description="Generates stubs for specified modules")
    parser.add_argument(
        "-o",
        "--output-dir",
        type=Path,
        help="The root directory for output stubs",
        default=Path("./stubs"),
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        dest="dry_run",
        help="Don't write stubs. Parses module and report errors",
    )
    parser.add_argument(
        "module_name",
        metavar="MODULE_NAME",
        type=str,
        help="module name",
    )
    args = parser.parse_args()
    run(args.module_name, args.output_dir, args.dry_run)


def render(mod: griffe.Module, output_dir: Path):
    def _do_render(env: jinja2.Environment, mod: griffe.Module, output_dir: Path):
        template = env.get_template("module.jinja")
        result = template.render(obj=mod)
        parts = mod.path.split(".")
        pkg_dir = output_dir / Path(*parts)
        pkg_dir.mkdir(parents=True, exist_ok=True)
        (pkg_dir / "__init__.pyi").write_text(result, encoding="utf-8")
        for child in mod.modules.values():
            _do_render(env, child, output_dir)

    templates_dir = Path(__file__).parent / "templates"
    env = jinja2.Environment(
        loader=jinja2.FileSystemLoader(templates_dir),
        keep_trailing_newline=True,
    )
    _do_render(env, mod, output_dir)


def load(module_name: str) -> griffe.Module:
    ext = griffe.load_extensions(
        Pybind11SubmoduleSupport,
        Pybind11InternalsFilter,
        Pybind11PropertySupport,
        Pybind11NativeEnumSupport,
        Pybind11DocstringParser,
        Pybind11ExportFix,
        Pybind11ImportFix,
        MemberOrderFix,
    )
    return griffe.load(module_name, extensions=ext)


def run(module_name: str, output_dir: Path, dry_run: bool = False):
    module = load(module_name)
    if dry_run:
        return

    render(module, output_dir)
