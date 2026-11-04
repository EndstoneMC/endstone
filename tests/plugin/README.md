# endstone-test

An Endstone plugin that tests the API against a live Bedrock Dedicated Server, plus the runner that drives it
unattended.

## Running the harness

The runner builds a scratch server from a wheel and a BDS zip, starts it, runs every scenario and exits non-zero on
any failure. Run it by its path; the example is relative to the repository root:

```shell
uv run --no-project --python 3.13 python tests/plugin/src/endstone_test/runner.py \
    --wheel wheelhouse/endstone-<version>-cp313-cp313-win_amd64.whl \
    --bds-zip ~/.bedrock_server/windows/bedrock-server-1.26.61.1.zip \
    --workdir C:/esh \
    --extra ../endstone-client
```

It only needs the standard library to start. It creates `<workdir>/venv` with the wheel, this plugin, the fixture
plugin in `fixtures/` and the extras, extracts the zip into `<workdir>/server`, writes `version.txt` so the launcher
does not download, sets a flat world, `online-mode=false`, `allow-list=false` and `transport=raknet` on `--port`
(default 19600, IPv6 on port + 1, the control channel on port + 2), then re-runs itself from the venv. Every Python
it starts runs with `PYTHONSAFEPATH=1` in the workdir, so the checkout's `endstone/` source package never shadows
the wheel, whatever the current directory. On Windows the workdir must be short: BDS ships paths of about 150
characters.

| Option | Effect |
|--------|--------|
| `--select <glob>` | Run only matching scenarios. A selected run is partial: skips and missing coverage do not fail it. |
| `--extra <package>` | Install another package into the venv: a requirement, a project directory, a wheel, or a wheel glob (the newest match wins). Repeatable. |
| `--find-links <dir>` | A directory of wheels that dependencies may resolve from. Repeatable. |
| `--pytest-file <file>` | Run only this file of `tests/` in the pytest passes, such as `test_chunk.py` or `player/test_ability.py::test_x`. Repeatable. Implies `--select "pytest.*"` unless you select scenarios yourself. |
| `--pytest-args=<args>` | Extra pytest arguments as one string, such as `--pytest-args="-k chunk -x"`. Write it with `=`, since the value starts with a dash. Also implies `--select "pytest.*"`. |
| `--timeout`, `--hang-timeout` | Wall-clock limit for the run, and for silence from the plugin. |
| `--crash-test` | Enable `harness.crash`, which crashes the server on purpose. |
| `--api-gate {enforce,report,off}` | Record which public API members run in the server and add `api_coverage` to the result. `enforce` (the default) fails a full run when a member never ran and has no reason in `NOT_AUTOMATABLE` in `harness/api_manifest.py`; `report` only reports; `off` does not record. |
| `--verbose` | Echo the server log. |

A project directory given to `--extra` is reinstalled on every run, together with the packages it takes from a
local path in `[tool.uv.sources]`. `endstone-client` takes its bedrock-protocol binding that way, so
`--extra ../endstone-client` also installs the newest build of
`../bedrock-protocol/wheelhouse/bedrock_protocol-*.whl`. A wheel given directly is reinstalled
too, so a rebuild with an unchanged version is still picked up.

The result is `<workdir>/results-<run id>.json` and the server log is `<workdir>/logs/<run id>.log`. Each scenario's
entry holds `status`, `detail`, `ticks`, `wall`, the captured `events`, and whatever the scenario put in `ctx.info`
under `info`. A control-channel line the runner cannot read (unparsable, or over its 256 MB limit) fails the scenario
that was running and is listed under `control_errors`. A full run fails
when a scenario fails, errors, crashes, never runs or skips, when pytest skips a test, when the log shows
`Could not pass event`, `Could not execute task`, `Unhandled exception`, a traceback or a crash banner, or when an
event path has neither a scenario nor a reason in the coverage manifest. After a crash the runner charges it to the
scenario that was running, restarts the server and carries on with the scenarios that are left, for as many crashes
as it takes; `--max-restarts` (default 3) only limits server lives in a row that end before any scenario finishes.

## How it works

Without `ENDSTONE_TEST_CONTROL` the plugin behaves as before: it runs the server pytest pass on enable and offers
`/test`. With it, the plugin connects to the runner, reports its scenarios and the coverage manifest, and runs the
plan the runner sends back.

- **Scenarios** (`scenarios/`) are async functions resumed once per server tick. They await `ctx.ticks(n)`,
  `ctx.until(predicate, timeout)` and calls into the runner, and never block the server thread. Each has a tick
  budget, and one with `plot=True` gets a cleared 16x24x16 box on a stone floor inside a ticking area.
- **Captures** (`ctx.capture(EventClass, where, cancel=, mutate=)`) record every matching event while the scenario
  runs. Cancelling and mutating happen inside the handler, and each event is snapshotted field by field. Every
  captured event is also checked against the invariants in `harness/fields.py`. Deaths and respawns are journaled
  whether or not a scenario captures them, and the cross-event checks there run when each scenario ends.
- **World access without a player.** The behavior pack in `assets/behavior_pack` is installed into the scratch world;
  `/scriptevent endstone_test:setblock` and `endstone_test:explode` reach it. The fixture plugin
  `endstone-test-fixture` exists to be disabled and enabled again.
