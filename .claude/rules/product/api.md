---
paths:
  - "include/endstone/**"
  - "src/endstone/python/**"
---

# Public API

## Paper parity

- Model new API on Paper/Bukkit: class, method and event names, shape, semantics and exception type. Deviate only where BDS forces it.
- Fetch Paper's source (`paper-api/src/main/java/...`) for every event or API you add; do not work from memory.
- Read Bukkit's and Spigot's API in Paper's `paper-api`, which carries both, and follow Paper where Spigot differs.
- Match Paper whenever the choice is between Paper's behaviour and Endstone's; when that changes shipped API, add a `**BREAKING**` CHANGELOG line and name the shipped API to the user.
- Judge Paper parity only on API new or changed since the last release (`git diff v<last release>..HEAD -- include/endstone`); older shipped differences are settled design, bugs excepted.
- Verify Bukkit-derived docs and names against the Paper Javadoc (https://jd.papermc.io/paper/) in the current version, adapted to Bedrock.
- Accepted mappings: `Entity` to `Actor`, `LivingEntity` to `Mob`, `World` to `Dimension`/`Level`, `Material` to ids/types, `PotionEffect` to `Effect`.
- Never use `Entity` in a ported name, and keep Bukkit entity events out of the player package (`ActorEvent`, `getActor()`, `event/actor/`).
- Look for the Bukkit equivalent under another name or package before inventing a name (toggle glide/swim are Actor events, crafting is `CraftItemEvent`).
- Mirror the Bukkit package as the include subfolder (`org.bukkit.block.sign.Side` is `block/sign/side.h`).
- Keep `EffectType` and `EffectId` in `potion/`; do not recreate an `effect/` folder.
- Mirror BDS over a tidier divergent design when Bukkit has no equivalent.
- Omit Bukkit API that Bedrock cannot back (such as a game rule change event).
- Port the upstream base class with the subclass (hierarchy, `Cancellable`, base accessors), the same subclass coverage (`ActorDeathEvent` covers players) and the `Reason`/`Cause` enum.
- Match Paper's accessors and mutability. Keep a Paper getter without a setter read-only and reject writes Bedrock cannot honour with `std::invalid_argument`.
- Give an event with no Bukkit equivalent Bukkit style: actor-vs-actor derives `ActorEvent`, exposes `getActor()` plus a named counterpart, never a vector of two.
- Keep Bedrock ids unchanged when only the constant spelling is aligned with Paper (`NIGHT_VISION` keeps `nightvision`), and name C++ enum values `CamelCase`.

## Types and ownership

- Use `Identifier<T>` (namespaced string, `minecraft:` default) for ids with typed aliases (`ItemTypeId`, `BlockTypeId`, `ActorTypeId`, `DimensionId`, `GameRuleId<T>`); never `NamespacedKey`, raw `std::string` ids or fixed enums for open sets.
- Declare a registry type as `class X : public Registry<X>::Type {}` plus `using XId = Identifier<X>;` (see `block/biome.h`), with equality and hash by id, implicit conversion to its id, and static `get(id)` returning `const T *` (nullptr when unknown). `Server::getRegistry<T>()` throws `std::out_of_range` for a missing registry.
- Make `getTranslationKey()` return the lang key clients resolve and throw when Bedrock has none; never return the id dressed as a key.
- Hand out any handle-managed object as `NotNull<T>` (null impossible) or `Nullable<T>` (null is a real result); never `T&`, `const T&`, `T*`, raw `unique_ptr` or bare `std::shared_ptr`.
- `NotNull` has no run-time check in its constructor; enforce at the Python boundary and at API entry points that take caller pointers.
- Take shared-handle parameters as `const NotNull<T> &`, never `T &` recovered through `shared_from_this()`, and never derive a new public type from `std::enable_shared_from_this`.
- Hold a back-reference to an object its referent can own as `std::weak_ptr<T>` and return `Nullable<T>` from the getter. Hand out permission attachments, permissions and subscribers as shared handles; subscription lists hold subscribers weakly.
- Narrow types with `is<T>()` / `as<T>()` on `Object`, never `asXxx()` virtuals.
- Compare types across DSO boundaries with `ClassInfo`, never `std::type_index` or `type_info` equality.
- Key type-keyed cross-plugin APIs on the `typeid(T).name()` string captured at registration; never `dynamic_cast` or `std::any` across plugin modules. `ServiceManager::load<T>` does not use `dynamic_pointer_cast`, and C++-to-Python service lookups are unsupported.
- Own strings in public value types; never `std::string_view` into caller buffers.
- Use STL-style naming (`snake_case` methods, `lower_case` aliases) only for foundational utility types such as `NotNull`/`Nullable`.
- Use `std::format` in the public API, never fmtlib.
- Append a new virtual at the end of a polymorphic public class, never mid-class.

## Errors

- Do not return `Result<T>` or `expected` from new API.
- Validate arguments with `Preconditions::checkArgument` (`std::invalid_argument`, `ValueError` in Python) wherever Paper throws `IllegalArgumentException`, and state with `checkState` (`std::runtime_error`).
- Validate with BDS's own limits (book title and pages in bytes) and Paper's thrown type.
- Document `@throws std::runtime_error` on server-thread-only mutators such as `Mob::addEffect`.

## Events

- Declare events with `ENDSTONE_EVENT(Name);` and derive cancellable ones from `Cancellable<Base>`. `callEvent` takes `const NotNull<Event> &`.
- Make an event `Cancellable` only when cancelling prevents the action on every path that fires it; otherwise ship it non-cancellable, even if Paper's is cancellable.
- Never corrupt data to force a failure, and never fire a `Cancellable` event and ignore the result.
- Hand out event recipient collections by live reference; do not add `setRecipients`.
- Store event members that outlive the handler as handles (`NotNull<Inventory>`, `NotNull<Block>`, `NotNull<Actor>`), never a `T&` to a stack object. Return a copy of mutable value objects (`Location`) from getters where Paper clones.
- Observe event payload values on either side of the BDS call and read the field BDS itself uses; never derive them from the request.
- Do not fire an event on query calls (gate on `shouldCapture()`), no-op inputs (same slot, empty item, unchanged book), or after an earlier event cancelled the same action.
- Rewrite only the fields you model and pass everything else through (books, maps, server-list).
- Honour vanilla settings and gamerules (show death messages, send command feedback, damage sensor).
- Follow Bukkit's `isValid()` as `isAlive() && valid`; `getHandle()` still reaches a removed actor, and on quit the order is event, disconnect, message.
- Report "not found" as a null or invalid handle where the return type allows; never construct a wrapper over a null handle.
- Remember the client builds an LL(1) table from `AvailableCommandsPacket`, and a development client asserts on any `CommandRegistry::buildPredictTable` collision that the console never shows.
- Register every command `message` parameter as required, and add no overloads whose first tokens accept the same token (string vs player).

## Python surface

- Name Python members `snake_case`, constants and enum values `UPPER_CASE`.
- Expose no-argument getter/setter pairs as properties, keeping the `is_` prefix for booleans; keep `get_`/`set_` methods only where the call takes arguments or acts.
- Group the Python module like the Bukkit package, one level below `endstone`.
