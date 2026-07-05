---
name: release
description: Cut a new Endstone release end-to-end - run prereq checks, curate CHANGELOG, stamp it, commit, tag, push, then watch the Build workflow publish. Use when the user asks to release/ship a new patch or minor version (e.g. "prepare release v0.11.4", "ship v0.12.0", "cut a release").
---

# Release Endstone

This skill drives the release. Execute the steps in order. Each step has
explicit commands; do not skip ahead. Stop and ask the user only at the
points flagged "**STOP**".

## Pipeline (what happens after the tag is pushed)

The repo splits its CI across three workflows:

- **`ci.yml`** - Release builds on Linux + Windows, Debug+coverage on
  Linux. Runs on PR + push + dispatch. **Does NOT trigger on tags.**
- **`docker.yml`** - Publishes `ghcr.io/endstonemc/endstone:nightly`
  from the from-source root `Dockerfile`. Develop branch only.
- **`build.yml`** - Wheels matrix, bundle, docker image (short-path),
  PyPI publish, GitHub Release. **The only workflow that fires on tag
  push** - because ci.yml's triggers exclude tags and docker.yml is
  develop-only.

```
git push --follow-tags origin <branch>
                |
                v
        build.yml (DAG of jobs on tag push)
        +-----------------------------------+
        |   wheels    (cp310-14 x 2 OS)     |
        |       |                           |
        |       +-- bundle  (zip per OS)    |
        |       +-- docker  (GHCR/Hub push) |
        |       +-- release (PyPI + GH)     |
        +-----------------------------------+
```

`release` is a job inside `build.yml` that fires only on tag pushes; it
needs `wheels` + `bundle`, publishes wheels to PyPI via OIDC, and
creates the GitHub release with body extracted from `CHANGELOG.md`.
`docker` builds the short-path image from the cp313 manylinux wheel
artifact and pushes versioned + `:latest` to GHCR and Docker Hub.

Because the tag push is authored by the user (not `GITHUB_TOKEN`),
`build.yml` actually triggers - that's why this skill does the tag push
from the local checkout instead of from a workflow.

The package version comes from `setuptools_scm` (the tag IS the version).
There is no `version = ...` field to edit.

---

## Step 0 - figure out the target version

Run these in parallel:

```shell
git tag --sort=-version:refname | head -n5     # last 5 tags
git rev-parse --abbrev-ref HEAD                # current branch
```

The release branch must be `v0.<minor>` (currently `v0.11`); never release
from `main`. Compute the next version:

- Default = previous tag's patch + 1 (e.g. `v0.11.3` -> `0.11.4`).
- For a minor/major bump (`0.12.0`, `1.0.0`), the user must have said so.

Verify all version-touching files match. The `bump-bds` flow updates the BDS
macros in `include/endstone/detail.h` but typically misses other surfaces -
check each one and edit it inline if it's stale.

```shell
grep -E 'project\(endstone VERSION' include/CMakeLists.txt
grep -E 'MINECRAFT_VERSION_(MAJOR|MINOR|PATCH)|NETWORK_PROTOCOL_VERSION' include/endstone/detail.h
grep -E 'minecraft-v[0-9]+\.[0-9]+_\(Bedrock\)' README.md
```

Checklist:

1. **`include/CMakeLists.txt`** `project(endstone VERSION x.y.z ...)` = the
   version being released. This is the fallback; the wheel build overrides
   `ENDSTONE_VERSION` with the exact setuptools_scm version, but the numeric
   major/minor/patch and `ENDSTONE_API_VERSION` come from here.
2. **`include/endstone/detail.h`** must have:
   - `MINECRAFT_VERSION_MAJOR/MINOR/PATCH` = the supported BDS version
   - `NETWORK_PROTOCOL_VERSION` = the BDS network protocol version
   If any are stale, edit the macros directly.
