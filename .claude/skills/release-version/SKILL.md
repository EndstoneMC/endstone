---
name: release-version
description: Cuts an Endstone release from a vX.Y branch by curating the CHANGELOG, tagging, pushing, watching the build and verifying the published artifacts. Use when asked to release, cut a release, publish X.Y.Z or ship a patch.
disable-model-invocation: true
---

# Release Version

Read first: `.claude/rules/github.md`, `.claude/rules/build.md`, `.claude/rules/environments.md`.

1. Run `git tag --sort=-version:refname | head -n5` and `git rev-parse --abbrev-ref HEAD`; the branch must be `vX.Y` and the target is the previous patch plus one unless the user named another.
2. Check that `project(endstone VERSION ...)` in `include/CMakeLists.txt` equals the target and the BDS version surfaces in `re/symbols.md` equal the supported BDS; when an edit is needed and the version is unconfirmed, stop and ask the user.
3. Pre-flight: `git status --porcelain` is empty, `git rev-list --left-right --count @{u}...HEAD` is `0 0`, and the latest `build.yml` and `ci.yml` runs for HEAD are `success`, and `qa-lead` reports the pre-release `validate-change` runs green; stop on any failure.
4. Have `devops-engineer` deploy a candidate wheel built from HEAD to UAT, and wait for `qa-lead` to sign UAT off; on a UAT failure stop and hand it to `fix-issue` as a failing QA scenario.
5. Have `product-lead` curate `[Unreleased]` against `git log --format='%h %s' v<prev>..HEAD`; stop and ask the user to approve the body.
6. Stamp `CHANGELOG.md`: move the bullets under `## [X.Y.Z] - <UTC date>` with an empty `## [Unreleased]` above, repoint `[Unreleased]` to `compare/vX.Y.Z...HEAD`, add the `[X.Y.Z]` compare link.
7. Stop and ask the user to confirm the tag push.
8. Run `git commit -am "Release X.Y.Z"`, `git tag -a vX.Y.Z -m "Release X.Y.Z"`, then `git push --follow-tags origin <branch>`; when rejected, `git pull --rebase origin <branch>` and push again.
9. Find the tag commit's `build.yml` run with `gh run list --workflow=build.yml --json databaseId,headBranch,headSha,status,conclusion --limit 10` and watch it in the background with `gh run watch <id>`.
10. Have `devops-engineer` check PROD in parallel: `gh release view vX.Y.Z`, `pip index versions endstone`, the GHCR tags via `gh api orgs/EndstoneMC/packages/container/endstone/versions`, and the Docker Hub tags; report each.
11. Fast-forward `main`: `git fetch origin main` and `git rev-list --left-right --count origin/main...origin/<branch>`; on `0 N` ask the user to confirm then `git push origin origin/<branch>:main`, on `M N` with M above 0 stop and ask.
12. When the user asks, merge the branch forward into the newer `vX.Y` branch per `github.md`.
13. When the wrong commit was tagged, ask the user to confirm, then delete the tag locally and on origin, yank from PyPI, delete the GitHub release and restart at step 6.
