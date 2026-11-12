---
name: design-feature
description: Turns a feature request into an approved Endstone product proposal covering the C++ and Python API, events and their cancel contract, and any server-admin UX of config, commands or console output, grounded in Paper's API and what BDS supports. Use before building a new API, event or UX change, when a request needs a decision on whether and how it fits, or when asked to "design X", "propose an API for Y" or "how should Z look".
---

# Design Feature

Read first: `.claude/rules/product/api.md`, `.claude/rules/dev/python.md`, `.claude/rules/github.md`.

1. Collect the request and who hits it; find related reports with `gh issue list --search "<terms>" --state all`.
2. Read Endstone's current surface for the area in `include/endstone/` and `endstone/**/*.pyi`.
3. Read Paper's and Bukkit's API for the feature and list the classes, methods and events to mirror.
4. Have `reverse-engineer` report what BDS exposes for it at the supported version: gameplay events, packets and fields.
5. Pick the release line and mark each change breaking or not per `github.md`.
6. Write the proposal: the problem and its users, the C++ declarations with one-line docs, the Python names, each event's trigger and cancel contract against Paper, the config, command and console changes, what is out of scope, and the CHANGELOG line.
7. List every deviation from Paper with its BDS reason, and every open question.
8. Show the proposal to the user and revise it until approved, then hand it to `dev-lead` for `implement-api`.
