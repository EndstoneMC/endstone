---
paths:
  - "src/bedrock/**/*.cpp"
  - "src/endstone/core/network/**"
  - "src/endstone/runtime/bedrock_hooks/network_system.cpp"
  - "src/endstone/runtime/bedrock_hooks/server_instance.cpp"
---

# Reimplementing BDS Function Bodies

- Reimplement a BDS function only from its decompile, 1:1 in a `.cpp` beside its header: same steps, order, checks and error codes; then delete the symbol entries it replaces on both platforms.
- Cite the function's address in the commit message.
- Match order of operations, integer widths and masks, the branch taken, initialisation and overflow guards.
- Copy an aliased id or key to a local before a loop that frees its storage.
- Match BDS's call path, not just its result: use the real container method (`find()`, not a linear scan) and the real callee, not a near-equivalent.
- Reproduce BDS limits (book limits, item meta) by following the decompiled control flow.
- When BDS returns void, compare state before and after the call to derive the Bukkit-style boolean.
- Reimplemented non-virtual bodies run for real; the virtual overrides stubbed with `unreachable()` are served by the real vtable.
- Re-check lists and tables copied from BDS on every bump; they go stale without a compile error.
