from __future__ import annotations

import json
import os

def write_report(runner) -> None:
    try:
        runner.result_path.parent.mkdir(parents=True, exist_ok=True)
        payload = {
            "schema_version": 2,
            "status": runner._status,
            "run_id": runner._run_id,
            "coverage": runner._coverage(final=runner.finished),
            "active_scenario": runner._active.name if runner._active else None,
            "active_driver_action": (
                runner._active.driver_action if runner._active else None
            ),
            "active_driver_data": (
                runner._active.driver_data if runner._active else None
            ),
            "active_driver_ready": (
                runner._driver_ready if runner._active else False
            ),
            "active_driver_ack": runner._driver_action_ack()[0],
            "run_mode": "complete",
            "selected_scenarios": [scenario.name for scenario in runner._scenarios],
            "player_driver_name": runner._bot_name,
            "player_online": runner.server.get_player(runner._bot_name) is not None,
            "world_name": os.getenv("ENDSTONE_TEST_WORLD_NAME"),
            "gametest": {
                "enabled": True,
                "active_case": runner._gametest_case,
                "state": runner._gametest_state,
                "error": runner._gametest_error,
            },
            "scenarios": [outcome.to_dict() for outcome in runner._outcomes],
        }
        runner.result_path.write_text(
            json.dumps(payload, ensure_ascii=False, indent=2) + "\n",
            encoding="utf-8",
        )
    except OSError as exc:
        runner.plugin.logger.error(
            f"Could not write automated event result to {runner.result_path}: {exc}"
        )
