---
name: track-roadmap
description: Syncs Endstone's roadmap board, GitHub org project 1, with the state of its issues and pull requests, adds new Feature issues, applies an approved plan, and reports progress per phase, overdue ETAs and missing fields. Use for a roadmap status report, after a release or a batch of merges, when applying a plan from plan-roadmap, or when asked where the roadmap stands.
---

# Track Roadmap

Read first: `.claude/rules/product/roadmap.md`.

1. Resolve ids: `gh project view 1 --owner EndstoneMC --format json -q .id` for the project, and `gh project field-list 1 --owner EndstoneMC -L 100 --format json` for fields and options.
2. Create each field the rule lists that the project lacks: `gh project field-create 1 --owner EndstoneMC --name <name> --data-type <SINGLE_SELECT|DATE>`, with `--single-select-options "<a,b,c>"` from the rule or the brief.
3. List the board: `gh project item-list 1 --owner EndstoneMC -L 1000 --format json`.
4. Add open Feature issues missing from the board: `gh issue list -R EndstoneMC/endstone --state open --search "type:Feature" -L 500 --json url`, then `gh project item-add 1 --owner EndstoneMC --url <url>` for each.
5. Set each item's Status from its issue and linked pull requests: `gh project item-edit --id <item> --project-id <project> --field-id <field> --single-select-option-id <option>`.
6. Apply the plan in the brief: Phase, Priority, Size and Area with `--single-select-option-id`, Start Date and Target Date with `--date YYYY-MM-DD`, and milestones with `gh issue edit <N> -R <repo> --milestone vX.Y.0`.
7. Convert each newly scheduled draft with `convertProjectV2DraftIssueItemToIssue` through `gh api graphql`, set its Feature type with `updateIssueIssueType`, and attach sub-issues with `addSubIssue`.
8. Re-list the board and check that every value the brief set held.
9. Report per phase the counts by Status, the items past their Target Date and not Done, the items missing Phase, Priority or Target Date, and one row per change.
