---
name: python-doc
description: Authoring, editing, and auditing docstrings for Endstone's pybind11 Python bindings under `src/endstone/python/`. Covers Google-style docstring format, pybind11 multi-line docstring patterns (C++ raw string literals), how to mirror the public C++ API docs into Python without re-inventing wording, and the cleanup pipeline for upgrading thin one-line docstrings into rich multi-line ones. Use whenever writing, editing, reviewing, or auditing Python binding docstrings.
---

# Python binding docs (Endstone pybind11 layer)

The Python bindings live in `src/endstone/python/*.cpp`. Each file calls
`pybind11::class_<T>` / `.def()` / `.def_property()` and passes a C++ string
as the last argument — that string becomes the Python `__doc__`. Historically
these were single sentences. This skill covers the upgrade to richer
multi-line Google-style docstrings and the conventions to use going forward.

## Project conventions

- **Style:** Google. Not Sphinx, not Numpy. Sphinx-style `:param:`/`:returns:`
  is **not** used.
- **Multi-line via C++11 raw string literals** with a custom delimiter to
  survive `)` characters in prose:

  ```cpp
  R"doc(
      One-line summary ending in a period.

      Optional longer description spanning paragraphs as needed.

      Args:
          foo: What foo represents and any constraints.
          bar: What bar represents.

      Returns:
          What the caller gets back.

      Raises:
          RuntimeError: When the call fails for reason X.
  )doc"
  ```

  Indent every line **uniformly** (pybind11/Sphinx strip the common leading
  whitespace; mixed indent breaks the strip). Use spaces, never tabs.

- **Authoritative source:** the matching public C++ header in
  `include/endstone/`. The C++ docs have already been cleaned up (see the
  `cpp-docs` skill) — port them; do not reinvent.
