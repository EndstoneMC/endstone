---
name: implement-api
description: Adds a public API, event, enum, registry type or method to Endstone across every layer it needs, then proves it on Windows, Linux and NetherNet. Use when asked to add or expose a feature in the API, such as "add a new event", "expose X on Player", "add a RespawnReason enum", "implement <feature> in the API" or "QA the new API".
---

# Implement API

Read first: `.claude/rules/product/api.md`, `.claude/rules/dev/core.md`, `.claude/rules/dev/hooks.md`, `.claude/rules/dev/python.md`, `.claude/rules/qa/testing.md`, `.claude/rules/github.md`.

1. Read Paper's API and server source for the feature; stop and ask the user when the branch is a released `vX.Y` line.
2. Add the public header under `include/endstone/<package>/` and its entry in `endstone.hpp`.
3. Add the bedrock layer in `src/bedrock`; delegate to `bedrock-developer` for more than an accessor.
4. Add the core wrapper in `src/endstone/core/`; a registry type also gets its `registry.cpp` specialisation and `registries_[typeid(X)]` in `server.cpp`.
5. Add a runtime hook only when behaviour or an event needs one; hand a new symbol to `reverse-engineer`.
6. Add the Python binding and docstrings in `src/endstone/python/`.
7. Run `verify-change`; `git diff` must show only your additions in the regenerated stubs.
8. Add a pytest per new method, a scenario per event path and each registry type to `test_registry`.
9. Run `git diff v<last release>..HEAD -- include/endstone` and have `api-developer` write scenarios for any API the gate reports missing.
10. Run `validate-change` on targeted scenarios, then full Windows, Linux and NetherNet.
11. Have `code-reviewer` check each batch, then read every diff yourself.
12. Add the CHANGELOG entry; commit only when the user asked.
13. Report a pass only when the three full runs exit 0 with `0 missing`; hand over the result files, open failures and the commit range.
