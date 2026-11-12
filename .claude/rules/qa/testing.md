---
paths:
  - "tests/**"
---

# Testing

- Put offline value types and pure logic in `tests/*.py` (pytest against the installed wheel) or `tests/*.cpp` (GTest `endstone_test` target); anything needing BDS goes in `tests/plugin`.
- Add each new GTest `.cpp` to the `add_executable(endstone_test ...)` list in `tests/CMakeLists.txt`.
- Name GTest suites `<Thing>Test.<BehaviourSentence>`.
- Keep `CppPluginLoaderTest.*` serialized through its `RESOURCE_LOCK`; every other GTest runs in parallel.
- Grep `ctest` output for the new test's name, or use `--gtest_filter`, before trusting "all passed"; a missing name means the link failed and left a stale binary.
- Do not link an interpreter into `endstone_test`; verify interpreter behaviour on a live server.
- Write pytest tests as plain functions typed `-> None`, with a one-line docstring starting "Verify ...", grouped under `# ====` section banners, and `pytest.mark.parametrize` for tables.
- Name a test helper `make_*`, never `test_*`.
- Add every new registry type to `test_registry` so its constants enumerate the registry.
- Fix a wrong test to assert what the API promises; do not delete it.
- Give every lifetime fix a regression that replays the exact crash sequence as an in-server `lifetime.*` scenario, plus a GTest where the code is core-only.
- Give each QA failure a Paper-parity test and land every fix's regression scenario in the same commit.
- Test PyGILState-sensitive callbacks by calling `PyGILState_GetThisThreadState` and a `threading.local` from the callback.

## Live-server harness

- Run one server at a time, with `--server-dir` set to the project's `bedrock_server` folder and `--workdir` in a scratch directory outside any repo; a Windows firewall prompt means a new server path was created, so remove it.
- Keep `--workdir` short on Windows; the BDS zip holds paths of about 150 characters.
- The runner uses ports 20232-20234 unless `--port` moves it; keep every local QA server inside the QA range, 20132-21131, and off 20132, which is the QA server behind `qa.endstone.dev`.
- Test a freshly built wheel, and pass the endstone-client project to `--extra`; a full run fails without it, and `uv cache clean endstone-client` gives a fresh client.
- A full run fails on a failed, errored, crashed or skipped scenario, a pytest skip, `Could not pass event`, `Could not execute task`, `Unhandled exception`, a traceback, a crash banner, or an event path with neither a scenario nor a manifest reason; read `results-<run id>.json` and `logs/<run id>.log` rather than the exit code, and a full run passes only with `0 missing`.
- Run player scenarios on Windows, Linux and `--transport nethernet`; run the signaling-ping scenario only over NetherNet.
- Pass `--timeout 7200` to a full NetherNet run; it takes over an hour, and the default 1800 seconds kills it.
- Write a scenario as `@scenario("area.name.path", covers={Event: "path"}, plot=True, budget=N)` taking `ctx: Context`; copy `scenarios/player_blocks.py`.
- Wait with `await ctx.ticks(n)` or `await ctx.until(pred, what=)`; never sleep or block the server thread.
- Declare `normal` for every event, `cancel` for a cancellable one and `mutate` per setter, unless `harness/api_manifest.py` lists the member in `NOT_AUTOMATABLE` with a reason.
- Give every new method a pytest in `tests/` or a scenario if it needs ticks.
- Update the harness in the commit of an API change it reads, such as an event becoming cancellable (`tests/test_event_classes.py`) or `dispatch_command` changing its return value (`harness/engine.py`).
- Put a pytest that needs a connected player or a resident chunk in `tests/plugin/src/endstone_test/tests/player/`; `setblock`, `spawn_actor` and `load_chunk` fail in unticked chunks.
- Assert on game state and on what the client received, never on the event alone: `bot.call("get_received_packets", "<Packet>", since=index)` and the client model (`get_block`, `local_player.get_inventory_json`).
- Prove "nothing happened" with a barrier packet the client must receive, as `barrier()` in `scenarios/player_actors.py` does.
- Observe events with `ctx.capture(EventClass, where, cancel=, mutate=)` and `ctx.release` it at the end; never register a listener in a test.
- Keep scenario state that must survive `/reload` in `ctx.stash`, and keep harness setup idempotent.
- Drive player-triggered behaviour with `ctx.bot(name)` and `bot.call("dotted.method")` under a unique bot name; add the missing bot action rather than waiving a hard path.
- Make bot changes mirror the real client's class and method names, handler chain and order, and prove a bot defect from the packet the bot sent against the real client; do not patch a symptom with a flag.
- Find the bot action table in `endstone-client`'s `docs/DESIGN.md`; a pickup scenario needs the bot to send `TakeItemActorPacket`.
- Add a `harness/fields.py` check when an event has a field rule.
- Do not kill a server you did not start; another BDS may hold 19132.
- Run no whole-drive `find` or other heavy scan during a run; it starves the `MC_SERVER` thread and the bots and causes timeouts.
- Send `ServerboundLoadingScreen` End from a headless client after spawn.
- Leave a scenario that fails on an Endstone bug failing until fixed; do not edit it, widen a timeout, add a waiver or rerun until green.
- Reproduce a failure alone with `--select`, and for a crash banner reproduce the scenario the runner charged it to; a failure only in a full run is an ordering or leak bug.
- Bisect a failure that passed on the previous release against the last tag, then fix it.
- Before a release run `--crash-test`, then a `--bench --bots 100` join storm and the same with `--vanilla`, and compare the `MC_SERVER` thread CPU on an idle machine.
- Check edited scenario files for stray NUL bytes when the plugin fails to import.
- Define injected fixtures (`server`, `plugin`, `plot`, `player`) on the test class.
- Use the `plot` fixture (`plot.pos`, `plot.at`, `plot.volume`) for anything needing a ticking spot, and clean up what you spawn.
- Unlink a venv file before overlaying a locally built binary, or use `--no-cache --link-mode=copy`.
- Drive a console repro from Bash, never PowerShell, by piping commands into `endstone -s bedrock_server -y --no-interactive`.
- Build a console-repro scene on the project's `bedrock_server` with a fresh `level-type=FLAT` world, `online-mode=false`, `allow-list=false` and unused ports, never a copied server folder; apply the reporter's `server.properties` changes before testing the symptom; use `tickingarea add`, `fill`, `setblock`, `summon` or a powered dispenser, and read results with `testforblock` or `testfor @e[...,x=,y=,z=,dx=,dy=,dz=]`.
- Pipe the same console script into the bare `bedrock_server` for a vanilla baseline.
- A dev venv loads every plugin installed in it as an entry point; for a clean run, and for Linux, `pip install endstone==<version>` in a `python` container pointed at the extracted BDS zip with a matching `version.txt`.
- Verify reconstructed BDS read paths with the in-server `/test` pytest suite.
- Keep human-eyes probes behind `/test <form|player|inv|spawn|map|block|sender>`; bare `/test` runs pytest.
