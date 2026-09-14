import os
import subprocess
from pathlib import Path
from shutil import which


def stop_process(process: subprocess.Popen[str] | None) -> None:
    if process is None:
        return
    if os.name == "nt":
        # The Endstone launcher owns BDS as a child, and the player driver may
        # have its own RakNet workers. Terminating only the Python launcher
        # leaves those processes connected to the next test run.
        subprocess.run(
            ["taskkill", "/PID", str(process.pid), "/T", "/F"],
            check=False,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        )
    elif process.poll() is None:
        process.terminate()

    if process.poll() is None:
        try:
            process.wait(timeout=15)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait()


def ensure_player_driver(bot_dir: Path) -> str:
    node = ensure_node_runtime(bot_dir)
    dependency = bot_dir / "node_modules" / "bedrock-protocol"
    if not dependency.is_dir():
        raise RuntimeError("bedrock-protocol was not installed by npm")
    return node


def ensure_node_runtime(bot_dir: Path) -> str:
    node = which("node")
    npm = which("npm")
    if node is None or npm is None:
        raise RuntimeError("node and npm are required for the GameTest/Bedrock test harness")

    dependencies = (
        bot_dir / "node_modules" / "bedrock-protocol",
        bot_dir / "node_modules" / "prismarine-nbt",
    )
    if any(not dependency.is_dir() for dependency in dependencies):
        print("Installing the pinned PrismarineJS test dependencies...", flush=True)
        subprocess.run(
            [npm, "ci", "--ignore-scripts", "--no-audit", "--no-fund"],
            cwd=bot_dir,
            check=True,
        )
    return node