- **Paths.** A scenario declares which paths it covers, `normal`, `cancel` or `mutate`, and asserts what the game did:
  the block that formed, the weather `/weather query` reports, the line the console printed. `harness/manifest.py`
  requires `normal` for every event, `cancel` for a cancellable one and `mutate` for one with a setter, unless it
  states why a path cannot be automated.
- **The runner** (`runner.py`) owns the server process, the log watch, the bots and the CPU sampler. The plugin asks
  it to type console commands, return the log lines written between two marks, start, drive and close bots, and
  sample the server thread.
- **The pytest suites** under `tests/` run as the `pytest.server` and `pytest.player` scenarios, and their exit code
  is the scenario's verdict.

### Bots

Player scenarios drive bots through `await ctx.bot(name)` and `bot.call("local_player.jump")`. The runner resolves
the dotted method on the bot's `endstone_client.ClientInstance` (only its `@action` methods) and returns its JSON.
`Bots` in `runner.py` is the only code that knows the client library; without `--extra` pointing at the client,
bot scenarios skip, which fails a full run.

### Server thread CPU

`await ctx.call("perf.start")` and `await ctx.call("perf.stop")` sample the CPU the server thread spends per
wall-clock second. The thread is the one the plugin's tasks run on; without the plugin, the thread BDS names
`MC_SERVER` (the lowest-numbered one on Linux, where its children inherit the name). Windows reads exact cycle
counts rather than thread times, which only advance at clock interrupts; Linux reads `schedstat`.

`--bench` measures a join storm without the plugin, so builds that cannot load it compare too:

```shell
... runner.py --wheel <endstone.whl> --bds-zip <zip> --workdir C:/esb --port 19610 --bench --bots 50
... runner.py --wheel <any wheel> --bds-zip <zip> --workdir C:/esv --port 19620 --bench --bots 50 --vanilla
```

It waits for `Server started.`, samples the idle server, joins every bot at once, samples again, and writes
`<workdir>/bench-<run id>.json`. `--vanilla` runs the zip's `bedrock_server` directly; the wheel still provides
the runner's venv and the bots.

## Writing a scenario

```python
@scenario("weather.rain.cancel", covers={WeatherChangeEvent: "cancel"})
async def rain_cancel(ctx: Context) -> None:
    rain = ctx.capture(WeatherChangeEvent, cancel=True)
    ctx.run("weather rain")
    await ctx.until(lambda: rain.count, what="WeatherChangeEvent")
    await ctx.ticks(40)
    assert weather(ctx) == "clear"
```

Assert on the game, not on the event: a cancel scenario checks the world and, once bots exist, what the client was
sent. Reading back a setter proves nothing.

A scenario may `/reload` the server. The reloaded plugin connects to the runner again and runs the interrupted
scenario from the start, so keep what the first run did in `ctx.stash`, a dict that survives the reload and is
cleared when the scenario ends. Harness setup is safe to repeat: the plot field's ticking area is reused if it
already exists. Once that scenario ends, the runner stops the server and runs the rest in a fresh one, since a reload
leaves state behind that later scenarios should not inherit; `--keep-reloaded-server` turns that off.

## Manual checks

Some API surface needs human eyes. These stay behind `/test`:

```
/test                                            run the pytest suite (the player pass when a player runs it)
/test form <message|action|modal>                MessageForm / ActionForm / ModalForm and every control
/test player <toast|title|actionbar|kick|particle|sound>
/test inv <mainhand|offhand|meta>                PlayerInventory hand slots and ItemMeta
/test spawn <actor: entity_type>                 Dimension.spawn_actor
/test map                                        a map drawn by ImageRenderer
/test block <block: block> [states: block_states]
/test sender
```

## Writing tests

The pytest suites have the live server through fixtures: `server`, `plugin`, `plot`, and `player` in the player pass.
A test needing a connected player goes in `tests/player/`. There is no API to unregister a listener, so never register
one inside a test.

The server thread is blocked while pytest runs, so no tick passes during a test. A chunk the server pass loads
itself with `load_chunk` is held but never ticks: `setblock` there fails and `spawn_actor` returns `None`. Use the
`plot` fixture for anything that needs a live spot. It is the harness plot reserved for the pass: a 16 x 24 x 16 box of
air on a stone floor, inside a ticking area, cleared before the pass.

```python
def test_mob_attribute(server, plot):
    overworld = server.level.get_dimension("overworld")
    x, y, z = plot.pos(*plot.middle)          # world coordinates of a spot in the plot
    pig = overworld.spawn_actor(Location(overworld, x + 0.5, y, z + 0.5), "minecraft:pig")
    try:
        ...
    finally:
        pig.remove()
```

`plot.pos(dx, dy, dz)` gives world coordinates, `plot.at(...)` the same as an `"x y z"` command argument, and
`plot.volume` a selector argument covering the box. Each pass gets its own plot, shared by every test in it, so stay
inside the box and clean up what you spawn. Outside the harness (`/test` in game) there is no plot, and tests that ask
for it skip.

Run a single file or test quickly with `--pytest-file`, for example:

```shell
... runner.py --wheel <whl> --bds-zip <zip> --workdir C:/esh --pytest-file test_plot.py --pytest-args="-v"
```
