# GitHub

## Commits

- Write subjects as `type(scope): imperative summary`, lower case, no trailing period.
- Use the types `fix`, `feat`, `docs`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`, `style` and `revert`.
- Omit the scope for repo-wide changes.
- Subject a release commit `Release X.Y.Z`, the one subject without a type.
- Use scope `changelog` for `docs(changelog):` edits and `plugin` for `test(plugin):` changes to `tests/plugin`.
- Use `build(deps)` for dependency bumps and `chore(bedrock)` or `chore(python)` for stub and BDS-target regeneration.
- Name a BDS bump commit `feat: add support for BDS X.Y.Z`.
- End the subject of a Paper-parity change with `as in Paper` or `as Paper does`, and of a bedrock fix mirroring vanilla with `as in BDS`.
- Append the PR number to the subject, `(#NNN)`, only for a PR from outside the maintainers.
- Write a prose body for every non-trivial fix: what was wrong and what changed.
- End every fix commit body with a `Paper:` paragraph naming the Paper class and method read, then "matches" or "differs because <Bedrock evidence>", or `Paper: not applicable` for build or performance changes.
- Subject a new Conan recipe `build: add <name> conan recipe`, stage only `recipes/<name>/` and the consuming changes, and write a body naming what it wraps, which platforms passed and the quirks (bundled deps, patches, `cpp_info`).
- Never add a `Co-Authored-By` line for Claude.
- Put `!` after the scope for a breaking change, as in `refactor(inventory)!:`.
- Title a revert `revert(scope): ...` and name the reverted commit hash in the body.

## Branches

- `main` is the latest released tag and a push to it logging "Bypassed rule violations" is expected; releases are cut from the `vX.Y` branch, never from `main`.
- `vX.Y` is the maintenance branch of released minor X.Y, and the newest `vX.Y` is the integration branch for the next minor.
- Never rewrite pushed history.
- Name topic branches `feat/<x>`, `fix/<x>` or `refactor/<x>`.
- Land a fix on the newest branch first, then back-port with `git cherry-pick -x` so the body ends `(cherry picked from commit <hash>)`.
- Back-port only fixes with a visible effect on the release branch.
- Check a back-port's prerequisite commits exist on the release branch, and leave the fix on the newer branch when the port needs new API, an ABI change or a different mechanism.
- After each release merge the older branch forward, `Merge branch 'vX.Y' into vX.Z` with git's default subject, then commit `docs(changelog): drop entries already shipped in X.Y`.

## Release lines

- Add no new API to a released minor line.
- Rename or remove API unreleased on the current line without deprecation.
- Mark `**BREAKING**` only a change plugin authors must act on; header-only refactors that keep plugin source compiling, such as dependency swaps, forward-declaration churn or out-lining, are not.

## CHANGELOG

- Follow [Keep a Changelog 1.1.0](https://keepachangelog.com/en/1.1.0/) and write for server admins and plugin developers.
- Group entries under Added, Changed, Deprecated, Removed, Fixed and Security, latest version first, each with a date and a reference-style link; keep each `## [X.Y.Z]` heading unique and non-empty, since the `release` job takes it as the GitHub release notes.
- Add an entry only when a server admin or plugin developer can observe the change; omit refactors, internal classes, symbol work, `src/bedrock` ports, tooling and tests.
- Write one short sentence per entry stating what changed, not why or how.
- Append `(#NNN)` when a fix closes an issue.
- Record a BDS bump as one `### Added` line and add no Changed or Removed entries for the shims it deleted.
- Fold extras that ride along with a fix into that fix's single line.
- Omit fixes for bugs introduced by unreleased work, and fixes with no visible effect on the release branch.
- Add no caveat that reads as a restriction, such as a supported-client limit or an unenforced transport warning.
- Prefix breaking changes with `**BREAKING**:` in Changed or Removed.
- Write Python API references in snake_case property syntax, `Player.send_message` rather than `Player::sendMessage`, and say "block actor", not "block entity".
- Fix a CHANGELOG typo found after the tag in a commit, then run `gh release edit vX.Y.Z --notes-file <file>`.

## Pull requests

- Keep a pull request to the scope its title names; a behaviour change to an existing event is its own PR.

## Issues

- Reply on an issue only when the user asks, as a few short plain-prose paragraphs for a server admin covering what was traced and compared, the mechanism in the reporter's terms and what happens next, with no function names, addresses, decompiled code, headings, bold, bullets, em-dash pivots or mirrored clauses, and no promised fix or date.
- Ask a reporter whose bug does not reproduce for OS or container, `/plugins` output, the log around one occurrence, and whether plain BDS behaves the same.
- Close an unfixable report (no repro, no Endstone-side cause, old version) only after the user confirms, with a comment saying what was tried and asking for a new issue on the latest release with the crash report, the Endstone and BDS versions and a minimal repro.
