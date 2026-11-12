---
paths:
  - "src/bedrock/**"
---

# Reconstructing bedrock types

## Layout and shape

- Mirror BDS's folder layout under `src/bedrock`, one snake_case header per type, `#pragma once`, includes rooted at `bedrock/...`.
- Match the reference declaration exactly: `class` vs `struct`, return and parameter types (reference vs pointer), const-qualification, member type and order, virtual count and order, template form and default arguments, enum underlying type.
- Name by Endstone convention only: members snake_case with a trailing `_` when private or protected, methods camelBack as BDS spells them. Keep BDS's function names and enumerator names.
- Keep enumerators in BDS's order; insert a new one at its position and shift the rest, never append; additive enumerators past Endstone's subset need no edit.
- Port `unsigned long` as `std::uint64_t`. Use BDS vocabulary types such as `brstd::function_ref`, not `std::function`.
- Model a reference member as `T &` with a user-declared, undefined constructor. Never substitute `T *`. A class with a `const bool` member declares its constructor too, or `-Wuninitialized` breaks the build.
- Model real pointee types (`std::unique_ptr<Bar>`, `Bar *`, function pointers) and forward-declare them. Never use `void *` for a member whose type is known.
- Define a pointee Endstone never dereferences as an empty `class Foo {};` in `forward.h` when `std::unique_ptr<Foo>` needs a complete type.
- Size a bitset or array indexed by an enum from its sentinel, or `magic_enum::enum_count<E>()` when the enum has none. Keep a literal only when no enum indexes it.
- Pin layout with `BEDROCK_STATIC_ASSERT_SIZE` on every class that is not truncated. Use no explicit offsets and no manual padding; use `NO_UNIQUE_ADDRESS` (`bedrock/platform/brstd/associative_adapter.h`) on empty members, never plain `[[no_unique_address]]`.
- A passing `BEDROCK_STATIC_ASSERT_SIZE` does not prove member order or vtable slots: swapped trailing members of different sizes and an invented virtual keep `sizeof`.
- `BEDROCK_STATIC_ASSERT_SIZE` checks only the platform being compiled; confirm layouts against the binary.
- `BEDROCK_STATIC_ASSERT_SIZE` checks only when `NDEBUG` is defined, which the `RelWithDebInfo` build does.
- Re-derive every size assert from the binaries on both platforms; a layout change's Linux asserts stay unverified until CI's Linux job passes. Never edit one to make it pass, and never source it from a passing assert.
- Tail additions to a `// ...` class need no `src/bedrock` edit; a middle addition does.
- When a BDS type alias became a struct, make ours a real struct with the layout, replace every use and delete the alias header.
- Flag a struct only Endstone fills and drains that differs from the upstream type for the maintainer; never regress released behaviour to match the name.
- Grep both `BEDROCK_STATIC_ASSERT_SIZE` and raw `static_assert(sizeof(X) == N)` before adding an assert, and replace the raw form with the macro when touching it.
- A class without a `// ...` marker is not necessarily complete: derive its real size, then finish the declaration or add the marker, never a silent prefix plus an assert.
- A class with no declared data members has no offset to get wrong, so a `sizeof` change on it needs no edit.
- A size delta confined to the tail is benign only after checking container stride, variant storage (the largest alternative, by-value `ItemInstance` and `ItemStack` members included) and base subobjects; it is free only when the owner holds the type solely by pointer.
- Omit what Endstone does not declare or use (nested helpers, statics, uncalled methods, trailing members) and never alter what is kept. A member Endstone reads needs every offset before it correct.
- Do not edit a base-class signature that another Endstone class overrides to fix drift; report it.
- Watch the knock-on edit: a member rename breaks call sites outside `src/bedrock`, an added pure virtual makes derived classes abstract, a new const or reference member deletes the default constructor, a pImpl conversion breaks every by-value embedder unless its special members are declared and defined in the sibling `.cpp`.

## Functions and bodies

- Declare, never define, a member function Endstone does not call. Never invent a body, hash or constructor, and leave an unreadable constant undefined.
- Call BDS functions Endstone does not reconstruct through `BEDROCK_CALL`, `BEDROCK_CTOR` or `BEDROCK_VAR` from `bedrock/symbol.h`; a symbol missing from the platform table is a compile error.
- Write the declaration from the reference header first, then update the symbol config `name` to its mangling. Never derive a signature from the PDB's public name.
- Declare a virtual Endstone never defines as pure (`= 0`) even where BDS's is not pure. One such line is one slot whatever its signature.
- Cross-check virtual order, not line order: a wrongly named virtual at the right slot links and corrupts.
- Model a class that became a handle (body behind a pointer) as a `Body` or `Impl` struct plus one pointer, keeping the accessor signatures.
- A handle shows as one extra `mov rax, [rax]` in a BDS function that reads through the object, so diff the chain of indirections rather than the endpoints.
- A pure virtual destructor never makes an embedder abstract, any other pure virtual needs an explicit override, and a declared-but-undefined virtual links only if no TU emits the vtable, which `llvm-nm` on existing `.obj` files shows.

## Packets and events

- Declare a reconstructed packet with its payload members and a size assert. Override `getId`, `getName`, `write` or `_read` only when Endstone constructs and sends that packet by value; `emote_packet.h` is the model.
- Model a cereal payload with an 8-aligned member as a payload sub-object, never an `#ifdef __linux__` filler member.
- Take a packet's `SerializationMode` initial value and a wire enum's underlying width from BDS and protocol-docs. Copy wire names verbatim, except a cereal name-coded enum, whose wire name is the C++ member name (`dev/network.md`).
- MSVC `std::variant` storage is the largest alternative's size with `_Which` right after it, so a changed alternative size moves `_Which` and an inserted or reordered alternative changes the index set.
- When one packet shows the `<Name>Payload` split, assume every `network/packet/*` has it; `inventory_slot_packet.h` is the reference.
- Mirror an event variant's full alternative list in count and order. Only the alternative that drives the size needs a byte-exact body.

## Placeholders and markers

- Name a guessed parameter positionally (`a1`, `a2`) with no marker.
- Never promote an inferred shape or name to a header as if read from a symbol. Never invent a Mojang name; a shape proves slot and return kind, never identity.
- Replace a placeholder with the real type once the reference has it, and delete its `TODO(fixme)` and evidence comment.
- Keep a byte stand-in (`std::byte x[N]`, with `alignas` when needed) when only one platform's size can be proven.
- Truncate only types BDS allocates and destroys itself, such as packets from `createPacket`.
- Fence multi-line additions with `// Endstone begins` and `// Endstone ends`.
- Put invented plumbing such as a `thread_local` scope in `src/endstone/runtime/bedrock_hooks/`, never unmarked inside a bedrock namespace in `src/bedrock`.
- Keep a hand-written helper on a reconstructed struct, with its `// Endstone` marker, when sweeping a type against the reference.

## forward.h and includes

- Add every forward declaration to `src/bedrock/forward.h` in its group (the `endstone` block, `// Classes`, `// Enums`, `// Nested Classes and Namespaces`, `// Template Classes`), alphabetical within the group.
- Match `class` vs `struct` on every forward declaration and alias to BDS's original.
- Prune a forward declaration only when the type is gone from the reference and unused in `src/bedrock` and `src/endstone`; if it is still used, fix the usage.
- Verify a header is self-contained with a one-line TU that includes it, compiled with clang-cl `/Zs` and the flags from a `FAILED:` build-log line, never the repo-root `compile_commands.json`.