- **Translation table:**
  - C++ `@param foo` → Google `Args:` entry `foo:` (note snake_case in
    Python; the binding renames already use snake_case via `py::arg`).
  - C++ `@return` → Google `Returns:`.
  - C++ `@throws Foo` → Google `Raises:` with the Python exception type
    pybind11 maps to (usually `RuntimeError`).
  - C++ `[[nodiscard]] virtual X getY() const = 0` exposed via
    `def_property_readonly("y", &T::getY)` → property docstring describes
    the value; no `Returns:` section needed (it's a property).
  - C++ getter+setter pair exposed via `def_property("name", &T::get,
    &T::set)` → "Gets or sets ..." style.

## Inline code and references

Docstring prose is rendered as **Markdown** (the docs site compiles each
fragment with a Markdown compiler; mkdocstrings does the same). So:

- Wrap code entities in **single backticks**: types (`` `ItemStack` ``),
  methods (`` `get_relative()` ``), attributes/parameters named in prose
  (`` `apply_physics` ``), literal values and ids (`` `minecraft:stone` ``,
  `` `True` `` / `` `None` ``), and dotted paths (`` `endstone.broadcast.user` ``).
  Don't backtick plain English.
- Use **single** backticks, not RST-style double (`` ``x`` ``). Double renders
  the same under CommonMark but isn't idiomatic; when porting a C++ doc that
  used double backticks, collapse to single.
- **No cross-reference autolinks in prose.** Unlike the C++/Doxygen side there
  is no autoref here — `[`Foo`][]` does not resolve, and Sphinx roles
  (`:class:`) are wrong. Type *annotations in the signature* already auto-link
  (`signature_crossrefs`), so prose backticks are for readability, not links.
- Fenced ` ```py ` blocks are for multi-line examples (an `Examples:` section),
  not inline references.

## Authoring rules

### Properties (`def_property*`)

Properties don't get an `Args:` / `Returns:` section — they describe a
value. Keep them as a description, multi-line only if there's genuinely
more to say.

Single-line is fine for trivial getters:
```cpp
.def_property_readonly("x", &Block::getX, "X-coordinate of this block.")
```

Multi-line when the underlying C++ doc has a body paragraph:
```cpp
.def_property_readonly(
    "exp_progress", &Player::getExpProgress,
    R"doc(
        The player's current experience progress towards the next level.

        A value in the range [0.0, 1.0]: 0.0 is "no progress" and 1.0 is
        "next level".
    )doc")
```

### Methods (`def`)

Methods get the full Google-style treatment when the corresponding C++
method has `@param`/`@return` info:

```cpp
.def("teleport", py::overload_cast<const Location &>(&Actor::teleport),
     py::arg("location"),
     R"doc(
         Teleports this actor to the given location.

         Args:
             location: New location to teleport this actor to.

         Returns:
             True if the teleport succeeded, False otherwise.
     )doc")
```

Trivial no-arg methods may stay one-line:
```cpp
.def("remove", &Actor::remove, "Remove this actor from the level.")
```

### Classes (`py::class_`)

Class docstring is the second positional arg of `py::class_<T>(m, "Name",
"...")`:

```cpp
py::class_<Block>(m, "Block",
    R"doc(
        Represents a block.

        This is a live object, and only one Block may exist for any given
        location in a dimension.
    )doc")
```

### Enums

`py::native_enum` / `py::enum_` members: each `.value(NAME, EnumValue,
"docstring")` may carry a short description. Most existing enums skip per-
value docstrings; only add them if the C++ enum has per-member comments
worth porting.

## What NOT to do

- **Don't write Sphinx style.** `:param foo: ...` / `:returns: ...` /
  `:rtype: ...` are wrong here.
- **Don't paraphrase the C++ docs.** Port the wording; only adjust for
  Python casing (snake_case parameter names) and Python idioms (`True` /
  `False` / `None`, `RuntimeError` instead of `std::runtime_error`).
- **Don't add `Args:` to properties** — they're values, not callables.
- **Don't add `Returns:` to `__init__` /`__str__` / `__repr__` / `__hash__`
  / `__eq__`** — Python convention is to skip the return section on these.
- **Don't introduce tabs** inside the raw string. pybind11 explicitly warns
  about this.
- **Don't strip the existing one-liner** if the C++ side has nothing extra
  to add. The goal is to enrich where source material exists, not to bloat
  every line.
- **Don't auto-generate docs from the C++ side** with a script. The
  enrichment requires judgment (which methods deserve full docstrings,
  which stay one-line, how to phrase property bundles like
  "Gets or sets ...") and should be done by reading both sides.

## Cleanup pipeline (upgrading thin docstrings)

When the user asks to "review the python bindings docs", "fix python
docstrings", "upgrade the bindings to multi-line", run this:

### 1. Inventory

`src/endstone/python/` has ~25 .cpp files, one per subsystem (actor.cpp,
block.cpp, event.cpp, ...). Match each to the corresponding
`include/endstone/<subsystem>/` directory. The Python file is the single
source of truth for what is exposed to Python; the C++ headers are the
source of truth for the docstring content.

### 2. Port

Work file-by-file. For each `.def*()` call:

1. Find the matching C++ declaration in `include/endstone/`.
2. **Check whether a docstring is present at all.** If the call has no
   trailing string argument, *add one* — port from the C++ doc. Common
   offenders: `.def_static("get", &T::get, py::arg("name"))`,
   `.def("load", &T::get, py::arg("name"));` (last in chain, no doc),
   plain `.def("method", &T::method)`. Dunder lambdas (`__str__`,
   `__repr__`, `__hash__`, `__eq__`) and internal-only methods prefixed
   `_` are exempt — leave them undocumented.
3. If the C++ doc is just a one-liner, keep the Python one-liner.
4. If the C++ doc has additional paragraphs, `Args`-worthy parameters, or
   a `Returns` worth keeping, convert the Python string to a `R"doc(...)doc"`
   block following the templates above.
5. Use snake_case parameter names (they should already match the existing
   `py::arg("foo")` literals in the call).

### 3. Spawn parallel agents (large files)

For a sweep across many files, spawn parallel `general-purpose` agents,
one per file or per related cluster (e.g. actor.cpp + event.cpp as one,
block.cpp + level.cpp as another). Pass each agent:
- The Python file path.
- The corresponding C++ header directory.
- A pointer to this skill for conventions.

Each agent reads both sides and edits the .cpp in place. Have them report
back the number of `.def*()` calls upgraded vs left as one-liners.

### 3a. Audit for missing docstrings (separate pass, mechanical)

A quick grep finds `.def*()` chain endings that look suspicious — calls
where the *last* visible string is a `py::arg` or a `py::return_value_policy`
rather than a docstring. Not a perfect filter, but it surfaces most gaps:

```bash
# .def... ending in ); without a string before it (rough)
grep -rEn '\.def[^"]*\)(\s*;|\s*$)' src/endstone/python/
```

For each hit, eyeball the call and confirm whether a docstring was
intended. Don't add docstrings to internal `_`-prefixed methods or dunder
lambdas — those are exempt by convention.

### 4. Verify

- **Build the wheel/module.** Either:
  - Incremental: `cmake --build --preset conan-release --target _python`
    inside `build/Release/generators/conanbuild.bat` (Windows) — much
    faster than a full reinstall.
  - Full: `uv pip install --reinstall -U . -C build-dir=./build` from a
    Visual Studio Developer prompt with clang-cl on PATH.
  Either way, raw-string mismatches and stray `"` inside docstrings
  will surface as C++ compile errors.
- **Sync the freshly-built `.pyd` to BOTH locations the Python
  interpreter and griffe may load from:**
  ```bash
  cp build/Release/src/endstone/python/_python.cp313-win_amd64.pyd \
     endstone/_python.cp313-win_amd64.pyd                            # project root
  cp build/Release/src/endstone/python/_python.cp313-win_amd64.pyd \
     .venv/Lib/site-packages/endstone/_python.cp313-win_amd64.pyd     # venv copy
  ```
  **This is the most common gotcha.** Python's `import endstone._python`
  finds the project-root copy (since cwd is in sys.path), so `help()`
  shows the latest docs from there. But `scripts/stubgen.py` invokes
  griffe, which loads via the *site-packages* path and ignores the
  project-root copy — so without the second copy, stubs render with
  stale docstrings. Symptoms: stub shows old wording (missing trailing
  period, missing `Args:` / `Returns:`) even though `help()` is correct.
- **Sanity-check `help()`:** `python -c "from endstone.block import
  Block; help(Block.get_relative)"` should show the formatted
  multi-line docstring with `Args:` / `Returns:` sections (for
  non-overloaded methods; overloads keep raw whitespace — pybind11
  limitation, see below).
- **Sanity-check stubs:** `python scripts/stubgen.py` should regenerate
  the `.pyi` files cleanly. Spot-check a touched method:
  ```bash
  grep -A 12 "def get_block_at" endstone/level/__init__.pyi
  ```
  Should show the full Args/Returns block.

#### Overloaded-method `help()` quirk

For overloaded `.def()` calls (multiple `.def("name", ...)` with same
Python name), pybind11 synthesizes a combined docstring with each
overload's text embedded verbatim. Python's `pydoc.getdoc` /
`inspect.cleandoc` can't normalize the leading whitespace in this
synthesis, so `help(MyClass.overloaded_method)` shows the raw 4-space
indent for body lines.

This is a known pybind11 limitation, **not** a docstring bug. The `.pyi`
stub (the primary consumption path for IDEs / mkdocstrings) renders
each overload independently via griffe and looks correct. Don't try to
"fix" it by going to zero indent — that just makes the source ugly
without changing the REPL outcome.

## Anti-patterns

- **Don't use Sphinx field markers** (`:param:`, `:returns:`, `:type:`,
  `:rtype:`, `:raises:`). The mkdocs setup renders Google style, not
  Sphinx field lists.
- **Don't repeat the property name in the docstring** ("The x property
  returns x" — just say "X-coordinate of this block.").
- **Don't add `Note:` / `Example:` sections without source material.** If
  the C++ side has no `@note` for this method, don't invent one.
- **Don't add Python type hints inside the docstring** — pybind11 already
  emits a signature line from the C++ types; duplicating it is noise.
- **Don't touch `__str__` / `__repr__` / `__hash__` / `__eq__` lambdas**
  unless they already have a docstring worth improving — these are dunder
  plumbing, not API.
