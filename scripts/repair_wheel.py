import argparse
import os
import platform
import subprocess
import tempfile
from pathlib import Path

from auditwheel.wheeltools import InWheelCtx


def run_command(cmd):
    """Run a shell command and stream its output to the console."""
    with subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True) as p:
        for line in p.stdout:
            print(line, end="")

        p.wait()
        if p.returncode != 0:
            print(f"Error: Command failed with return code {p.returncode}")
            print(f"Command: {' '.join(p.args)}")
            raise subprocess.CalledProcessError(p.returncode, p.args)


def upload_and_strip(ctx, org, project, auth_token, strip):
    if auth_token:
        print("Uploading debug files to sentry...")
        # Use sentry-cli to upload all debug files from the unpacked wheel
        cmd = [
            "sentry-cli",
            "debug-files",
            "upload",
            "-o",
            org,
            "-p",
            project,
            "--include-sources",
            "--auth-token",
            auth_token,
            str(ctx.path),
        ]

        run_command(cmd)
    else:
        print("Skipping uploading debug files as no auth token is specified...")

    if strip:
        print("Stripping debug files from the wheel file...")
        for root, _, filenames in os.walk(ctx.path):
            for file in filenames:
                file_path = Path(root) / file
                if file_path.suffix in (".pdb", ".dbg"):
                    print(f"Removing: {file_path}")
                    os.remove(file_path)


def process_wheel(wheel_file, org, project, auth_token, dest_dir, strip):
    system = platform.system().lower()

    if system == "windows":
        output_wheel = Path(dest_dir) / Path(wheel_file).name
        with InWheelCtx(wheel_file, str(output_wheel)) as ctx:
            upload_and_strip(ctx, org, project, auth_token, strip)
    elif system == "linux":
        # Handle Linux: Write stripped wheel to temp dir and repair it
        with tempfile.TemporaryDirectory() as temp_dir:
            temp_wheel = Path(temp_dir) / Path(wheel_file).name
            with InWheelCtx(wheel_file, str(temp_wheel)) as ctx:
                upload_and_strip(ctx, org, project, auth_token, strip)

            # Repair the stripped wheel
            cmd = ["auditwheel", "repair", "-w", dest_dir, str(temp_wheel)]
            run_command(cmd)
    else:
        raise NotImplementedError(f"Unsupported platform: {system}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Repair a Python wheel file by uploading symbols and optionally stripping debug files."
    )
    parser.add_argument("wheel_file", help="Path to the .whl file to process.")
    parser.add_argument("-w", "--dest-dir", required=True, help="Destination directory for the repaired wheel.")
    parser.add_argument("-o", "--org", help="The organization ID or slug. Defaults to SENTRY_ORG environment variable.")
    parser.add_argument(
        "-p", "--project", help="The project ID or slug. Defaults to SENTRY_PROJECT environment variable."
    )
    parser.add_argument(
        "--auth-token", help="Use the given Sentry auth token. Defaults to SENTRY_AUTH_TOKEN environment variable."
    )
    parser.add_argument("-s", "--strip", action="store_true", help="Strip the wheel by removing debug files.")

    args = parser.parse_args()
    wheel_file_path = Path(args.wheel_file).resolve()
    dest_dir_path = Path(args.dest_dir).resolve()

    org = args.org or os.getenv("SENTRY_ORG")
    project = args.project or os.getenv("SENTRY_PROJECT")
    auth_token = args.auth_token or os.getenv("SENTRY_AUTH_TOKEN")

    if not org:
        print("Error: Organization not set. Provide --org or set SENTRY_ORG environment variable.")
        exit(1)

    if not project:
        print("Error: Project not set. Provide --project or set SENTRY_PROJECT environment variable.")
        exit(1)

    if not wheel_file_path.exists():
        print(f"Error: File {wheel_file_path} does not exist.")
        exit(1)

    process_wheel(wheel_file_path, org, project, auth_token, dest_dir_path, args.strip)
    print(f"Successfully processed the wheel file: {wheel_file_path}")