3. **`README.md` Minecraft badge**
   (`https://img.shields.io/badge/minecraft-vNN.NN_(Bedrock)-black`) must
   match `MINECRAFT_VERSION_MINOR.MINECRAFT_VERSION_PATCH` from
   `detail.h`. Update with `Edit` if stale - the badge URL is the only
   spot users see the supported BDS version on the project landing page.

If any update is needed and the user hasn't already confirmed the target
version, **STOP** to confirm before editing.

---

## Step 1 - pre-flight checks (all must pass)

Run in parallel:

```shell
git status --porcelain                                                                       # must be empty
git rev-list --left-right --count @{u}...HEAD                                                # must be "0 0"
gh run list --branch $(git rev-parse --abbrev-ref HEAD) --workflow=build.yml --limit 3
gh run list --branch $(git rev-parse --abbrev-ref HEAD) --workflow=ci.yml --limit 3
```

Pass conditions:

1. **Working tree clean.** `git status --porcelain` returns nothing.
2. **In sync with origin.** Left-right count is `0  0`.
3. **Build green on HEAD.** Latest `Build` run on this branch concludes
   `success`. **Every** wheel matrix job must pass - one failed
   `manylinux_x86_64` or `win_amd64` job means the release job's PyPI
   publish fails after the tag push, leaving an orphan tag.
4. **CI green on HEAD.** Latest `CI` run (Release + Coverage) concludes
   `success`. Less critical than Build (CI doesn't gate the release
   path), but a red CI on the tag commit is a smell - investigate.

If any check fails: surface the exact failure to the user and **STOP**.
Do not proceed with a dirty tree, an out-of-sync branch, or failing Build.

For a failed matrix job, fetch the log to diagnose:

```shell
gh api repos/EndstoneMC/endstone/actions/jobs/<job_id>/logs 2>&1 | tail -100
```

---

## Step 2 - curate CHANGELOG `[Unreleased]`

List candidate user-visible commits:

```shell
git log --format='%h %s' v<prev>..HEAD
```

Read `CHANGELOG.md` lines 1-30 to see what's already in `[Unreleased]`.

For each commit decide: user-visible?

- **Include**: API additions/changes, behavioral changes, bug fixes plugin
  authors notice, supported-BDS version bumps, deprecations, security fixes.
- **Skip**: refactors, internal class/struct changes, CI/build/toolchain
  changes (unless they force plugin devs to change something), comment
  edits, test-only changes, skill/doc-only changes.

