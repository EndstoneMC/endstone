import subprocess
import sys
from pathlib import Path

# Remove project directory from sys.path to ensure imports come from site-packages rather than the local endstone/ folder
_project_root = Path(__file__).resolve().parent.parent
sys.path = [p for p in sys.path if Path(p).resolve() != _project_root]


def main() -> None:
    try:
        from endstone_stubgen import load, render
    except ImportError:
        print("Error: endstone-stubgen not installed. Install with: pip install endstone-stubgen")
        sys.exit(1)

    module_name = "endstone._python"
    stubs_path = Path.cwd() / "stubs"
    module = load(module_name)
    package = module.package

    module.members = {"_T": package.get_member("_T"), **module.members}
    for member in list(module.members.keys()):
        if member.endswith("Registry"):
            module.members.pop(member)
            module.exports.remove(member)
    module.set_member("Registry", package.get_member("Registry"))
    module.exports.append("Registry")
    module.exports = sorted(module.exports)
    module["Server.get_registry"] = package["Server.get_registry"]

    module.set_member("__version__", package.get_member("__version__"))
    module.set_member("__minecraft_version__", package.get_member("__minecraft_version__"))
    module.get_member("__minecraft_version__").value = None
    module.imports.setdefault("__version__", package.imports.get("__version__"))
    module.exports = ["__version__", "__minecraft_version__"] + module.exports
    module.set_member("event.event_handler", package.get_member("event.event_handler"))
    module["event"].exports.append("event_handler")
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
        if relative_path == Path(".") / "__init__.pyi":
            text = text.replace("from endstone._version import __version__", "from ._version import __version__")

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
