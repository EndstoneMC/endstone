---
paths:
  - "include/endstone/**"
  - "src/endstone/python/**"
---

# Documentation

## Doxygen

- Write docs as `/** ... */` blocks, never `///`.
- End the first sentence with a period, which `JAVADOC_AUTOBRIEF` in `mkdocs.yml` makes the brief; never write `@brief`.
- Use only `@param`, `@return`, `@note`, `@see`, `@throws`, `@deprecated`.
- Separate paragraphs with a blank ` *` line; add no new `<p>` tags.
- Match `@param` names to the signature exactly; fix the tag rather than renaming the parameter.
- Add or drop `@param`/`@return` when a signature changes, and update the description when behaviour changes.
- Put no `@return` on `void` functions; for `bool` setters describe the value's meaning.
- Describe the real signature in `@return` without restating the type (pointer may be null, reference never is).
- Write `nullptr` for null pointers and `std::nullopt` or "empty" for value types; never Java `null`.
- Translate Bukkit terms (`Material` to string ids like `minecraft:stone`, `World` to `Dimension`/`Level`) and drop `org.bukkit.*`, "Bukkit", "Java" and Java-only types.
- Write `MapCursorCollection` as `std::vector<MapCursor>`, drop `Region`, and drop the Java `(Type)` suffix from method references in prose.
- Take wording for ambiguous or missing docs from the matching Paper Javadoc and `src/endstone/core/`; never invent limits or behaviour, and add `@throws` only after confirming it throws.
- Cross-reference with bare type names (no backticks, no `{@link}`), `Class::method()` or `Class#method` for methods, `Class::member_` for members, `@see A::b(), C` for related pointers.
- Write all-lowercase documented names as `@ref`; a bare method name without `()` or scope does not link.
- Use `@link X text @endlink` or `@ref page "text"` for custom link text and convert leftover `{@code x}` to backticks.
- Put literals, ids and keywords in backticks or `@c`; prefix `%` to suppress an unwanted link.
- Document the declaration above an inline definition in a header; do not edit `include/endstone/detail.h` or `namespace fmt { }` formatter blocks for docs.
- Do not rewrite a terse but correct doc for style.

## Docstrings

- Write binding docstrings Google-style (`Args:`, `Returns:`, `Raises:`); no Sphinx fields.
- Write multi-line docstrings as `R"doc(...)doc"` with spaces only and uniform indentation; check every `R"doc(` has its `)doc"`.
- Port wording from the matching `include/endstone/` doc, adjusting only for `snake_case` names and Python idioms.
- Map `@param` to `Args:`, `@return` to `Returns:`, `@throws` to `Raises:` with the exception the binding raises.
- Describe properties as a value, "Gets or sets ..." when read/write; no `Args:` or `Returns:`.
- Give every other `.def*()` and `py::class_` a docstring; the class doc is the third argument after the name.
- Leave the raw indent in overloaded `.def()` docstrings; `help()` shows it and the `.pyi` stub is correct.
- Add an enum `.value(NAME, X, "doc")` docstring only when the C++ enum has a per-member comment.
- Leave `__init__`, `__str__`, `__repr__`, `__hash__`, `__eq__` lambdas and `_`-prefixed internals without `Returns:` or docs.
- Write prose as Markdown with single backticks around types, methods, parameters, ids, `True`, `None` and dotted paths; no cross-reference links or Sphinx roles.
- Never repeat the property name in its docstring, add `Note:` or `Example:` without a C++ counterpart, or put type hints in docstrings.
- Write docstrings by reading both the C++ and Python sides, never with a script.
