---
name: cpp-docs
description: Authoring, editing, and auditing Doxygen comments in Endstone's public C++ API under `include/endstone/`. Covers project conventions (JAVADOC_AUTOBRIEF, no `@brief`), Bukkit-derived heritage (use Paper Javadocs as the source), Endstone-specific terminology (Dimension/Level, string IDs, smart-pointer wrappers), and the cleanup pipeline for systematic fixes. Use whenever writing, editing, reviewing, or auditing C++ docstrings in `include/endstone/`.
---

# C++ Docs (Endstone public API)

The public C++ API lives in `include/endstone/`. It was ported from Bukkit,
and the Doxygen comments still reflect that ancestry. This skill is the
reference for anything documentation-related in that tree — adding a new
class, editing one method, or doing a sweep across many files.

## Project conventions

- **Block comments** `/** ... */`. Single-line `///` is not used.
- **`JAVADOC_AUTOBRIEF: YES`** is set in `mkdocs.yml` → `mkdoxy.doxy-cfg`.
  The first sentence (terminated by `.`) of each block comment is the brief.
  **Do not write `@brief`** — it's redundant.
- Tags in use: `@param <name>`, `@return`, `@note`, `@see`, `@throws`,
  `@deprecated`. Bukkit-style `<p>` paragraph tags are kept in legacy code
  (Doxygen handles them fine); prefer a blank `*` line for new paragraphs.
- Docs are published via `mkdoxy` (mkdocs plugin for Doxygen). Headers under
  `include/endstone/detail/` are excluded from publication and should be
  skipped.
- `namespace fmt { ... }` formatters at the bottom of headers are utility
  blocks, not API — no docs needed, don't touch in audits.

## Endstone-specific terminology

When the wording smells Bukkit-y, translate to the Endstone equivalent:

| Bukkit (Java)            | Endstone (C++)                                |
| ------------------------ | --------------------------------------------- |
| `Material` enum          | string IDs like `minecraft:stone`             |
| `World`                  | `Dimension` (per-dimension), `Level` (server) |
| `MapCursorCollection`    | `std::vector<MapCursor>`                      |
| `Region`                 | n/a — usually drop                            |
| `null`                   | `nullptr` (pointers), `std::nullopt` / "empty" / "no value" (values) |
| Java method sig in prose | drop the `(Type)` suffix                      |

Some names *are* shared and legitimate in Endstone: `Player`, `Block`,
`BlockState`, `Inventory`, `ItemMeta`, `ItemFactory`, `Scoreboard`,
`Objective`, `Permission`, `Chunk` — those are real Endstone types, not
stale Bukkit references. Only flag when the surrounding wording is
Bukkit-specific.

Pointer/reference idioms in Endstone:
- Raw `T*` for nullable observer pointers.
- `T&` for non-null observers.
- `std::unique_ptr<T>` / `std::shared_ptr<T>` for ownership.
- `Nullable<T>` / `NotNull<T>` smart-pointer wrappers in some APIs (e.g. the
  ban-list family in `include/endstone/ban/`).
- `Result<T>` for fallible returns.

When writing a `@return` line, describe the actual signature, not what Bukkit
returned. A `std::unique_ptr<Objective>` is not "a reference"; a `Permission&`
cannot return nullptr.

## Authoritative source: Paper Javadocs

Endstone's docs descend from Bukkit, and Paper (PaperMC) is the
actively-maintained fork with the cleanest, most current Javadocs. When
wording is ambiguous, copy-pasted, or missing, look up the matching class on
**https://jd.papermc.io/paper/**:

- `org.bukkit.block.Block` ↔ `endstone::Block`
- `org.bukkit.entity.Player` ↔ `endstone::Player`
- `org.bukkit.scoreboard.Objective` ↔ `endstone::Objective`

Method names usually map 1:1 (`getRelative`, `addScoreboardTag`, ...). Adapt
the wording to Bedrock semantics — don't copy blindly. Anything Java-only
(Material lookups, ChunkSnapshot, MetadataValue, ...) should be dropped, not
translated.

