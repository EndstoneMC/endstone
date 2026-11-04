import traceback

from endstone_test.harness.api_recorder import Recorder, api_members, recording_mode

NOT_AUTOMATABLE: dict[str, str] = {
    "endstone.OfflinePlayer.name#get": "no API returns an OfflinePlayer in 0.12 and the class has no constructor",
    "endstone.OfflinePlayer.unique_id#get": "no API returns an OfflinePlayer in 0.12 and the class has no constructor",
    "endstone.inventory.MolangIngredient.expression#get": "no 1.26.60 recipe has a Molang ingredient and plugins "
    "cannot create recipes",
}


def manifest(recorder: Recorder) -> dict:
    problems = list(recorder.problems)
    try:
        members = api_members(recorder.unwrap)
    except Exception:  # noqa: BLE001
        members = []
        problems.append(f"the API members could not be listed:\n{traceback.format_exc()}")
    required = {member.key: member.kind for member in members}
    instrumented = set(recorder.wrapped)
    instrumented.update(member.key for found in recorder.codes.values() for member in found)
    instrumented.update(member.key for member in recorder.calls.values())
    for key in required:
        if key not in instrumented:
            problems.append(f"{key} is an API member the recorder does not instrument")
    for key in NOT_AUTOMATABLE:
        if key not in required:
            problems.append(f"NOT_AUTOMATABLE names {key}, which is not an API member")
    return {
        "mode": recording_mode(),
        "members": required,
        "waived": {key: reason for key, reason in NOT_AUTOMATABLE.items() if key in required},
        "problems": problems,
        "tool": recorder.tool,
    }