Follow [Keep a Changelog 1.1.0](https://keepachangelog.com/en/1.1.0/):

- Group under `### Added` / `### Changed` / `### Deprecated` / `### Removed`
  / `### Fixed` / `### Security`. Omit empty groups.
- Past tense, one bullet per change, written for plugin authors and server
  admins.
- Prefix breaking changes with `**BREAKING**:` in the Changed/Removed group.
- Python API references use snake_case and property syntax
  (`Player.send_message`, not `Player::sendMessage`).
- Bedrock terminology: "block actor" not "block entity"; mark Java-only
  features as N/A.
- If lowercase `[unreleased]:` ref-link duplicate exists, delete it.

Edit `CHANGELOG.md` to add anything missing. **STOP** and show the user the
final `[Unreleased]` body before stamping - this is the one human approval
point.

---

## Step 3 - stamp `CHANGELOG.md`

Do this with the `Edit` tool.

Get the previous tag and today's date:

```shell
git tag --sort=-version:refname | head -n1     # PREV_TAG, e.g. v0.11.3
date -u +%Y-%m-%d                              # DATE, e.g. 2026-05-25
```

Make exactly two edits to `CHANGELOG.md`:

**(a)** Below the `## [Unreleased]` header, insert a new version stanza,
leaving `[Unreleased]` empty for the next cycle. Move every bullet that
was under `[Unreleased]` into the new `[X.Y.Z]` block.

```diff
 ## [Unreleased]

-### Added
-- Added support for BDS version 1.26.20.
+## [X.Y.Z] - YYYY-MM-DD
+
+### Added
+- Added support for BDS version 1.26.20.
```

**(b)** In the reference-link block at the bottom of the file, update the
`[Unreleased]` link to use the new tag as its base, and insert a new
`[X.Y.Z]` compare link directly below it.

```diff
-[Unreleased]: https://github.com/EndstoneMC/endstone/compare/v<prev>...HEAD
+[Unreleased]: https://github.com/EndstoneMC/endstone/compare/vX.Y.Z...HEAD
+[X.Y.Z]: https://github.com/EndstoneMC/endstone/compare/v<prev>...vX.Y.Z
 [<prev>]: ...
```

Verify the section header `## [X.Y.Z]` is unique and the section is
non-empty - `build.yml`'s release job greps for it on the runner and fails
the publish if missing.

---

## Step 4 - commit, tag, push

Run these sequentially (each depends on the previous):

```shell
git add CHANGELOG.md
git commit -m "Release X.Y.Z"
git tag -a vX.Y.Z -m "Release X.Y.Z"
git push --follow-tags origin $(git rev-parse --abbrev-ref HEAD)
```

`--follow-tags` pushes the commit and the annotated tag in one go. As soon
as it returns, `build.yml` triggers on the tag push.

If the push is rejected (someone else pushed first):

```shell
git pull --rebase origin $(git rev-parse --abbrev-ref HEAD)
git push --follow-tags origin $(git rev-parse --abbrev-ref HEAD)
```

If `git push` succeeds but `git push --follow-tags` did not include the
tag (rare; happens if the tag was pushed in a separate hook), explicitly
`git push origin vX.Y.Z`.

---

## Step 5 - watch the Build run

Tag pushes trigger only `build.yml` (ci.yml's `push.branches` filter
excludes tags, docker.yml is develop-only). Confirm the Build run
started:

```shell
gh run list --workflow=build.yml --limit 3
```

You should see a new `Build` run against `vX.Y.Z` (or the tag's commit
SHA). If it's missing, the trigger didn't fire - investigate before
declaring success.

Watch progress:

```shell
gh run watch <run_id>            # blocks until the run finishes
gh run view <run_id> --json jobs --jq '.jobs[] | {name, conclusion}'
```

For long-running watches, use `run_in_background: true` on the Bash tool
with `gh run watch <run_id>` and check back later instead of polling.

Expected job outcomes on a tag push:

- **wheels** (~15-20 min, parallel matrix): 10 wheels (cp310-cp314 ×
  manylinux_x86_64/win_amd64). Linux wheels build inside the
  `manylinux_2_31` container; deps come from the `Linux-conan-Release-*`
  cache populated by prior develop/main pushes. Artifacts uploaded with
  `archive: false`, so artifact names are the wheel filenames.
- **bundle** (~1 min, needs wheels): produces
  `endstone-X.Y.Z-windows-x86_64.zip` and `endstone-X.Y.Z-linux-x86_64.zip`
  (cp313 wheel + `start.cmd`/`start.sh` + LICENSE + CHANGELOG).
- **docker** (~5 min, needs wheels): builds the short-path image at
  `.github/docker/Dockerfile` from the cp313 manylinux wheel artifact,
  pushes `ghcr.io/endstonemc/endstone:X.Y.Z,:latest` and
  `endstone/endstone:X.Y.Z,:latest`.
- **release** (~3 min, needs wheels + bundle, tag-only): publishes wheels
  to PyPI via OIDC, extracts the `## [X.Y.Z]` CHANGELOG section, creates
  the GH release with body + uploaded bundle archives.

Total wall time ~25 min (bounded by `wheels`).

---

## Step 6 - verify the artifacts

Run in parallel after the Build run succeeds:

```shell
gh release view vX.Y.Z                                                                              # release exists, has body, has bundle zips
pip index versions endstone                                                                         # PyPI lists X.Y.Z
gh api /orgs/EndstoneMC/packages/container/endstone/versions --jq '.[0].metadata.container.tags'    # GHCR has X.Y.Z
curl -s https://hub.docker.com/v2/repositories/endstone/endstone/tags?page_size=5 | jq '.results[].name'   # Docker Hub has X.Y.Z
```

Report the verified artifact list back to the user.

---

## Step 7 - fast-forward `main` to the release commit

Convention in this repo: `main` tracks the latest release branch's HEAD,
so downstream consumers of `main` get the release. The release branch is
normally ahead of `main`; fast-forward `main` to it.

Safety check first:

```shell
git fetch origin main
git rev-list --left-right --count origin/main...origin/<release-branch>
```

Expected output: `0  N` where N >= 1 (release branch is N commits ahead of
main, main has no extra commits). Any other shape means a fast-forward is
not safe:

- `M  N` with M > 0: `main` has its own commits not on the release branch
  -> **STOP**. A fast-forward would lose them. Ask the user how to
  reconcile (typically they want to cherry-pick or merge, not FF).
- `0  0`: already in sync, nothing to do.

If `0  N`, push the release branch's tip to `main` directly (no local
checkout switch needed):

