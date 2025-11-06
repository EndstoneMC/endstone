import subprocess
from pathlib import Path

from stubgen import load, render


def main():
    # Load the module
    module_name = "endstone._python"
    stubs_path = Path(__file__).parent.parent / "stubs"
    module = load(module_name)

    # Inject extras
    package = module.package
    module.set_member("__version__", package.get_member("__version__"))
    module.set_member("__minecraft_version__", package.get_member("__minecraft_version__"))
    module.imports.setdefault("__version__", package.imports.get("__version__"))
    module.exports = ["__version__", "__minecraft_version__"] + module.exports
    module.set_member("event.event_handler", package.get_member("event.event_handler"))
    module["event"].exports.append("event_handler")
    module.set_member("plugin.Plugin", package.get_member("plugin.Plugin"))
    module["plugin.Plugin"].bases = None
    module["plugin.Plugin"].members.pop("_get_description")

    # Render the stubs
    render(module, stubs_path)

    output_path = Path(__file__).parent.parent / "endstone"
    output_path.mkdir(parents=True, exist_ok=True)

    source_path = stubs_path / "endstone" / "_python"
    for pyi in stubs_path.rglob("*.pyi"):
        text = pyi.read_text(encoding="utf-8")
        text = text.replace("endstone._python", "endstone")
        text = text.replace("from endstone._version import __version__", "from ._version import __version__")
        text = text.replace("collections.abc.Sequence", "list")
        text = text.replace("typing.SupportsFloat", "float")
        text = text.replace("typing.SupportsInt", "int")
        relative_path = pyi.relative_to(source_path)
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
