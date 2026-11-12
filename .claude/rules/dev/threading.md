---
paths:
  - "src/endstone/**"
---

# Threading

- BDS's server thread is named `MC_SERVER` and runs Endstone's scheduler; Endstone renames the process's main thread `Main`.
- The Endstone API is server-thread-only; never add speculative locks, retries or null guards for callers that break that contract, and land unrelated concurrency fixes separately.
- Throw for server-thread-only mutators with `Preconditions::checkState(getServer().isPrimaryThread(), ...)`; allow any thread only where Paper does (`createBossBar`).
- Route player-facing sends (messages, packets, forms, sounds, titles, maps, particles) through `deferToServerThread`.
- Build packets and validate arguments on the caller's thread and hop only the send.
- Fire synchronous events on the server thread only and asynchronous events off it; `callEvent` logs an error and drops a mismatch.
- Gate the liquid block hooks' events on `!isInstaticking()`.
- Drive the scheduler from the sim tick (hooked `Level::tick`), with `current_tick_` seeded from the persisted level tick.
- Order same-tick scheduler tasks by next run, then creation time, then task id.
- Take the next task off the queue into a local `shared_ptr` before running it; hold no iterator or reference into the queue across `run()`.
- Return `std::vector<NotNull<Task>>` from the scheduler, never raw `Task *`.
- Release a task's callable once the scheduler is done with it: cancelled, one-shot ran, or last async worker finished.
- Never hold the scheduler lock and a task lock together; collect under one lock and act after releasing it.
- Keep event dispatch thread-safe: the handler map under a `shared_mutex`, `HandlerList` owning `NotNull<EventHandler>` and returning snapshots, no lock held while a listener runs.
- Never run anything that takes the GIL while holding a `HandlerList` or handler-map lock; release removed handlers after unlocking and before plugin references.
- Guard a list mutated from several threads with a mutex, copy out under the lock, and call listeners and updates after release (boss bars).
- Take the GIL with `py::gil_scoped_acquire` in any trampoline or C++-originated call into Python that can run off the interpreter thread.
- Keep thread identity in a function-local static, never a namespace-scope `std::thread::id`.
- Call `EndstoneServer::setMainThread` and name the process main thread `Main` in `init()` before `hook::install()`.
- Require the server thread for BDS registry or level access from a hook: check `isPrimaryThread()` and log an error otherwise.
- Install vtable hooks from `ServerInstanceLifecycleListener::onServerThreadStarted`.
- Take over a BDS thread by detouring the entry the constructor passes to `pthread_create` / `_beginthreadex`.
- Never prefetch or write BDS pools or globals in the first seconds of startup; wait for `ServerInstance::initializeServer`.
- Run `PacketSendEvent`, `PacketReceiveEvent`, packet patching and every handler on the main thread; only decrypt, decompress and split may move off it.
- Never wake worker threads from the main thread per packet.
- Assume one shared network worker pool thread and serial decrypt per connection.
- Dispatch `PacketSendEvent` inline at send time, never deferred.
- On every bump re-check the async-pool wake-threshold write in `server_instance.cpp`: the field, both pool constructors storing `0.05F`, and `MinecraftWorkerPool::ASYNC` resolving.
