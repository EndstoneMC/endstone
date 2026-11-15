# Stable-readiness test plugin

Operator-driven probes for the existing Zernyth Java 25 API. The plugin registers
`/zernythtest` with permission `zernyth.test`. Native dispatch checks the permission,
and the Java callback checks it defensively again. Activation registers the command
and logs lifecycle order. Tests run only when explicitly requested.

## Build and installation

From `java/`, using locally installed Gradle 9.1+ and Eclipse Temurin 25:

```powershell
gradle --offline --no-daemon :stable-test:build :stable-test:javadoc
```

With an existing alternative JDK 25, add `-PzernythJavaVendor=any`.
Toolchain auto-download remains disabled. No additional library is required.
The bounded `selfTest` JavaExec suite runs through `check`, with a 128 MiB heap
and a 30-second limit. The JUnit task is disabled because this suite uses no JUnit.

Install `java/stable-test/build/libs/stable-test-1.0.0.jar` in the separate
BDS test server's `plugins/` directory. Use the version from `zernyth-version.txt`
when the project version changes. Install the regular JAR. Its API dependency is
compile-only. The engine supplies the matching API and runtime.

The locally prepared installation is under `build/stable-validation/`:

- `venv/` contains Zernyth installed from TestPyPI and cached Python dependencies.
- `packages/` contains the downloaded Windows x64 CPython 3.15 wheel.
- `server/` contains a separate copy of the existing BDS resources and this plugin.
- `server/server.properties` selects `zernyth-stable-test`, a fresh world on first start.
- `validation.json` records the installation, artifact and test checks.

No original worlds, player permissions, allowlist entries or other plugins are copied.
The operator must configure a test account and establish network isolation before starting.
The existing BDS installation is the read-only source of the local test copy.

## Commands

All command callbacks and native API calls run on the server thread. Event and packet
callbacks retain only counters. Form responses use nonblocking completion callbacks.
Command senders are callback-scoped and are never captured by delayed tasks.

| Subcommand | Probe |
| --- | --- |
| `lifecycle` | Current plugin diagnostics and ownership counts |
| `commands` | Permission-protected native command dispatch |
| `events start/check/stop` | Native movement observation and registration removal |
| `scheduler` | Cancel a 20-tick task and observe a separate 40-tick callback |
| `services` | Register, call and revoke a native-backed service proxy |
| `config` | Explicit save/reload in the dedicated `stable-probe.yml` |
| `gameplay read` | Read the dedicated world, dimensions and an existing entity |
| `gameplay player` | Connected player's UUID, position, mode, inventory and entity view |
| `gameplay block X Y Z` | Explicit single-block change and restoration |
| `forms` | MessageForm response validation from a connected test player |
| `network start/check/stop` | Capability-checked, read-only packet metadata counting |
| `cleanup` | Attempt every owned cleanup operation, isolating individual failures |
| `cleanup check` | Check native listener, task and service counts after cleanup |

Each result uses `PASS`, `FAIL` or `NOT TESTED`, a probe name and a reason in the
server log. These statuses cover the specific operation described by each line.
A successful cleanup request does not prove that all running work has ended.
After callbacks have drained, invoke `cleanup check` separately.
The command remains registered until plugin disable.

The world-dependent probes require exactly one world named `zernyth-stable-test`.
This name check protects against accidentally selecting another world. It cannot
prove network isolation or that an operator has supplied a disposable world.
Never rename a production world to satisfy this guard.

## Manual BDS and Bedrock test checklist

Record PASS, FAIL or NOT TESTED for every item, together with date, BDS and client
versions, engine and plugin versions, JAR SHA-256, JDK vendor, and supporting logs.
A server start alone does not validate player-dependent functionality.

1. Establish isolated networking externally. Disabled STUN and LAN discovery do
   not prove that native UDP listeners are isolated. Keep the default deny allowlist
   until the intended Bedrock test account has been configured.
2. Start the operator-managed test server. Verify JVM initialization, JAR discovery,
   `lifecycle.load` before `lifecycle.enable`, and exactly one command registration.
3. Run `commands` from the console and an authorized test player. Verify rejection
   for a player without `zernyth.test`. Unknown subcommands must display help.
4. Run `lifecycle` and record baseline diagnostics.
5. Run `events start`, move the connected client, then `events check`. Expect a
   positive count. Run `events stop`, move again, then check that the count remains
   unchanged. After stop, the plugin reports NOT TESTED unless unexpected callbacks
   occur. The operator must confirm that client activity actually occurred.
6. Run `scheduler` and wait at least 40 ticks. The cancelled callback must never
   run. The 40-tick callback must report PASS. A concurrent invocation must refuse
   another probe. Repeat, requesting cleanup or native plugin disable before expiry.
