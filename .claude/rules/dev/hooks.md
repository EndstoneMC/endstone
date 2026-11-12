---
paths:
  - "src/endstone/runtime/**"
---

# Runtime Hooks

- Declare every function hook in its `src/bedrock` header with `ENDSTONE_HOOK` and define the detour in `src/endstone/runtime/bedrock_hooks/<header name>.cpp`.
- Name a hook `.cpp` after the `src/bedrock` header whose hooks it implements, never after the feature; a feature that hooks two BDS classes gets two hook files.
- Add each new hook `.cpp` to the explicit source list in `src/endstone/runtime/CMakeLists.txt`.
- Do not write a name-lookup table for detours; the exported detour's mangled name must equal a key in `src/bedrock/symbols/<platform>.h`.
- A detour without a symbol-table entry makes `hook::install()` throw and aborts startup.
- Keep `virtual` on a hooked virtual's detour declaration; without it `hook::install()` throws "Unable to find target function for detour".
- Hook a virtual by symbol through a standalone class that declares the member alone, as the `ScriptPlayerGameplayHandler` hooks do, never a class derived from the BDS type; a derived holder fails at link.
- Check a detour's object with `nm`: it defines exactly the mangled name in `symbols/<platform>.h` as `GLOBAL DEFAULT` with no undefined base symbols.
- Call the original with `ENDSTONE_HOOK_CALL_ORIGINAL(&Class::method, this, args...)` inside the detour.
- Use `ENDSTONE_HOOK_CALL_ORIGINAL_NAME(fp, symbol, ...)` only where `__FUNCDNAME__` is not the hooked function's name, such as inside a lambda.
- Pass the declaring base's pointer-to-member to `ENDSTONE_HOOK_CALL_ORIGINAL` when hooking a virtual inherited from a secondary base.
- Prefer calling a secondary-base virtual normally from a hooked non-virtual member over hooking it.
- Wrap a platform-only hook in `#ifdef _WIN32` / `#elif __linux__`, modelled on `bedrock_hooks/leaves_block.cpp`.
- Choose the hook point from the call graph on both platforms; hook the one function every path funnels through (such as `Player::stopSleepInBed`), not a function one platform inlines and not the client packet behind one path.
- Find a hook site by naming Paper's moment, locating the routine that applies it through a sound, particle, stat, loot, `setBlock` or format-string anchor, and walking `xrefs_to` callers up to the single funnel with validation above and mutation below.
- Confirm non-player paths reach the funnel.
- Prefer a BDS gameplay handler, then a function hook on a single chokepoint, and use an `EndstonePacketHandler::handle` overload only for pure client intent.
- Prefer a `[[signatures]]` entry over `vhook::create<Ordinal>` except for a gameplay-handler virtual, which is always a vtable hook; use a vtable hook elsewhere only when no stable pattern can be cut.
- Hook a gameplay-handler virtual by declaring it `ENDSTONE_VHOOK` in the `Script*Handler` header, installing it with `vhook::create<Ordinal>` in `hookEventHandler` (`server_instance.cpp`), and calling the original with `ENDSTONE_VHOOK_CALL_ORIGINAL`.
- Give every `vhook::create` ordinal a separate `#ifdef _WIN32` / `#else` value.
- Skip a gameplay-handler coordinator whose handler is null; a server with `enable-script=false` never builds it, so reproduce a crash only on that setting with it.
- Delete a hook together with its header marker, its `bedrock_hooks/*.cpp`, its `CMakeLists.txt` entry and its symbol entries in both platform configs when its need goes, then grep the class name.
- Justify a performance hook with a measured server-thread cost and drop it when a cheaper mechanism covers it.
- Justify a new symbol by working this ladder and stopping at the first rung: a BDS gameplay event in `src/bedrock/world/events/*_events.h`, an already-resolved anchor or its caller, a faithful reimplementation of a readable non-virtual body, one wide chokepoint, then the new symbol.
- Add no symbol for a read-only accessor, or for convenience over an existing reconstruction.
- Patch code bytes only through a `BytePatch` entry in `hook::install()` with an expected-byte guard.
- Do not ship verification-only scaffolding inside the runtime.

## Hook context

- Carry hook context (state telling a hook why it was called) in an entt component such as those in `core/entity/components/flag_components.h`, or in the hook file's anonymous namespace.
- Do not add a helper namespace, header or translation unit in `endstone::core` or `endstone::runtime` for hook context.
- Keep file-local state next to its only user; do not share it between hook files.

## Events

- Fire an event from a hook only after `EndstonePluginManager::isEventRegistered` says it has listeners; call the original straight away otherwise.
- Place the `isEventRegistered` guard after any work the hook needs regardless, such as patching an outbound packet or refreshing a cached value.
- To re-enter BDS from a hook without re-firing the event, call `ENDSTONE_HOOK_CALL_ORIGINAL` directly.
- Create events with `std::make_shared` and dispatch them with `server.getPluginManager().callEvent(e)`.
- Fire the event at the point Paper fires it, after BDS validates and before it mutates, so a cancel prevents the effect; an event fired after the mutation makes `Cancellable` and `set*` meaningless.
- Never fire for an action the server then rejects, more than once per action or every tick a held flag stays set, or with pre-action state in the getters.
- Handle cancel and re-read every field a listener can change after the event returns.
- Re-derive the BDS request from the changed event; do not call the original with the pre-event copy.
- Where the BDS event has no cancel channel, own the packet handler and fire before the vanilla call.
- Catch gameplay events in `script_*_gameplay_handler.cpp` and re-fire the `endstone::` event through `callEvent`.
- Move a gameplay cancel to where BDS finishes the whole operation, such as `InventoryTransaction::executeFull`, when sub-action order differs between platforms.
- Wrap an interaction callback and fire the event when it runs, not when BDS only asks for the interact text.

## Packets

- Packet hooks are `EndstonePacketHandler::handle(<Packet> &)` specializations in `bedrock_hooks/packet.cpp`.
- Register each with `EndstonePacketHandlerDispatcher<XPacket>::set(&packet->handler_)` in the `MinecraftPackets::createPacket` hook.
- Call `handle()` to forward to the vanilla dispatcher, or return without calling it to drop the packet.
- Add the real packet type under `src/bedrock/network/packet/` before keying a dispatcher on it; never key on a raw `Packet&`.
- Do not add packet plumbing to `EndstonePlayer`.
- Model a packet handler on Paper's `PlayerConnection`: resolve the sender lazily, fire the events, then forward or drop.
- Prefer splicing an Endstone class into BDS's object chain (`EndstoneBatchedNetworkPeer` in `core/network/`, spliced in `network_system.cpp` on `onNewIncomingConnection`) over hooking methods that run per packet.
