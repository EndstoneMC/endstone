---
name: fix-issue
description: Diagnoses and fixes an Endstone bug report such as a server crash, an event or API that differs from vanilla BDS or Paper, an event that never fires, or a Python binding that raises, then lands the fix and answers the reporter. Use when asked to "fix issue N", "look at this crash report", "X doesn't work like vanilla", "event Y doesn't fire" or "Z crashes the server".
---

# Fix Issue

Read first: `.claude/rules/re/evidence.md`, `.claude/rules/dev/core.md`, `.claude/rules/github.md`, `.claude/rules/qa/testing.md`.

1. Triage a crash by exception code and first post-handler frame resolved to an RVA, and a behaviour report by class.
2. Reproduce at the reported release on `bedrock_server` with the reporter's `server.properties` changes: a console drive from Bash for server-only behaviour, `validate-change` for player-driven behaviour.
3. When it does not reproduce, change nothing and ask the reporter for OS or container, `/plugins` output, the log around one occurrence and whether plain BDS behaves the same.
4. Have `reverse-engineer` decompile the BDS function at the target version and read Paper's implementation before naming a cause.
5. Read the rule file for the class of bug: `re/reconstruction.md` and `re/abi.md`, `dev/core.md`, `re/reimplementation.md`, `dev/hooks.md` and `product/api.md`, `dev/python.md`, `dev/network.md` or `dev/threading.md`.
6. Fix the root cause and grep the file and its siblings for the same mistake.
7. Run `verify-change`, plus `validate-change` for BDS-backed behaviour on both platforms when a layout changed.
8. Land the regression scenario with the fix and list what was not verified.
9. Add the CHANGELOG line ending `(#N)`; commit and back-port per `github.md` only when the user asked.
10. When asked to reply, draft the comment, show the user, then run `gh issue comment <N> --body-file <file>` after approval.
11. When no fix is possible, ask the user to confirm, then run `gh issue close <N> --reason "not planned"` with the explanatory comment.