7. Run `services`. Expect successful lookup/invocation and rejection of the revoked
   proxy. Cross-plugin imports, priorities and provider failure need separate fixtures.
8. Run `config`. Inspect only the plugin-owned `stable-probe.yml`. Disable must not
   implicitly save configuration. Malformed files and path traversal remain covered
   by the engine's automated regression suite and need separate real-server checks.
9. Run `gameplay read`. Connect a Bedrock test client and run `gameplay player`.
   The console must receive NOT TESTED for the player probe. Compare the recorded
   position, mode and inventory size with the actual player.
10. Prepare an isolated stone or dirt block in a loaded overworld chunk, away from
    redstone, block entities and sensitive neighbors. Explicitly run
    `gameplay block X Y Z` once. Verify the temporary type and restored block data.
    Neighbor updates and other side effects are not guaranteed reversible. An unloaded
    block or invalid coordinates fail without automatic chunk generation.
11. Run `forms` as the connected player. Test both buttons, closing the UI,
    disconnecting with an open form, cleanup, and disable in separate runs. Submitted
    responses must contain exactly one integer selection in 0..1. Invalid responses
    report FAIL. Other terminal statuses are reported for operator interpretation.
12. Run `network check`, record native protocol and capabilities, then `network start`.
    Generate client activity, check for a positive callback count, stop, generate more
    traffic, and check that the count remains unchanged. Unsupported adapters report
    NOT TESTED. No payloads, credentials, packet modification or sends are exposed.
13. Run `cleanup` repeatedly and, after pending callbacks have drained, `cleanup check`.
    Compare counters with baseline. An individual cleanup failure must be logged and
    must not prevent the remaining resources from being closed.
14. Disable through the existing native plugin manager with observers, timer or form
    open. Verify command removal and absence of later callbacks externally. Re-enable
    through that same manager and verify the new instance/owner generation. The plugin
    cannot inspect its revoked facade after full disable. Unsupported live management
    must remain NOT TESTED; perform a controlled stop/start instead.
15. Stop through the BDS console with `stop`. Verify the disable logs, exit status and
    final native cleanup. Do not force-kill Java code or restart the embedded JVM.

Other event types, priorities/cancellation, async overload, inventory mutation,
teleportation, spawning, cross-plugin service behavior and prolonged load remain
separate stable-release gates. This focused probe does not claim complete coverage.

## Automated validation

The eight dependency-free mock groups cover packed BDS message arguments, exact world selection, permission and
world guards, scheduler cancellation and overlapping probes, cleanup failure isolation,
repeated cleanup, command release, unsupported networking, and missing test clients, plus valid/malformed form selections and form cleanup.
The synthetic wrong-world test intentionally produces a FAIL gameplay log while
asserting that no world mutation is reached. A mock PASS does not validate BDS.

Local validation uses Oracle JDK 25.0.5.0.3 with the documented vendor override.
The build checks Java 25 bytecode, `-Xlint:all -Werror`, and Javadoc Doclint/Werror.
See `build/stable-validation/validation.json` for the actual installation/test results.
BDS/client validation and a stable-release decision remain pending.

## First real BDS/client run and local corrections

The first operator-run test on BDS 1.26.60.6 confirmed loading/enabling/disabling,
a connected Bedrock client, authorized command execution, services, configuration,
scheduler cancellation and one submitted MessageForm response. The operator reported
exit code 0. Diagnostics failed because Java Thread identity changed across JNI
reattachment. The correction uses the existing native owner/server-thread check,
with a reproducing native regression and continued worker rejection.

Multi-part test commands arrived through BDS as one message parameter. The test
plugin now normalizes that restricted argument format, with packed-message regressions.
Gameplay, events, networking and cleanup-counter results must be repeated manually.
The local validation venv contains a patched runtime with the original TestPyPI
runtime backed up. The published alpha wheel has not been updated.

### Post-fix BDS/client retest

The operator repeated the focused checks on BDS 1.26.60.6. Native-backed diagnostics,
world/dimension/player/inventory reads, existing entities, 170 movement callbacks,
2730 packet callbacks with zero callback errors/wrong-thread drops, listener closure,
zero native listener/task/service counts after cleanup, a valid form response and
plugin disable are confirmed by the server log. No FAIL probe result appears in
that run. Remaining feature gates listed above still need individual validation.
The locally patched runtime has not been published as a new release artifact.

The optional block probe resolves the native Overworld identifier without relying
on lowercase spelling. Mock checks cover both normal restoration and restoration
after a simulated readback failure. A real BDS block mutation remains NOT TESTED.
