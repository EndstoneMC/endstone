---
paths:
  - "**/*.h"
  - "**/*.cpp"
---

# C++ Style

Formatting, naming and include order come from `.clang-format` and `.clang-tidy`.

## Code shape

- Do not add free helper functions: no anonymous-namespace utilities, `detail::` namespaces or exported parse/format helpers.
- Inline the logic where it is used or put it in the member that owns it; a small local lambda is fine.
- Write the smallest faithful implementation: mirror BDS's structure and add no speculative limits, caches or abstractions.
- Fix the root cause and grep the file and its siblings for the same mistake; never wrap, delay or guard around a symptom, and report an adjacent problem instead of fixing it.
- Do not introduce a named constant for a one-off value; inline the literal.
- Test classes through their real entry points, not through helpers exported for the test.
- Do not join conditions with `&&` in an `#if`; branch on the outer condition first and nest the inner `#if` inside each branch.
- Start every new file you author, in `src/bedrock` too, with the Apache-2.0 header copied from a sibling; vendored third-party files keep their own.
- In `endstone::` code, qualify the bedrock type as `::X` when `endstone::X` exists, such as `::Biome` and `::BlockType`.

## Includes and compile cost

- Include what you use; clang-cl does not pull standard headers in transitively.
- Remove unused includes.
- Forward-declare instead of including when a header uses a type only as a pointer or reference member, a parameter or return type, an alias, a `friend`, or an STL element; include the real header in the `.cpp`.
- Keep the full `#include` when the header inherits from the type, holds it by value, calls its members, takes `sizeof`, or instantiates a template that needs it complete.
- Hold an incomplete type in `std::unique_ptr` or `std::shared_ptr` only when the owner's destructor is declared in the header and defined in the `.cpp`.
- Forward-declare in the type's real namespace, nested inside the file's existing outer namespace block instead of reopening it.
- Declare constructors, destructors and non-trivial virtuals in the header and define or `= default` them in the `.cpp`; only cheap non-virtual getters and setters go inline.
- Do not instantiate `magic_enum::enum_name` on a large enum, `std::regex` or a heavy `std::format` body in a widely included header; declare a function and define it once in a `.cpp`.
- Never construct a `std::regex` in an inline function, least of all a virtual one.
- Measure compile cost with `-ftime-trace`, aggregated by header self-parse time times include count and by template instantiation totals; fix the top offenders and re-measure, trimming a header to cut its self cost and forward-declaring it out of a hub to cut its inclusive cost.
- Keep `include/endstone/` header-only, with forward declarations, few includes and minimal inline bodies.
- Guard the include of RakNet headers (`RakNetTypes.h`, `RakPeerInterface.h`, `SocketIncludes.h`) with `#ifdef _WIN32`, and `#ifndef`-guard `WIN32_LEAN_AND_MEAN` and `NOMINMAX` there; never define them globally.
- Keep includes in `include/endstone/endstone.hpp` alphabetical; insert new entries, never reorder existing ones.
