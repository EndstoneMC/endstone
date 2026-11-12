---
name: review-pr
description: Reviews one Endstone pull request or local commit range that adds or changes API, events, hooks, bindings or src/bedrock declarations, and writes a structured report. Use when asked to review PR N, check the open PRs, triage a contribution, or before merging a community PR.
---

# Review PR

Read first: every file under `.claude/rules/`.

1. Review one PR per invocation; fan out one agent per PR.
2. Stay read-only unless `--comment`: `gh pr view <N> --json title,author,baseRefName,headRefOid,files,commits`, `git fetch origin pull/<N>/head`, then `git diff <base>...FETCH_HEAD`; a local range uses `git diff <base>...<head>`.
3. Run `git log <base>..FETCH_HEAD` and note a stale base and commits outside the title's scope.
4. Run `git merge-tree --write-tree <base> FETCH_HEAD` against the base and against each other open PR from `gh pr list`; state a safe merge order.
5. Grep the `paths` frontmatter in `.claude/rules/` for globs matching the changed files, read those rules plus `github.md` and `comments.md`, and check each hunk.
6. Fetch Paper's and Bukkit's source for each new or changed API and compare it.
7. Check every added, moved, retyped or reordered `src/bedrock` declaration, one row per declaration.
8. Check new symbols, the trigger and cancel contract, bindings, stubs, CHANGELOG and commit format.
9. Write the report in the format below; with `--comment` post it once with `gh pr review <N> --comment --body-file <file>`, else print it.

```
Review draft PRs too and lead the report with the draft status. Title `PR #N - <title> (@author)`, then these lines:

- Verdict: request changes, approve with suggestions, or approve
- Scope: what the PR does and whether the title matches
- Base: stale or clean, stray commits
- New symbols: anchor chosen and why
- Trigger: where it fires, against Paper's call site

Then these sections:

- Blocking: each item is the rule broken, `file:line` and the rule file
- Suggestions: findings no rule backs
- Bedrock verification: a table of declaration, `ok | wrong | unverified`, and source
- Questions: claims you could not source
- Cross-PR: conflicts and merge order

Cite both sides of every finding, and put a claim you cannot source under Questions. Do not pad: three sourced blockers beat twenty guesses. Say when a claim rests on a build or runtime value you cannot produce.

Cite BDS header evidence as "confirmed against the BDS headers" or as an IDA address plus the one decompiled line. Never paste header text into a report, comment or commit.
```
