---
paths:
  - "src/bedrock/**"
  - "src/endstone/runtime/**"
---

# Bedrock ABI

- Do not model per-platform `std` layout differences with `#ifdef` (`std::string` and `std::unordered_map` sizes, `unique_ptr<T, std::function>` pointer order); declare the member as the header does.
- Never transfer a byte delta between platforms; derive Windows and Linux sizes separately.
- Itanium allocates derived members from the base's `dsize`, MSVC from its `sizeof`; print both with `clang++ --target=x86_64-pc-linux-gnu -Xclang -fdump-record-layouts` and the Windows target.
- A packet payload with alignment <= 4 starts inside `Packet`'s tail padding on Itanium, so the Windows size is the Linux size + 8.
- A payload with an 8-aligned first member cannot use the tail padding, so both platforms get the same size.
- A base class that grows shifts every derived member on both platforms.
- Count 2 destructor slots per class on Itanium and 1 on MSVC when comparing vtables across platforms.
- MSVC lays out same-name virtual overloads grouped and in reverse declaration order; declare them in source order and account for it in slot arithmetic.
- Declare every virtual of a class and of its interfaces; an undeclared override shifts Itanium vtable slots.
- A secondary-interface override sits only in the secondary base's vftable on MSVC and also adds a slot to the primary vtable on Itanium; call it through the interface on both.
- A virtual introduced in a secondary base has `this` already adjusted on MSVC, without a thunk, so its body addresses members at base-relative offsets; destructors are introduced in the primary base and do get thunks.
- Itanium passes the derived `this` to a secondary-base virtual and adjusts it with a non-virtual thunk.
- Keep `detail::fp_cast` operands member pointers with zero `this` adjustment.
- Qualify unqualified virtual calls inside a reimplementation.
- An Itanium class with no key function gets its vtable emitted in every TU that uses it; give a reconstructed class an out-of-line destructor so a hook that defines its only virtual does not own the vtable on Linux.
- Keep a detour's signature form identical to the replaced function: MSVC passes `this` in rcx and the large-return buffer in rdx for members and the reverse for free functions.
- Declare a hooked or `BEDROCK_CALL`ed function exactly as the header does; the only divergence is a parameter Linux clang dropped, declared `static` under `#ifdef` per platform (both `static` when both dropped `this`); every argument one register off in a non-virtual means the compiler dropped `this`, so declare it `static` per platform and change the config `name`.
- Never declare an argument-promoted signature (arguments replaced by the values they point to); hook a header-faithful caller instead.
- When a mangled name flips between static and member, disassemble one call site per platform and count argument registers before choosing.
- `class` versus `struct` on a forward declaration or alias is invisible on Itanium but changes the MSVC mangled name.
- Judge by-value parameter triviality on both ABIs: MSVC x64 looks only at the copy constructor, Itanium also at the move constructor and destructor.
- Give a moved-out `brstd::move_only_function` or `copyable_function` returned from a hook a real move constructor; they move through `move_to` and copy through `copy_to`.
- BDS patch releases within a minor line keep the same ABI, so a patch bump needs signature refreshes, not layout re-derivation; a minor bump does not.
- A size delta is a net, so decompose it before reading its sign: `delta_win - delta_linux == 8*N` for N added `std::string`s.
- A payload sub-object and flat members differ only on Linux and only for a payload with an 8-aligned member; MSVC measures the same, so its assert passes on a wrong model.
- A later 8-aligned member absorbs the shift, so asserts pass while leading members sit 4 bytes low and a flat first member reads uninitialised padding.
- A shortfall on one platform is head misalignment, an equal shortfall of 8 on both is a missing payload wrapper at the tail; confirm with one `-fdump-record-layouts` run per target.
- Diff the old and new prologue's argument marshalling to catch an ABI change behind an unchanged mangled name: an sret shifts `this` from `rdi` to `rsi`, an extra xmm spill is a new float parameter, a reference parameter moves register.
