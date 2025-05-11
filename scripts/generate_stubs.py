import subprocess
from pathlib import Path


def main():
    output_path = Path(__file__).resolve().parent.parent
    command = [
        "pybind11-stubgen",
        "-o", str(output_path),
        "--enum-class-locations=.+:endstone._internal.endstone_python",
        "endstone._internal.endstone_python"
    ]
    subprocess.run(command, check=True)
    print(f"Stubs generated successfully to: {output_path}")


if __name__ == "__main__":
    main()
