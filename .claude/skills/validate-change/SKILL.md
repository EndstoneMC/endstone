---
name: validate-change
description: Builds a wheel and runs the tests/plugin harness against the repository's bedrock_server on Windows, Linux or NetherNet, then reports each scenario's result. Use when a change needs proof on a running server, to run one scenario alone, to run the crash test or benchmark before a release, or at the live-server step of implement-api, fix-issue or upgrade-bds.
---

# Validate Change

Read first: `.claude/rules/qa/testing.md`, `.claude/rules/build.md`, `.claude/rules/environments.md`.

1. Build the wheel: `uv build --wheel -o wheelhouse`, from a Visual Studio developer prompt on Windows.
2. Fetch the BDS zip for the version in `scripts/configs/<platform>.toml`: `uv run --script .claude/skills/upgrade-bds/fetch_bds.py --platform <windows|linux>`.
3. Back up `bedrock_server/{worlds,plugins,logs,crash_reports,endstone.toml}` into a short scratch directory outside any repo.
4. Run in the background: `uv run --no-project --python 3.13 python tests/plugin/src/endstone_test/runner.py --wheel <whl> --bds-zip <zip> --server-dir bedrock_server --workdir <scratch>/run --extra ../endstone-client`.
5. Narrow or vary the run with `--select "<glob>"`, `--pytest-file <file>` with `--pytest-args=<args>`, `--transport nethernet` or `--port <n>`.
6. For Linux, build the manylinux wheel and run the same command in a Linux container with the Linux zip and the repo mounted.
7. For full coverage run one server at a time: targeted `--select` until green, then full Windows, full Linux, full `--transport nethernet`.
8. Before a release, add `--crash-test`, then `--bench --bots 100` and the same with `--vanilla`.
9. Read `<scratch>/run/results-<run id>.json` and `<scratch>/run/logs/<run id>.log` and report each scenario's status; pass only with `0 missing`.
10. Triage each failure alone with `--select` as a known bug, a regression, a new bug (hand off to `fix-issue`) or a harness defect.
11. Restore the backup into `bedrock_server` and delete anything the run created outside the scratch directory.
