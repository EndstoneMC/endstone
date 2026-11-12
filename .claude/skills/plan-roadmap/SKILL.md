---
name: plan-roadmap
description: Plans Endstone's roadmap on GitHub org project 1 - which features go into which phase, their priority, size, release milestone and ETA - from what shipped, open requests, Paper and ScriptAPI coverage gaps and new BDS capabilities, then has project-manager apply the approved plan. Use when asked what to build next, to scope the next minor, to plan phases or ETAs, to rank feature requests, or to refresh the roadmap.
---

# Plan Roadmap

Read first: `.claude/rules/product/roadmap.md`, `.claude/rules/product/api.md`, `.claude/rules/github.md`.

1. Have `project-manager` run `track-roadmap` and return the current board.
2. Collect open requests: `gh issue list -R EndstoneMC/endstone --state open -L 500 --json number,title,labels,reactionGroups`, plus the Ideas category of GitHub Discussions.
3. Fan out `product-designer` agents, one per Paper API package, to list the classes and methods with no Endstone counterpart in `include/endstone/`.
4. Have `product-designer` list what ScriptAPI (`@minecraft/server`) offers that Endstone lacks.
5. Have `product-designer` diff `Mojang/bedrock-protocol-docs` between the previous and the supported BDS version and list the packets and fields no Endstone API reaches.
6. Take the cadence and BDS upgrade load from `re-lead`, the velocity, sizes and open-PR readiness from `dev-lead`, and the release readiness and validation cost from `qa-lead`, as the main session collects them.
7. Group the items into themes for plugin developers and for server admins, and order the themes into phases.
8. Give each item a Priority, Size and Area, and give the next minor's items its milestone, a Start Date and a Target Date.
9. Have `re-lead`, `dev-lead` and `qa-lead` review the draft for deliverability, then revise it.
10. Hand the main session the plan, one row per item with title, phase, priority, size, milestone, start and target; it approves once every lead's review is resolved, and the user is told the result, never asked to approve it.
11. Have `project-manager` apply the approved plan with `track-roadmap` and confirm every value on the board.
