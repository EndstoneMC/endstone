# C++ Header & Compile-Time Hygiene (Endstone)

Adapted from the [Chromium C++ style guide](https://github.com/chromium/chromium/blob/main/styleguide/c++/c++.md)
and `c++-dos-and-donts.md`, tuned to this project. Goal: keep the build fast by
minimizing what each header drags into the ~270 translation units that include it.
This complements the "Code Style" section in `CLAUDE.md` — it does not replace the
clang-format/clang-tidy rules there.

## Prefer forward declarations over `#include`

Prefer a forward declaration to an `#include` whenever the header only needs the
name, not the definition. A type is used *by name only* when it appears as:

- a pointer or reference member;
- a function parameter or return type by value, reference, or pointer;
- a `std::unique_ptr<T>` / `std::shared_ptr<T>` member — **provided the owning
  class's destructor is declared out-of-line** (defined in the `.cpp`). `unique_ptr`
  needs the complete type only where the deleter runs, i.e. in the `.cpp`;
- a `typedef` / `using` alias, or a `friend` declaration;
- the element type of most STL containers (`std::vector<T*>`, etc.).

Move the real `#include` into the `.cpp`. Do **not** turn a by-value member into a
pointer just to be able to forward-declare its type.

You still need the full `#include` (forward declaration is *not* enough) when the
header:

- inherits from the type (complete base required);
- stores it as a by-value member, or otherwise needs its size/layout;
- calls its methods, touches its members, or takes `sizeof`;
- instantiates a template that requires the complete type.

### Match the namespace, and nest rather than reopen

A forward declaration must use the type's real namespace or it silently declares a
different type. When the type shares an outer namespace with the current file, nest
the declaration instead of reopening:

```cpp
namespace endstone {
class PluginLoader;          // fwd-decl shares the endstone block

namespace core {
class EndstonePluginManager : public PluginManager { ... };
}  // namespace core
}  // namespace endstone
```

Prefer this over a separate `namespace endstone { class PluginLoader; }` block
followed by `namespace endstone::core { ... }`.

## Keep implementation out of headers

- Don't define non-trivial methods in the header — an in-class body is an implicit
  inline request that recompiles everywhere.
- **Don't inline constructors or destructors.** They are more expensive than they
  look (member/base construction, exception cleanup). Declare them in the header,
  `= default` them in the `.cpp` when you just need an out-of-line anchor (this is
  also what unlocks `unique_ptr<Incomplete>` members).
- **Don't inline non-trivial virtual functions.** A class whose virtuals are all
  inline has no key function, so its vtable and every inline virtual body are emitted
  in *every* TU that uses the type.
- Only cheap, non-virtual getters/setters belong inline.
- Forward-declare nested classes and define them in the `.cpp`.
- Remove unused includes — there is no automated IWYU here, so check carefully when
  refactoring.

## Never instantiate an expensive template on a fixed type in a widely-included header

Some templates are very costly to instantiate, and a **non-dependent** use of one
inside a header (even inside a `template` member, even inside a non-template inline
member of an explicit specialization) is instantiated at parse time, in every TU
that includes the header. Measured offenders in this codebase:

- `magic_enum::enum_name<E>` on a large enum (~0.4s per include);
- `std::basic_regex<char>` construction / `std::regex` (~3.5s per instantiation);
- heavy `std::format` / `std::vformat` formatter bodies.

Rule: if a header reaches many TUs (anything on the `server.h` / `check.h` /
`callstack.h` include chains), route the expensive call through a function
**declared** in the header and **defined once** in a `.cpp`. The header then only
contains a cheap function call — nothing to instantiate. Example: `LogAreaID`'s
`std::formatter` calls `endstone::detail::log_area_name()` (declared in
`bedrock/diagnostics/log_fmt.h`, defined in `log_fmt.cpp`) instead of calling
`magic_enum::enum_name` inline.

For `std::regex`: don't construct one in an inline (esp. virtual) header function.
Build it in a `.cpp`, or avoid regex entirely for simple suffix/glob matching.

## Project-specific notes

- **`include/endstone/` is the public, header-only plugin API.** You cannot move
  bodies into a `.cpp` here, so the lever is forward declarations + dropping
  unnecessary includes. Keep inline bodies minimal; push heavy machinery behind the
  core layer.
- **`src/bedrock/` is a faithful reconstruction of BDS ABI types.** Don't invent
  helpers in the `Bedrock`/global namespace that don't exist in BDS. When endstone
  needs compile-glue over a BDS type (e.g. a `std::formatter`), keep the helper under
  an obviously-endstone namespace such as `endstone::detail` so it can't be mistaken
  for a real BDS symbol, and define it out-of-line.
- **The STL floor is precompiled** (`src/stl_pch.h`). Standard-library headers are
  effectively free in TUs that use the PCH — spend optimization effort on project and
  `bedrock/` headers, not on `<vector>`/`<format>`.

## Include ordering

Follow the existing order (clang-format enforces grouping): C++ standard headers,
then third-party `<...>` headers, then project `"..."` headers, each group sorted
and separated by a blank line.

## Measure, don't guess

Before optimizing, profile with `-ftime-trace` and aggregate the traces
(ClangBuildAnalyzer-style: rank headers by self-parse-time × include count, and rank
template instantiations by total time). Fix the top offenders, rebuild, re-measure.
A header's *self* cost (its own text + eager instantiations) is what you attack by
trimming/out-lining; its *inclusive* cost falls when you forward-declare it out of a
hub.
