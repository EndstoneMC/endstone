import subprocess
from pathlib import Path

from stubgen import load, render


def main():
    module_name = "endstone._python"
    stubs_path = Path(__file__).parent.parent / "stubs"
    module = load(module_name)
    render(module, stubs_path)

    output_path = Path(__file__).parent.parent / "endstone"
    output_path.mkdir(parents=True, exist_ok=True)

    source_path = stubs_path / "endstone" / "_python"
    for pyi in stubs_path.rglob("*.pyi"):
        text = pyi.read_text(encoding="utf-8")
        updated = text.replace("endstone._python", "endstone")
        relative_path = pyi.relative_to(source_path)
        output_file = output_path / relative_path
        output_file.write_text(updated, encoding="utf-8")

    cmds = [
        ["ruff", "check", "--fix", "--select", "I", str(output_path)],
        ["ruff", "format", str(output_path)],
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