```shell
git push origin origin/<release-branch>:main
```

This is a server-side fast-forward push - rejected by GitHub if it's not
truly a FF, so it's safe to attempt. If you prefer a local merge:

```shell
git checkout main
git pull --ff-only
git merge --ff-only vX.Y.Z
git push origin main
git checkout <release-branch>
```

Release is done.

---

## Recovery cookbook

- **release job failed (missing CHANGELOG section).** Push a follow-up
  commit fixing the `## [X.Y.Z]` header in `CHANGELOG.md`, then rerun
  the `release` job from the Actions UI.
- **One wheel matrix job failed.** Rerun just that job from the Actions
  UI. Other jobs' artifacts are kept; the `release` job will pick them up
  on retry. Do NOT re-tag.
- **All wheels built but PyPI publish failed.** Rerun the `release` job;
  wheels are already in artifacts.
- **docker job failed.** Rerun from the Actions UI. The image build is
  independent of `release`, so PyPI publish proceeds regardless.
- **CHANGELOG typo, tag already pushed.** Fix in a follow-up commit on
  the branch, then `gh release edit vX.Y.Z --notes-file <new-body>`.
- **Wrong commit got tagged.** Avoid. If you must: delete the tag
  locally and on origin (`git push --delete origin vX.Y.Z`), yank from
  PyPI, delete the GH release, restart from Step 3.

---

## Pitfalls

- **`GITHUB_TOKEN` pushes don't cascade.** That's why this skill tags
  from the local checkout. Never reintroduce tag-creation inside a
  workflow without a PAT or GitHub App token.
- **Linux wheel needs autotools that actually work.** Some Conan deps
  (libelf, openssl) shell out to `autoreconf`/`m4`. The cached
  `m4/1.4.19` binary has been observed broken in our manylinux image
  (`autom4te: error: need GNU m4 1.4 or later`). Fixed by
  `build = ["missing", "m4/*"]` in `pyproject.toml` under
  `[tool.scikit-build-core-conan]`. If this regresses, re-add the
  pattern.
- **`[Unreleased]` must stay present after stamping.** Don't rename it
  to `[X.Y.Z]` - copy contents into a new `[X.Y.Z]` stanza and leave
  `[Unreleased]` empty. Otherwise the next release loses its scratchpad
  and the `[Unreleased]:` ref link points to a missing anchor.
- **Conan cache poisoning.** Cache writers are split: `build.yml`'s
  `wheels` job saves Release binaries built inside `manylinux_2_31`;
  `ci.yml`'s `coverage` job saves Debug binaries built on the host.
  They live under different `${{ runner.os }}-conan-<type>` keys so they
  don't collide. If a poisoned cache slips through (e.g. glibc symbol
  mismatch like `_dl_find_object` from a host-built cpptrace), purge
  with `gh cache list --json id,key | jq ... | xargs gh cache delete`.