## Writing new docs

Skeleton for a new public method:

```cpp
/**
 * One-sentence summary ending in a period.
 *
 * Optional additional detail — context, edge cases, examples.
 *
 * @param foo What foo represents and any constraints.
 * @param bar What bar represents.
 * @return What the caller gets back. Skip on `void`.
 */
[[nodiscard]] virtual ReturnType methodName(int foo, std::string bar) const = 0;
```

Rules:
- No `@brief` — the first sentence is the brief.
- The first sentence **must** end in `.` so autobrief delimits correctly. If
  it doesn't, the whole first paragraph becomes the brief (long, ugly).
- `@param` names must match the actual identifier in the signature.
- `void` setters get no `@return`. Setters that return success flags
  (`bool`, `Result<void>`) get `@return` describing the meaning of the
  value, not a duplicated brief.
- Don't restate the type — `@return The dimension.` beats `@return Dimension
  the dimension this block is in.`
- For an inline implementation in a header, document the declaration above
  the full definition (`{ ... }`), not separately.

Skeleton for a new class:

```cpp
/**
 * One-sentence summary ending in a period.
 *
 * Optional extended description — invariants, lifetime, who owns instances.
 */
class FooBar { ... };
```

## Editing existing docs

When you change a method signature:
- Add/remove/rename `@param` to match.
- If the return type changes to/from `void`, add or drop `@return`.

When you change *behavior*: update the description to match. The first
sentence ("Returns X" / "Sets Y" / "Checks whether ...") must still be
accurate to the postcondition.

When the existing docstring is wrong or unclear: **do not invent**. Read the
implementation in `src/endstone/core/` to find the truth, or look up the
matching method on Paper Javadocs and adapt.

## Auditing / cleanup pipeline

For a sweep across many files (e.g. user says "clean up the docs", "audit
the headers", "fix the docstrings"), run the four phases below.

### 1. Audit (read-only)

Spawn parallel `general-purpose` agents, one per slice, to scan the headers.
Keep slices roughly even (~50 files each):

- **slice A** — `actor/`, `attribute/`, `ban/`, `block/`, `boss/`, `damage/`,
  `effect/`, `enchantments/`, `lang/`, `map/`, `nbt/`, `potion/`, plus any
  top-level files not in D.
- **slice B** — everything under `event/` (recursive).
- **slice C** — `command/`, `form/`, `inventory/`, `level/`, `permissions/`,
  `plugin/`, `scheduler/`, `scoreboard/`, `util/`.
- **slice D** — the large loose top-level headers (`player.h`, `server.h`,
  `offline_player.h`, `logger.h`, `registry.h`, ...). Be exhaustive — these
  tend to have the most issues.

Each agent should report ONLY:
1. Copy-paste errors in description / `@return` / `@param` (e.g. `getY()`
   returning "x-coordinate", a setter's `@return` text copied from the
   getter).
2. Missing `@param` (only when the function already has a docstring).
3. Mismatched `@param` name vs the real parameter identifier.
4. Wrong description content (says "Sets X" on a getter; describes the
   wrong event; refers to a sibling method's behavior).
5. Bukkit/Java artifacts (see the table above): `Material`, `World`,
   `MapCursorCollection`, `org.bukkit.*`, "Bukkit", "Java", bare `null`
   where it should be `nullptr`, Java method signatures in prose.
6. Empty / placeholder briefs (`TODO`, just-the-method-name, ...).

The agent must **not** flag missing `@brief` (it's intentionally absent) and
must **not** fix anything. Output format per finding:

```
<relative-path>:<line>  [category]  <one-line description>
    > <offending text>
