import json
import os
import subprocess
import sys
import time
import uuid
from pathlib import Path
from typing import Any

from .cli import parse_args
from .common import (
    BOT_DIR,
    DRIVER_ACK_SUFFIX,
    RUN_TIMEOUT_SECONDS,
    TERMINAL_STATUSES,
    env_enabled,
)
from .common import (
    repository_root as get_repository_root,
)
from .gametest import GameTestProvisioning, enable_gametest_experiment, provision_gametest_pack
from .plugin import PluginProvisioning, provision_lifecycle_fixture_plugin, resolve_runner_python
from .process import ensure_node_runtime, ensure_player_driver, stop_process
from .result import (
    cleanup_driver_ack_files,
    read_current_result,
    write_infrastructure_failure,
)
from .world import ServerFolderLock, TestWorldProvisioning, provision_test_world


def main() -> int:
    args = parse_args()
    selected_scenarios = tuple(
        dict.fromkeys(
            scenario.strip()
            for value in (args.scenarios or [])
            for scenario in value.split(",")
            if scenario.strip()
        )
    )
    server_folder = args.server_folder.resolve()
    result_file = (server_folder / "endstone-event-test-result.json").resolve()
    bot_dir = BOT_DIR
    root = get_repository_root()

    # The event suite owns its test assets and driver identity.  They are not
    # configurable test-selection switches: every run must execute all three
    # supported categories against the checked-in GameTest pack and runner
    # world template.
    gametest_scope = "system"
    gametest_pack = None
    test_world_template = None
    bot_host = "127.0.0.1"
    bot_port = 19132
    bot_name = "EndstoneEventBot"

    test_world_provisioning: TestWorldProvisioning | None = None
    gametest_provisioning: GameTestProvisioning | None = None
    plugin_provisioning: PluginProvisioning | None = None
    node: str | None = None
    runner_lock = ServerFolderLock(server_folder)
    runner_lock.acquire()
    run_id = uuid.uuid4().hex
    driver_ack_file = Path(f"{result_file}{DRIVER_ACK_SUFFIX}")
    process: subprocess.Popen[str] | None = None
    bot_process: subprocess.Popen[str] | None = None
    terminal_result: dict[str, Any] | None = None
    failure_phase: str | None = None
    failure_message: str | None = None
    failure_returncodes: dict[str, int | None] = {}
    failure_exit_code = 2
    cleanup_errors: list[str] = []

    def mark_failure(
        phase: str,
        message: str,
        *,
        returncodes: dict[str, int | None] | None = None,
        exit_code: int = 2,
    ) -> None:
        nonlocal failure_phase, failure_message, failure_exit_code
        if failure_phase is None:
            failure_phase = phase
            failure_message = message
        else:
            failure_message = f"{failure_message}; {phase}: {message}"
        if returncodes:
            failure_returncodes.update(returncodes)
        failure_exit_code = exit_code

    def active_world_name() -> str | None:
        return test_world_provisioning.run_name if test_world_provisioning is not None else None

    environment = os.environ.copy()
    python_executable: str | None = None
    command: list[str] | None = None
    deadline = 0.0
    last_console_scenario: str | None = None

    try:
        # Remove stale state before provisioning.  The report and ACK are both
        # bound to run_id below, but deleting them also keeps diagnostics clear.
        result_file.unlink(missing_ok=True)
        cleanup_driver_ack_files(driver_ack_file)
        python_executable = resolve_runner_python()
        plugin_provisioning = provision_lifecycle_fixture_plugin(python_executable)

        # Every complete run receives a fresh copy. The template itself is
        # never used as the active BDS world and is generated automatically
        # once when it is missing.
        test_world_provisioning = provision_test_world(
            server_folder,
            test_world_template,
            env_enabled("ENDSTONE_TEST_KEEP_WORLD"),
        )
        gametest_world = test_world_provisioning.run_name
        node = ensure_node_runtime(bot_dir)
        gametest_provisioning = provision_gametest_pack(
            server_folder,
            gametest_pack,
            gametest_scope,
            gametest_world,
            env_enabled("ENDSTONE_TEST_KEEP_GAMETEST"),
        )
        enable_gametest_experiment(node, server_folder, gametest_world)

        endstone_pythonpath = root / "build" / "package"
        if endstone_pythonpath.is_dir():
            existing_pythonpath = environment.get("PYTHONPATH")
            environment["PYTHONPATH"] = os.pathsep.join(
                value for value in (str(endstone_pythonpath), existing_pythonpath) if value
            )
        assert plugin_provisioning is not None
        existing_pythonpath = environment.get("PYTHONPATH")
        environment["PYTHONPATH"] = os.pathsep.join(
            value for value in (str(plugin_provisioning.python_directory), existing_pythonpath) if value
        )
        environment.update(
            {
                "ENDSTONE_TEST_AUTO_EVENTS": "1",
                "ENDSTONE_TEST_AUTO_EVENTS_STOP": "1",
                "ENDSTONE_TEST_RESULT_FILE": str(result_file),
                "ENDSTONE_TEST_RUN_ID": run_id,
                "ENDSTONE_TEST_BOT_NAME": bot_name,
                "ENDSTONE_TEST_GAMETEST_SCOPE": gametest_scope,
                "ENDSTONE_TEST_WORLD_NAME": test_world_provisioning.run_name,
                "ENDSTONE_TEST_DRIVER_ACK_FILE": str(driver_ack_file),
            }
        )
        if selected_scenarios:
            environment["ENDSTONE_TEST_AUTO_EVENT_SCENARIOS"] = ",".join(
                selected_scenarios
            )
            # Targeted event debugging must not spend time running the
            # unrelated embedded pytest suite before the selected scenarios.
            environment["ENDSTONE_TEST_SKIP_PYTEST"] = "1"
        else:
            environment.pop("ENDSTONE_TEST_AUTO_EVENT_SCENARIOS", None)
            environment.pop("ENDSTONE_TEST_SKIP_PYTEST", None)
        if gametest_provisioning is not None:
            environment["ENDSTONE_TEST_GAMETEST_PACK"] = str(gametest_provisioning.installed_pack)
        else:
            environment.pop("ENDSTONE_TEST_GAMETEST_PACK", None)
        assert python_executable is not None
        command = [
            python_executable,
            "-m",
            "endstone",
            "--server-folder",
            str(server_folder),
            "--yes",
            "--no-interactive",
        ]

        print("Starting Endstone event smoke run:", " ".join(command), flush=True)
        assert node is not None
        ensure_player_driver(bot_dir)
        process = subprocess.Popen(
            command,
            cwd=server_folder,
            env=environment,
            text=True,
            stdin=subprocess.PIPE,
        )
        deadline = time.monotonic() + RUN_TIMEOUT_SECONDS

        while time.monotonic() < deadline:
            report = read_current_result(
                result_file,
                run_id=run_id,
                world_name=test_world_provisioning.run_name,
            )
            if (
                node is not None
                and bot_process is None
                and report is not None
                and report.get("active_driver_action") == "connect"
            ):
                bot_command = [
                    node,
                    str(bot_dir / "driver.js"),
                    "--result-file",
                    str(result_file),
                    "--host",
                    bot_host,
                    "--port",
                    str(bot_port),
                    "--username",
                    bot_name,
                    "--timeout",
                    str(max(1.0, deadline - time.monotonic())),
                ]
                print(
                    "Starting exact-version Bedrock player driver:",
                    " ".join(bot_command),
                    flush=True,
                )
                bot_process = subprocess.Popen(bot_command, cwd=bot_dir, env=environment, text=True)

            if (
                process.stdin is not None
                and report is not None
                and report.get("active_driver_action") == "console-command"
                and report.get("active_driver_ready") is True
                and report.get("active_scenario") != last_console_scenario
            ):
                data = report.get("active_driver_data")
                command_line = data.get("command") if isinstance(data, dict) else None
                if not isinstance(command_line, str) or not command_line.strip():
                    raise RuntimeError("console-command scenario has no command")
                process.stdin.write(command_line.rstrip("\r\n") + "\n")
                process.stdin.flush()
                last_console_scenario = report.get("active_scenario")

            result = report if report is not None and report.get("status") in TERMINAL_STATUSES else None
            if result is not None:
                terminal_result = result
                if bot_process is not None:
                    stop_process(bot_process)
                if process.poll() is None:
                    try:
                        process.wait(timeout=30)
                    except subprocess.TimeoutExpired:
                        stop_process(process)
                if process.returncode not in {0, None}:
                    mark_failure(
                        "endstone-shutdown",
                        "Endstone exited with a non-zero code after publishing its result",
                        returncodes={"endstone": process.returncode},
                    )
                else:
                    failure_exit_code = (
                        0 if result["status"] == "passed" else 2 if result["status"] == "infrastructure_failed" else 1
                    )
                break

            if bot_process is not None and bot_process.poll() is not None:
                mark_failure(
                    "player-driver",
                    "player driver exited before the server produced a result",
                    returncodes={"player_driver": bot_process.returncode},
                )
                break

            if process.poll() is not None:
                mark_failure(
                    "endstone-startup",
                    "Endstone exited before producing a terminal result",
                    returncodes={"endstone": process.returncode},
                )
                break
            time.sleep(0.25)
        else:
            mark_failure("timeout", f"timed out waiting for {result_file}")
    except KeyboardInterrupt:
        mark_failure("keyboard-interrupt", "event-test run interrupted by the user", exit_code=130)
    except Exception as exc:  # noqa: BLE001 - publish runner failures as terminal reports
        mark_failure("runner", f"{type(exc).__name__}: {exc}")
    finally:
        # Stop children before touching server.properties or deleting the run
        # world.  A failed stop is itself an infrastructure failure and keeps
        # the transaction/world available for recovery.
        for name, child in (("player_driver", bot_process), ("endstone", process)):
            if child is None or child.poll() is not None:
                continue
            try:
                stop_process(child)
            except Exception as exc:  # noqa: BLE001
                cleanup_errors.append(f"{name} process: {exc}")

        if failure_phase is not None:
            failure_payload = write_infrastructure_failure(
                result_file,
                run_id=run_id,
                world_name=active_world_name(),
                phase=failure_phase,
                message=failure_message or "event-test runner failed",
                returncodes=failure_returncodes,
            )
        else:
            failure_payload = None

        if gametest_provisioning is not None:
            try:
                gametest_provisioning.cleanup()
            except Exception as exc:  # noqa: BLE001
                cleanup_errors.append(f"GameTest provisioning: {exc}")
        if plugin_provisioning is not None:
            try:
                plugin_provisioning.cleanup()
            except Exception as exc:  # noqa: BLE001
                cleanup_errors.append(f"plugin provisioning: {exc}")
        if test_world_provisioning is not None:
            try:
                test_world_provisioning.cleanup()
            except Exception as exc:  # noqa: BLE001
                cleanup_errors.append(f"test world: {exc}")
        try:
            cleanup_driver_ack_files(driver_ack_file)
        except Exception as exc:  # noqa: BLE001
            cleanup_errors.append(str(exc))

        if cleanup_errors:
            cleanup_message = "; ".join(cleanup_errors)
            mark_failure("cleanup", cleanup_message)
            failure_payload = write_infrastructure_failure(
                result_file,
                run_id=run_id,
                world_name=active_world_name(),
                phase=failure_phase or "cleanup",
                message=failure_message or cleanup_message,
                returncodes=failure_returncodes,
            )
            print("[runner] Cleanup incomplete: " + cleanup_message, file=sys.stderr, flush=True)
        runner_lock.release()

    if failure_phase is not None:
        print(json.dumps(failure_payload, ensure_ascii=False, indent=2))
        return failure_exit_code
    assert terminal_result is not None
    print(json.dumps(terminal_result, ensure_ascii=False, indent=2))
    return failure_exit_code