```

### 2. Strip stray `@brief` (only if any sneaked in)

`@brief` shouldn't appear in the tree. If a grep finds any (e.g. someone
copy-pasted from upstream Bukkit), strip them mechanically. One regex
covers all forms — body lines, `/**` openers, and the `*@brief` / `@breif`
typo variants:

```python
# /?\*+ matches `*` (body line), `**` (the `**` of `/**`), and `/**` itself.
PATTERN = re.compile(r"(/?\*+)\s*@(?:brief|breif)\s+")
# Replacement: keep group 1 + a single space ->  f"{m.group(1)} "
```

Run as a throwaway script via `uv run --no-project`, walking `rglob("*.h")`
under `include/endstone/`. After running, `grep -rn "@brief\|@breif"
include/endstone/` should return zero.

Cleanup after the strip:
- For briefs that don't end in `.` on line 1, add a trailing period so
  autobrief picks up the right boundary.
- For multi-line briefs without a period on line 1: either merge into one
  line ending in `.`, or split into "short first sentence. <blank *> rest
  of description."

### 3. Apply audit fixes

Work through the punch list. Rules:

- **Don't make stuff up.** Unclear description → fetch the matching Paper
  Javadoc and adapt to Bedrock semantics. `@return` contradicts the
  signature → read the implementation in `src/endstone/core/` to find the
  truth.
- Java `null` on returns: `nullptr` for raw pointers; `std::nullopt` /
  "empty" / "no value" for value types — a `std::string` can't be null.
- Stale Bukkit type references: rewrite to the Endstone equivalent or drop.
- Copy-paste errors: rewrite to match the actual method.
- `@return` text on a `void` setter: convert to a proper `@param` line, or
  drop it. Don't fabricate a return value.
- `@throws` is fine to add when the implementation actually throws — verify
  in `src/endstone/core/` first, don't guess from the signature.
- Param renames at the C++ signature level (not just the `@param` tag) are
  out of scope for a doc cleanup. Flag them and let the user decide; fix
  the `@param` tag to match the existing identifier, not the other way
  around (parameter names are part of the API surface).

After every batch of fixes, re-grep for residuals:

```bash
grep -rn "\bnull\b\|may be null\|return null\b\|, null otherwise" include/endstone/
grep -rn "\bMaterial\b\|MapCursorCollection\|org\.bukkit\|\bWorld\b" include/endstone/
```

Eyeball the matches — some are legitimate (`Non-null identifier`, `ItemMeta`
real type references, `Material*Quartz` color codes, etc.) and some are
residual Java-isms.

### 4. Verify

- `grep -rn "@brief\|@breif" include/endstone/` → no output.
- `grep -rn "\bMaterial\b\|MapCursorCollection\|org\.bukkit"
  include/endstone/` → no output (or only legitimate type references —
  review by eye).
- Spot-check a handful of touched files visually.
- If the docs can be built locally (`mkdocs serve` with `CI=true`), have
  the user eyeball the rendered output for one or two touched classes —
  the brief should be the first sentence with no `@brief` tag visible.

## Anti-patterns

- **Don't write `@brief`.** It's redundant under `JAVADOC_AUTOBRIEF`. New
  code should never reintroduce it.
- **Don't rewrite docs just because they're terse.** Goal is correctness,
  not style. A short, accurate one-liner beats a verbose rewrite with
  invented detail.
- **Don't strip `<p>` paragraph tags from existing code** — harmless and
  removing them is noise. Don't add new `<p>` in new code (use blank `*`
  lines).
- **Don't touch `namespace fmt { ... }` formatters** at the bottom of
  headers. Utility blocks, not API.
- **Don't touch `include/endstone/detail/`** — internal, excluded from docs.
- **Don't add `@return` to void setters** just because the getter has one.
- **Don't fabricate examples or limits.** A concrete `e.g. minecraft:stone`
  is fine when it matches the file's existing example style; pulling
  "Maximum 50 pages with 256 characters per page" out of nothing is not.
- **Don't rename C++ parameter names in signatures** during a doc cleanup
  — fix the `@param` to match the existing identifier.
