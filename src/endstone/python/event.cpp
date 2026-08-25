// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "event.h"

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

void init_event(py::module_ &m, py::class_<Event, PyEvent> &event)
{
    py::native_enum<EventResult>(
        m, "EventResult", "enum.Enum",
        "Represents the result a plugin can apply to an event whose default behaviour can be allowed, denied, or "
        "left to the server.")
        .value("DENY", EventResult::Deny)
        .value("DEFAULT", EventResult::Default)
        .value("ALLOW", EventResult::Allow)
        .finalize();

    event.def(py::init<bool>(), py::arg("is_async") = false)
        .def_property_readonly("event_name", &Event::getEventName, "A user-friendly identifier for this event.")
        .def_property_readonly("is_asynchronous", &Event::isAsynchronous,
                               "`False` by default, `True` if the event fires asynchronously.");

    py::class_<ICancellable>(m, "Cancellable",
                             "A type characterizing events that may be cancelled by a plugin or the server.")
        .def_property("is_cancelled", &ICancellable::isCancelled, &ICancellable::setCancelled, R"doc(
    The cancellation state of this event.

    A cancelled event will not be executed in the server, but will still pass to other plugins.
)doc")
        .def("cancel", &ICancellable::cancel, R"doc(
    Cancel this event.

    A cancelled event will not be executed in the server, but will still pass to other plugins.
)doc");

    // Actor events
    py::class_<ActorEvent<Actor>, Event>(m, "ActorEvent", "Represents an `Actor`-related event.")
        .def_property_readonly("actor", &ActorEvent<Actor>::getActor,
                               "The `Actor` which is involved in this event.");
    py::class_<ActorEvent<Mob>, Event>(m, "MobEvent", "Represents an `Mob`-related event.")
        .def_property_readonly("actor", &ActorEvent<Mob>::getActor,
                               "The `Mob` which is involved in this event.");
    py::class_<ActorCollideWithActorEvent, Event, ICancellable>(m, "ActorCollideWithActorEvent", R"doc(
    Called when two actors collide with each other.

    If this event is cancelled, the actors will not be pushed away from each other. Cancelling also stops
    either actor from being pulled onto the other when the other is a rideable vehicle, so a listener that
    cancels every collision also stops boats and minecarts from being boarded by walking into them.

    The server fires this before it decides whether the collision leads to a push, so it is also called for
    pairs the server then leaves alone, and it is called more than once per tick for a pair that keeps
    overlapping.
)doc")
        .def_property_readonly("actors", &ActorCollideWithActorEvent::getActors,
                               "The actors that are involved in this event.");
    py::class_<ActorDamageEvent, ActorEvent<Mob>, ICancellable>(m, "ActorDamageEvent",
                                                                "Called when an `Actor` is damaged.")
        .def_property("damage", &ActorDamageEvent::getDamage, &ActorDamageEvent::setDamage,
                      "The raw amount of damage caused by the event.")
        .def_property_readonly("damage_source", &ActorDamageEvent::getDamageSource,
                               "A `DamageSource` detailing the source of the damage.");
    py::class_<ActorDeathEvent, ActorEvent<Mob>>(m, "ActorDeathEvent", "Called when an `Actor` dies.")
        .def_property_readonly("damage_source", &ActorDeathEvent::getDamageSource,
                               "A `DamageSource` detailing the source of the damage for the death.");
    py::class_<PlayerDeathEvent, ActorDeathEvent>(m, "PlayerDeathEvent", "Called when a `Player` dies.")
        .def_property_readonly("player", &PlayerDeathEvent::getPlayer,
                               "The `Player` which is involved in this event.")
        .def_property("death_message", &PlayerDeathEvent::getDeathMessage, &PlayerDeathEvent::setDeathMessage,
                      "The death message that will appear to everyone on the server.");
    py::class_<ActorExplodeEvent, ActorEvent<Actor>, ICancellable>(m, "ActorExplodeEvent",
                                                                   "Called when an actor explodes.")
        .def_property_readonly("location", &ActorExplodeEvent::getLocation, R"doc(
    The location where the explosion happened.

    It is not possible to get this value from the `Entity` as the `Entity` no longer exists in the
    world.
)doc")
        .def_property("block_list", py::overload_cast<>(&ActorExplodeEvent::getBlockList),
                      &ActorExplodeEvent::setBlockList, py::return_value_policy::reference_internal,
                      "The list of blocks that would have been removed or were removed from the explosion event.");
    auto actor_effect_event = py::class_<ActorEffectEvent, ActorEvent<Mob>, ICancellable>(m, "ActorEffectEvent", R"doc(
    Called when an effect on a `Mob` changes.

    This is fired before the change is applied. Cancelling the event prevents it, and the effect may be
    replaced with a different one by assigning to `effect`.
)doc");
    py::native_enum<ActorEffectEvent::Action>(actor_effect_event, "Action", "enum.Enum",
                                              "An enum to specify how the effect changed.")
        .value("ADDED", ActorEffectEvent::Action::Added)
        .export_values()
        .finalize();
    actor_effect_event.def_property_readonly("action", &ActorEffectEvent::getAction, "How the effect changed.")
        .def_property("effect", &ActorEffectEvent::getEffect, &ActorEffectEvent::setEffect,
                      "The effect involved in this event.");
    py::class_<ActorDismountEvent, ActorEvent<Actor>, ICancellable>(
        m, "ActorDismountEvent", "Called when an `Actor` stops riding another `Actor`.")
        .def_property_readonly("vehicle", &ActorDismountEvent::getVehicle, "The actor that is being dismounted.");
    py::class_<ActorChangeBlockEvent, ActorEvent<Actor>, ICancellable>(m, "ActorChangeBlockEvent", R"doc(
    Called when an `Actor` changes a block through its own behaviour, such as a creeper exploding, an
    enderman picking a block up, a ravager trampling crops or a zombie breaking a door.

    Unlike Bukkit's equivalent, this covers only the mob griefing paths. It is not called for falling
    blocks landing or for sheep eating grass, and the resulting block state is not available.
)doc")
        .def_property_readonly("block", &ActorChangeBlockEvent::getBlock,
                               "The block that will be changed.");
    py::class_<ActorKnockbackEvent, ActorEvent<Mob>, ICancellable>(m, "ActorKnockbackEvent",
                                                                   "Called when a living entity receives knockback.")
        .def_property_readonly("source", &ActorKnockbackEvent::getSource,
                               "The source actor that has caused knockback to the defender, or `None` if the "
                               "knockback is not caused by an actor.")
        .def_property("knockback", &ActorKnockbackEvent::getKnockback, &ActorKnockbackEvent::setKnockback, R"doc(
    The knockback that will be applied to the entity.

    Note: the getter returns a copy; changes must be applied via the setter.
)doc");
    py::class_<ActorPickupItemEvent, ActorEvent<Actor>, ICancellable>(m, "ActorPickupItemEvent", R"doc(
    Called when an `Actor` picks an item up from the ground.

    This is not called for players; see `PlayerPickupItemEvent` instead.
)doc")
        .def_property_readonly("item", &ActorPickupItemEvent::getItem, "The Item picked up by the actor.")
        .def_property_readonly("amount", &ActorPickupItemEvent::getAmount,
                               "The number of items that will be picked up from the stack.");
    py::class_<ActorRemoveEvent, ActorEvent<Actor>>(m, "ActorRemoveEvent", R"doc(
    Called when an `Actor` is removed.

    This event should only be used for monitoring. Modifying the actor during or after this event
    leads to undefined behaviours. This event will not be called for `Player`s.
)doc");
    py::class_<ActorSpawnEvent, ActorEvent<Actor>, ICancellable>(m, "ActorSpawnEvent", R"doc(
    Called when an `Actor` is spawned into a world.

    If an Actor Spawn event is cancelled, the actor will not spawn.
)doc");
    py::class_<ActorTeleportEvent, ActorEvent<Actor>, ICancellable>(m, "ActorTeleportEvent", R"doc(
    Called when a non-player entity is teleported from one location to another.

    This may be as a result of natural causes (Enderman, Shulker), pathfinding (Wolf), or commands
    (/teleport).
)doc")
        .def_property("from_location", &ActorTeleportEvent::getFrom, &ActorTeleportEvent::setFrom,
                      "The location that this actor moved from.")
        .def_property("to_location", &ActorTeleportEvent::getTo, &ActorTeleportEvent::setTo,
                      "The location that this actor moved to.");
    py::class_<ActorToggleGlideEvent, ActorEvent<Mob>>(
        m, "ActorToggleGlideEvent", "Called when an `Actor`'s gliding state is toggled with an elytra.")
        .def_property_readonly("is_gliding", &ActorToggleGlideEvent::isGliding,
                               "Whether the actor is now gliding or not.");
    py::class_<ActorToggleSwimEvent, ActorEvent<Mob>>(m, "ActorToggleSwimEvent",
                                                      "Called when an `Actor`'s swimming state is toggled.")
        .def_property_readonly("is_swimming", &ActorToggleSwimEvent::isSwimming,
                               "Whether the actor is now swimming or not.");

    // Block events
    py::class_<BlockEvent, Event>(m, "BlockEvent", "Represents an `Block`-related event.")
        .def_property_readonly("block", &BlockEvent::getBlock,
                               "The `Block` which is involved in this event.");
    py::class_<BlockBreakEvent, BlockEvent, ICancellable>(m, "BlockBreakEvent", R"doc(
    Called when a block is broken by a player.

    If a `BlockBreakEvent` is cancelled, the block will not break and experience will not drop.
)doc")
        .def_property_readonly("player", &BlockBreakEvent::getPlayer,
                               "The `Player` that is breaking the block involved in this event.");
    py::class_<BlockExplodeEvent, BlockEvent, ICancellable>(m, "BlockExplodeEvent", R"doc(
    Called when a block explodes (e.g. bed in the Nether, respawn anchor in the Overworld).

    If a `BlockExplodeEvent` is cancelled, the explosion will not occur.
)doc")
        .def_property("block_list", py::overload_cast<>(&BlockExplodeEvent::getBlockList),
                      &BlockExplodeEvent::setBlockList, py::return_value_policy::reference_internal,
                      "The list of blocks that would have been removed or were removed from the explosion event.");
    py::class_<BlockCookEvent, BlockEvent, ICancellable>(m, "BlockCookEvent",
                                                         "Called when an `ItemStack` is successfully cooked in a block.")
        .def_property_readonly("source", &BlockCookEvent::getSource, py::return_value_policy::reference,
                               "The smelted (source) `ItemStack` for this event.")
        .def_property("result", &BlockCookEvent::getResult, &BlockCookEvent::setResult,
                      "The resultant `ItemStack` for this event.");
    py::class_<BlockGrowEvent, BlockEvent, ICancellable>(m, "BlockGrowEvent", R"doc(
    Called when a block grows naturally in the world.

    If a Block Grow event is cancelled, the block will not grow.
)doc")
        .def_property_readonly("new_state", &BlockGrowEvent::getNewState,
                               "The new state of the block after it has grown.");
    py::class_<BlockFormEvent, BlockGrowEvent>(m, "BlockFormEvent", R"doc(
    Called when a block is formed or spreads based on world conditions.

    Use `BlockSpreadEvent` to catch blocks that actually spread and don't just "randomly" form.

    Examples:
        - Snow forming due to a snow storm.
        - Ice forming in a snowy Biome like Taiga or Tundra.
        - Obsidian / Cobblestone forming due to contact with water.
        - Concrete forming due to mixing of concrete powder and water.

    If a Block Form event is cancelled, the block will not be formed.
)doc");
    py::class_<BlockFromToEvent, BlockEvent, ICancellable>(m, "BlockFromToEvent", R"doc(
    Represents events with a source block and a destination block, currently only applies to liquid
    (lava and water) and teleporting dragon eggs.

    If a Block From To event is cancelled, the block will not move (the liquid will not flow).
)doc")
        .def_property_readonly("to_block", &BlockFromToEvent::getToBlock, py::return_value_policy::reference,
                               "The faced `Block`.");
    py::class_<BlockPistonEvent, BlockEvent, ICancellable>(m, "BlockPistonEvent",
                                                           "Called when a piston block is triggered.")
        .def_property_readonly("direction", &BlockPistonEvent::getDirection,
                               "The direction in which the piston will operate.");
    py::class_<BlockPistonExtendEvent, BlockPistonEvent>(m, "BlockPistonExtendEvent", "Called when a piston extends.");
    py::class_<BlockPistonRetractEvent, BlockPistonEvent>(m, "BlockPistonRetractEvent",
                                                          "Called when a piston retracts.");
    py::class_<BlockPlaceEvent, BlockEvent, ICancellable>(m, "BlockPlaceEvent", R"doc(
    Called when a block is placed by a player.

    If a `BlockPlaceEvent` is cancelled, the block will not be placed.
)doc")
        .def_property_readonly("player", &BlockPlaceEvent::getPlayer,
                               "The `Player` who placed the block involved in this event.")
        .def_property_readonly("block_placed", &BlockPlaceEvent::getBlockPlaced,
                               "The `Block` that was placed.")
        .def_property_readonly("block_replaced_state", &BlockPlaceEvent::getBlockReplacedState,
                               "The `BlockState` of the block that was replaced.")
        .def_property_readonly("block_against", &BlockPlaceEvent::getBlockAgainst,
                               "The block that the new block was placed against.");
    py::class_<LeavesDecayEvent, BlockEvent, ICancellable>(m, "LeavesDecayEvent", R"doc(
    Called when leaves are decaying naturally.

    If a Leaves Decay event is cancelled, the leaves will not decay.
)doc");

    // Level events
    py::class_<LevelEvent, Event>(m, "LevelEvent", "Represents events within a level.")
        .def_property_readonly("level", &LevelEvent::getLevel, py::return_value_policy::reference,
                               "The `Level` primarily involved with this event.");
    py::class_<DimensionEvent, LevelEvent>(m, "DimensionEvent", "Represents events within a dimension.")
        .def_property_readonly("dimension", &DimensionEvent::getDimension,
                               "The `Dimension` primarily involved with this event.");
    py::class_<DimensionLoadEvent, DimensionEvent>(m, "DimensionLoadEvent", "Called when a dimension is loaded.");

    // Chunk events
    py::class_<ChunkEvent, DimensionEvent>(m, "ChunkEvent", "Represents a `Chunk` related event.")
        .def_property_readonly("chunk", &ChunkEvent::getChunk, py::return_value_policy::reference,
                               "The `Chunk` being loaded/unloaded.");
    py::class_<ChunkLoadEvent, ChunkEvent>(m, "ChunkLoadEvent", "Called when a chunk is loaded.");
    py::class_<ChunkUnloadEvent, ChunkEvent>(m, "ChunkUnloadEvent", "Called when a chunk is unloaded.");

    py::class_<UnknownCommandEvent, Event>(m, "UnknownCommandEvent",
                                           "Called when a command sender executes a command that is not defined.")
        .def_property_readonly("sender", &UnknownCommandEvent::getSender, "The command sender.")
        .def_property_readonly("command_line", &UnknownCommandEvent::getCommandLine, "The command that was sent.")
        .def_property("message", &UnknownCommandEvent::getMessage, &UnknownCommandEvent::setMessage,
                      "The message that will be returned, or `None` if no message will be sent.");

    // Player events
    py::class_<PlayerEvent, Event>(m, "PlayerEvent", "Represents a player related event.")
        .def_property_readonly("player", &PlayerEvent::getPlayer,
                               "The `Player` who is involved in this event.");
    py::class_<PlayerArmSwingEvent, PlayerEvent, ICancellable>(m, "PlayerArmSwingEvent", R"doc(
    Called when a player swings their arm.

    Cancelling stops the server acting on the swing at all. The swing is neither recorded nor shown to the other
    players in the dimension.

    Note:
        The swinging player still sees their own arm move, because their client plays the animation without waiting
        for the server. The event covers swings the player starts. Swings the server drives itself, such as dropping
        an item, do not fire it.
)doc")
        .def_property_readonly("item", &PlayerArmSwingEvent::getItem,
                               "The item the player was holding when they swung their arm.");
    auto player_bed_enter_event = py::class_<PlayerBedEnterEvent, PlayerEvent, ICancellable>(
        m, "PlayerBedEnterEvent", "Called when a player is almost about to enter the bed.");
    player_bed_enter_event.def_property_readonly("bed", &PlayerBedEnterEvent::getBed,
                                                 py::return_value_policy::reference,
                                                 "The bed block involved in this event.");

    py::class_<PlayerBedLeaveEvent, PlayerEvent>(m, "PlayerBedLeaveEvent", "Called when a player is leaving a bed.")
        .def_property_readonly("bed", &PlayerBedLeaveEvent::getBed, py::return_value_policy::reference,
                               "The bed block involved in this event.");
    py::class_<PlayerBucketActorEvent, PlayerEvent, ICancellable>(m, "PlayerBucketActorEvent",
                                                                  "Called when a player captures an actor in a bucket.")
        .def_property_readonly("actor", &PlayerBucketActorEvent::getActor, "The actor being captured.")
        .def_property_readonly("original_bucket", &PlayerBucketActorEvent::getOriginalBucket,
                               "The bucket used to capture the actor.")
        .def_property_readonly("hand", &PlayerBucketActorEvent::getHand, "The hand used to capture the actor.");
    py::class_<PlayerChatEvent, PlayerEvent, ICancellable>(m, "PlayerChatEvent",
                                                           "Called when a player sends a chat message.")
        .def_property("message", &PlayerChatEvent::getMessage, &PlayerChatEvent::setMessage,
                      "The message that the player is attempting to send.")
        .def_property("player", &PlayerChatEvent::getPlayer, &PlayerChatEvent::setPlayer,
                      "The player that this message will be displayed as being sent by.")
        .def_property("format", &PlayerChatEvent::getFormat, &PlayerChatEvent::setFormat, R"doc(
    The format to use to display this chat message.

    See the format string syntax at https://en.cppreference.com/w/cpp/utility/format/spec.html.
)doc")
        .def_property_readonly("recipients", &PlayerChatEvent::getRecipients,
                               "The set of `Player`s who will see this chat message.");
    py::class_<PlayerCommandEvent, PlayerEvent, ICancellable>(m, "PlayerCommandEvent",
                                                              "Called whenever a player runs a command.")
        .def_property("command", &PlayerCommandEvent::getCommand, &PlayerCommandEvent::setCommand,
                      "The command that the player is attempting to send.");
    py::class_<PlayerDimensionChangeEvent, PlayerEvent>(m, "PlayerDimensionChangeEvent",
                                                        "Called when a player switches to another dimension.")
        .def_property_readonly("from_dimension", &PlayerDimensionChangeEvent::getFrom,
                               "The player's previous dimension.")
        .def_property_readonly("to_dimension", &PlayerDimensionChangeEvent::getTo, "The player's new dimension.");
    py::class_<PlayerCraftItemEvent, PlayerEvent, ICancellable>(m, "PlayerCraftItemEvent", R"doc(
    Called when a player crafts an item, either inside a crafting grid or straight from the recipe book.

    If the event is cancelled the item will not be crafted and the ingredients will not be consumed.
)doc")
        .def_property_readonly(
            "recipe", [](const PlayerCraftItemEvent &self) { return wrap_recipe(self.getRecipe()); },
            "A copy of the current recipe on the crafting matrix.")
        .def_property_readonly("ingredients", &PlayerCraftItemEvent::getIngredients, R"doc(
    The ingredients a single craft consumes.

    These are the items in the crafting grid where the player used one. Crafting from the recipe book never fills
    the grid, so the ingredients then come from the recipe instead, and an ingredient that accepts several items
    reports the one the recipe names rather than the one the player supplied.
)doc")
        .def_property("results", &PlayerCraftItemEvent::getResults, &PlayerCraftItemEvent::setResults, R"doc(
    The items a single craft produces.

    A recipe usually produces one item, but may produce several, and an ingredient that leaves a remainder behind
    contributes one too. Results are replaced one for one, so any beyond the number the recipe produces are ignored;
    cancel the event to stop the craft instead.
)doc")
        .def_property("repetitions", &PlayerCraftItemEvent::getRepetitions, &PlayerCraftItemEvent::setRepetitions,
                      R"doc(
    The number of times the recipe is being crafted.

    This is usually 1, but is higher when a batch is crafted at once, such as a shift click in the recipe book.
)doc");
    py::class_<PlayerDropItemEvent, PlayerEvent, ICancellable>(
        m, "PlayerDropItemEvent", "Called when a player drops an item from their inventory.")
        .def_property_readonly("item", &PlayerDropItemEvent::getItem, py::return_value_policy::reference,
                               "The `ItemStack` dropped by the player.");
    py::class_<PlayerEditBookEvent, PlayerEvent, ICancellable>(m, "PlayerEditBookEvent",
                                                               "Called when a player edits or signs a book.")
        .def_property_readonly("slot", &PlayerEditBookEvent::getSlot, "The inventory slot containing the book.")
        .def_property_readonly("previous_book_meta", &PlayerEditBookEvent::getPreviousBookMeta, R"doc(
    A copy of the book metadata before the edit.

    Note:
        This is a copy: changes made to it are not written back to the book.
)doc")
        .def_property("new_book_meta", &PlayerEditBookEvent::getNewBookMeta, &PlayerEditBookEvent::setNewBookMeta,
                      R"doc(
    The book metadata that the player is attempting to add to the book.

    Note:
        Reading this gives a copy: assign to it to change what will actually be added to the book.

        A title, an author and a generation are only written when the book is being signed. On a plain edit the book
        stays a book and quill, which holds none of them, and they are dropped. See `is_signing`.
)doc")
        .def_property("is_signing", &PlayerEditBookEvent::isSigning, &PlayerEditBookEvent::setSigning,
                      "Whether the player is signing the book.");
    py::class_<PlayerEmoteEvent, PlayerEvent, ICancellable>(m, "PlayerEmoteEvent",
                                                            "Called when a player uses an emote.")
        .def_property_readonly("emote_id", &PlayerEmoteEvent::getEmoteId, "The emote piece ID.")
        .def_property("is_muted", &PlayerEmoteEvent::isMuted, &PlayerEmoteEvent::setMuted, R"doc(
    The muted state for the emote.

    When `True`, the emote is executed without sending a chat message about the emote.
)doc");
    py::class_<PlayerExpChangeEvent, PlayerEvent>(m, "PlayerExpChangeEvent",
                                                  "Called when a player's experience changes.")
        .def_property("amount", &PlayerExpChangeEvent::getAmount, &PlayerExpChangeEvent::setAmount,
                      "The amount of experience the player will be given.");
    py::class_<PlayerGameModeChangeEvent, PlayerEvent, ICancellable>(
        m, "PlayerGameModeChangeEvent", "Called when the `GameMode` of the player is changed.")
        .def_property_readonly("new_game_mode", &PlayerGameModeChangeEvent::getNewGameMode,
                               "The `GameMode` the player is switched to.");
    py::class_<PlayerInputEvent, PlayerEvent>(m, "PlayerInputEvent",
                                              "Called when a player sends updated input to the server.")
        .def_property_readonly("input", &PlayerInputEvent::getInput,
                               "The new input received from this player.");
    auto player_interact_event = py::class_<PlayerInteractEvent, PlayerEvent, ICancellable>(
        m, "PlayerInteractEvent", "Called when a player interacts with an object or air.");
    py::native_enum<PlayerInteractEvent::Action>(player_interact_event, "Action", "enum.Enum",
                                                 "Represents the type of interaction that triggered this event.")
        .value("LEFT_CLICK_BLOCK", PlayerInteractEvent::Action::LeftClickBlock)
        .value("RIGHT_CLICK_BLOCK", PlayerInteractEvent::Action::RightClickBlock)
        .value("LEFT_CLICK_AIR", PlayerInteractEvent::Action::LeftClickAir)
        .value("RIGHT_CLICK_AIR", PlayerInteractEvent::Action::RightClickAir)
        .export_values()
        .finalize();
    player_interact_event
        .def_property_readonly("action", &PlayerInteractEvent::getAction, "The action type of this interaction.")
        .def_property_readonly("has_item", &PlayerInteractEvent::hasItem, "`True` if this event involved an item.")
        .def_property_readonly("item", &PlayerInteractEvent::getItem,
                               "The item in hand represented by this event, or `None` if no item.")
        .def_property_readonly("has_block", &PlayerInteractEvent::hasBlock, "`True` if this event involved a block.")
        .def_property_readonly("block", &PlayerInteractEvent::getBlock, "The block clicked with this item.")
        .def_property_readonly("block_face", &PlayerInteractEvent::getBlockFace,
                               "The face of the block that was clicked.")
        .def_property_readonly("clicked_position", &PlayerInteractEvent::getClickedPosition, R"doc(
    The exact position on the block the player interacted with.

    This will be `None` outside of `Action.RIGHT_CLICK_BLOCK`. All vector components are between 0.0 and
    1.0 inclusive.
)doc");
    py::class_<PlayerInteractActorEvent, PlayerEvent, ICancellable>(m, "PlayerInteractActorEvent",
                                                                    "Called when a player right-clicks an actor.")
        .def_property_readonly("actor", &PlayerInteractActorEvent::getActor,
                               "The actor that was right-clicked by the player.");
    py::class_<PlayerArmorStandManipulateEvent, PlayerInteractActorEvent>(
        m, "PlayerArmorStandManipulateEvent",
        "Called when a player interacts with an armor stand and will either swap, retrieve or place an item.")
        .def_property_readonly("armor_stand_item", &PlayerArmorStandManipulateEvent::getArmorStandItem,
                               "The item held by the armor stand in the affected slot.")
        .def_property_readonly("player_item", &PlayerArmorStandManipulateEvent::getPlayerItem,
                               "The item held by the player during the interaction.")
        .def_property_readonly("slot", &PlayerArmorStandManipulateEvent::getSlot,
                               "The armor stand equipment slot affected by the interaction.");
    py::class_<PlayerItemConsumeEvent, PlayerEvent, ICancellable>(m, "PlayerItemConsumeEvent", R"doc(
    Called when a player is finishing consuming an item (food, potion, milk bucket).

    If the `ItemStack` is modified the server will use the effects of the new item and not remove the
    original one from the player's inventory.

    If the event is cancelled the effect will not be applied and the item will not be removed from
    the player's inventory.
)doc")
        .def_property_readonly("item", &PlayerItemConsumeEvent::getItem,
                               "An `ItemStack` for the item being consumed.")
        .def_property_readonly("hand", &PlayerItemConsumeEvent::getHand, "The hand used to consume the item.");
    py::class_<PlayerItemHeldEvent, PlayerEvent, ICancellable>(
        m, "PlayerItemHeldEvent", "Called when a player changes their currently held item.")
        .def_property_readonly("new_slot", &PlayerItemHeldEvent::getNewSlot, "The new held slot index.")
        .def_property_readonly("previous_slot", &PlayerItemHeldEvent::getPreviousSlot,
                               "The previous held slot index.");
    py::class_<PlayerToggleSneakEvent, PlayerEvent>(m, "PlayerToggleSneakEvent",
                                                    "Called when a player toggles their sneaking state.")
        .def_property_readonly("is_sneaking", &PlayerToggleSneakEvent::isSneaking,
                               "Whether the player is now sneaking or not.");
    py::class_<PlayerToggleSprintEvent, PlayerEvent>(m, "PlayerToggleSprintEvent",
                                                     "Called when a player toggles their sprinting state.")
        .def_property_readonly("is_sprinting", &PlayerToggleSprintEvent::isSprinting,
                               "Whether the player is now sprinting or not.");
    py::class_<PlayerToggleCrawlEvent, PlayerEvent>(m, "PlayerToggleCrawlEvent",
                                                    "Called when a player toggles their crawling state.")
        .def_property_readonly("is_crawling", &PlayerToggleCrawlEvent::isCrawling,
                               "Whether the player is now crawling or not.");
    py::class_<PlayerToggleFlightEvent, PlayerEvent>(m, "PlayerToggleFlightEvent",
                                                     "Called when a player toggles their flying state.")
        .def_property_readonly("is_flying", &PlayerToggleFlightEvent::isFlying,
                               "Whether the player is now flying or not.");
    py::class_<PlayerJoinEvent, PlayerEvent>(m, "PlayerJoinEvent", "Called when a player joins a server.")
        .def_property("join_message", &PlayerJoinEvent::getJoinMessage, &PlayerJoinEvent::setJoinMessage,
                      "The join message to send to all online players.");
    py::class_<PlayerKickEvent, PlayerEvent, ICancellable>(m, "PlayerKickEvent",
                                                           "Called when a player gets kicked from the server.")
        .def_property("reason", &PlayerKickEvent::getReason, &PlayerKickEvent::setReason,
                      "The reason why the player is getting kicked.");
    py::class_<PlayerLevelChangeEvent, PlayerEvent>(m, "PlayerLevelChangeEvent",
                                                    "Called when a player's level changes.")
        .def_property_readonly("old_level", &PlayerLevelChangeEvent::getOldLevel,
                               "The player's level before the change.")
        .def_property_readonly("new_level", &PlayerLevelChangeEvent::getNewLevel,
                               "The player's level after the change.");
    py::class_<PlayerLoginEvent, PlayerEvent, ICancellable>(m, "PlayerLoginEvent",
                                                            "Called when a player attempts to login in.")
        .def_property("kick_message", &PlayerLoginEvent::getKickMessage, &PlayerLoginEvent::setKickMessage,
                      "The kick message to display if the event is cancelled.");
    py::class_<PlayerMoveEvent, PlayerEvent, ICancellable>(m, "PlayerMoveEvent", "Called when a player moves.")
        .def_property("from_location", &PlayerMoveEvent::getFrom, &PlayerMoveEvent::setFrom,
                      "The location that this player moved from.")
        .def_property("to_location", &PlayerMoveEvent::getTo, &PlayerMoveEvent::setTo,
                      "The location that this player moved to.");
    py::class_<PlayerJumpEvent, PlayerMoveEvent>(m, "PlayerJumpEvent", "Called when a player jumps.");
    py::class_<PlayerQuitEvent, PlayerEvent>(m, "PlayerQuitEvent", "Called when a player leaves a server.")
        .def_property("quit_message", &PlayerQuitEvent::getQuitMessage, &PlayerQuitEvent::setQuitMessage,
                      "The quit message to send to all online players.");
    auto player_recipe_book_settings_change_event = py::class_<PlayerRecipeBookSettingsChangeEvent, PlayerEvent>(
        m, "PlayerRecipeBookSettingsChangeEvent", "Called when a player changes recipe book settings.");
    py::native_enum<PlayerRecipeBookSettingsChangeEvent::RecipeBookType>(
        player_recipe_book_settings_change_event, "RecipeBookType", "enum.Enum", "The recipe book type.")
        .value("CRAFTING", PlayerRecipeBookSettingsChangeEvent::RecipeBookType::Crafting)
        .value("FURNACE", PlayerRecipeBookSettingsChangeEvent::RecipeBookType::Furnace)
        .value("BLAST_FURNACE", PlayerRecipeBookSettingsChangeEvent::RecipeBookType::BlastFurnace)
        .value("SMOKER", PlayerRecipeBookSettingsChangeEvent::RecipeBookType::Smoker)
        .export_values()
        .finalize();
    player_recipe_book_settings_change_event
        .def_property_readonly("recipe_book_type", &PlayerRecipeBookSettingsChangeEvent::getRecipeBookType,
                               "The type of recipe book whose settings changed.")
        .def_property_readonly("is_filtering", &PlayerRecipeBookSettingsChangeEvent::isFiltering,
                               "Whether recipe filtering is enabled.")
        .def_property_readonly("is_open", &PlayerRecipeBookSettingsChangeEvent::isOpen,
                               "Whether the recipe book is open.");
    auto player_respawn_event =
        py::class_<PlayerRespawnEvent, PlayerEvent>(m, "PlayerRespawnEvent", "Called when a player respawns.");
    py::native_enum<PlayerRespawnEvent::RespawnReason>(player_respawn_event, "RespawnReason", "enum.Enum",
                                                       "An enum to specify the reason a respawn occurred.")
        .value("DEATH", PlayerRespawnEvent::RespawnReason::Death)
        .value("END_PORTAL", PlayerRespawnEvent::RespawnReason::EndPortal)
        .export_values()
        .finalize();
    player_respawn_event.def_property_readonly("respawn_reason", &PlayerRespawnEvent::getRespawnReason,
                                               "The reason this respawn occurred.");
    py::class_<PlayerRiptideEvent, PlayerEvent>(m, "PlayerRiptideEvent", R"doc(
    Called when a player activates the riptide enchantment, using their trident to propel them through the air.

    The riptide action is currently performed client side, so manipulating the player in this event may have
    undesired effects.
)doc")
        .def_property_readonly("item", &PlayerRiptideEvent::getItem, "An `ItemStack` for the trident being used.");
    auto player_set_spawn_event =
        py::class_<PlayerSetSpawnEvent, PlayerEvent, ICancellable>(m, "PlayerSetSpawnEvent", R"doc(
    Called when a player's spawn is set, either by themselves or otherwise.

    Assigning a new `location` redirects the spawn that is about to be written; cancelling leaves the respawn point
    untouched.

    Note:
        Only the location's block coordinates and dimension are written back; Bedrock does not persist yaw/pitch for
        a respawn point. Cancelling stops the respawn point from changing, but not the feedback around it:
        `/spawnpoint` still reports success and a respawn anchor still plays its sound and message, because neither
        consults the setter. The event is not fired when Bedrock clears a respawn point, so `/clearspawnpoint` and
        breaking the bed a player is bound to are both silent.
)doc");
    py::native_enum<PlayerSetSpawnEvent::Cause>(player_set_spawn_event, "Cause", "enum.Enum",
                                                "The cause of the spawn change.")
        .value("BED", PlayerSetSpawnEvent::Cause::Bed)
        .value("RESPAWN_ANCHOR", PlayerSetSpawnEvent::Cause::RespawnAnchor)
        .value("COMMAND", PlayerSetSpawnEvent::Cause::Command)
        .value("PLUGIN", PlayerSetSpawnEvent::Cause::Plugin)
        .value("UNKNOWN", PlayerSetSpawnEvent::Cause::Unknown)
        .export_values()
        .finalize();
    player_set_spawn_event
        .def_property_readonly("cause", &PlayerSetSpawnEvent::getCause, "The cause of the spawn change.")
        .def_property("location", &PlayerSetSpawnEvent::getLocation, &PlayerSetSpawnEvent::setLocation,
                      "The spawn location, or `None` to remove the spawn location.");
    py::class_<PlayerShearActorEvent, PlayerEvent, ICancellable>(m, "PlayerShearActorEvent",
                                                                 "Called when a player shears an actor.")
        .def_property_readonly("actor", &PlayerShearActorEvent::getActor, "The actor being sheared.")
        .def_property_readonly("item", &PlayerShearActorEvent::getItem, "The shears used.")
        .def_property_readonly("hand", &PlayerShearActorEvent::getHand, "The hand used to shear the actor.");
    py::class_<PlayerSkinChangeEvent, PlayerEvent, ICancellable>(m, "PlayerSkinChangeEvent",
                                                                 "Called when a player changes their skin.")
        .def_property_readonly("new_skin", &PlayerSkinChangeEvent::getNewSkin, "The skin that will be applied.")
        .def_property("skin_change_message", &PlayerSkinChangeEvent::getSkinChangeMessage,
                      &PlayerSkinChangeEvent::setSkinChangeMessage,
                      "The message to send to all online players for this skin change.");
    py::class_<PlayerTeleportEvent, PlayerMoveEvent>(
        m, "PlayerTeleportEvent", "Called when a player is teleported from one location to another.");
    py::class_<PlayerPortalEvent, PlayerTeleportEvent>(
        m, "PlayerPortalEvent", "Called when a player is about to teleport because it is in contact with a portal.");
    py::class_<PlayerPickupArrowEvent, PlayerEvent, ICancellable>(
        m, "PlayerPickupArrowEvent", "Called when a player picks up an arrow or a thrown trident from the ground.")
        .def_property_readonly("arrow", &PlayerPickupArrowEvent::getArrow, "The arrow picked up by the player.");
    py::class_<PlayerPickupExperienceEvent, PlayerEvent, ICancellable>(m, "PlayerPickupExperienceEvent", R"doc(
    Called when a player picks up an experience orb.

    Cancelling the event leaves the orb in the world.
)doc")
        .def_property_readonly("amount", &PlayerPickupExperienceEvent::getAmount,
                               "The amount of experience the orb is worth.");
    py::class_<PlayerPickupItemEvent, PlayerEvent, ICancellable>(
        m, "PlayerPickupItemEvent", "Called when a player picks an item up from the ground.")
        .def_property_readonly("item", &PlayerPickupItemEvent::getItem,
                               "The Item picked up by the entity.");

    // Inventory events
    py::class_<InventoryEvent, Event>(m, "InventoryEvent", "Represents a player related inventory event.")
        .def_property_readonly("inventory", &InventoryEvent::getInventory, py::return_value_policy::reference,
                               "The primary `Inventory` involved in this transaction.");
    py::class_<InventoryInteractEvent, InventoryEvent, ICancellable>(m, "InventoryInteractEvent", R"doc(
    An abstract base class for events that describe an interaction between a `Player` and the contents of an
    `Inventory`.
)doc")
        .def_property_readonly("who_clicked", &InventoryInteractEvent::getWhoClicked,
                               "The player who performed the click.");
    py::class_<InventoryOpenEvent, InventoryEvent, ICancellable>(m, "InventoryOpenEvent", R"doc(
    Called when a player opens an inventory.

    If this event is cancelled, the inventory will not be opened and the player will not see the container screen.
)doc")
        .def_property_readonly("player", &InventoryOpenEvent::getPlayer, "The player who is opening the inventory.");
    py::class_<InventoryCloseEvent, InventoryEvent>(m, "InventoryCloseEvent",
                                                    "Called when a player closes an inventory.")
        .def_property_readonly("player", &InventoryCloseEvent::getPlayer, "The player who is closing the inventory.");

    // Server events
    py::class_<ServerEvent, Event>(m, "ServerEvent", "Represents a Server-related event.");
    py::class_<BroadcastMessageEvent, ServerEvent, ICancellable>(m, "BroadcastMessageEvent", R"doc(
    Event triggered for server broadcast messages such as from `Server.broadcast`.

    This event should be async if fired from an async thread.
)doc")
        .def_property("message", &BroadcastMessageEvent::getMessage, &BroadcastMessageEvent::setMessage,
                      "The message to broadcast.")
        .def_property_readonly("recipients", &BroadcastMessageEvent::getRecipients,
                               "The set of `CommandSender`s who will see this broadcast message.");
    py::class_<MapInitializeEvent, ServerEvent>(m, "MapInitializeEvent", "Called when a map is initialized.")
        .def_property_readonly("map", &MapInitializeEvent::getMap, py::return_value_policy::reference,
                               "The `Map` initialized in this event.");
    py::class_<PacketReceiveEvent, ServerEvent, ICancellable>(
        m, "PacketReceiveEvent", "Called when the server receives a packet from a connected client.")
        .def_property_readonly("packet_id", &PacketReceiveEvent::getPacketId, "The ID of the packet.")
        .def_property(
            "payload", [](const PacketReceiveEvent &self) { return py::bytes(self.getPayload()); },
            [](PacketReceiveEvent &self, const py::bytes &payload) { self.setPayload(payload); },
            "The raw packet data, excluding the header.")
        .def_property_readonly("player", &PacketReceiveEvent::getPlayer, R"doc(
    The `Player` who is involved in this event.

    This may return `None` if the packet is sent before the player completes the login process.
)doc")
        .def_property_readonly("address", &PacketReceiveEvent::getAddress, py::return_value_policy::copy,
                               "The network address of the client that sent this packet.")
        .def_property_readonly("sub_client_id", &PacketReceiveEvent::getSubClientId, R"doc(
    The SubClient ID.

    Range is 0 to 3 (0 = primary client; 1-3 = split-screen clients).
)doc");

    py::class_<PacketSendEvent, ServerEvent, ICancellable>(
        m, "PacketSendEvent", "Called when the server sends a packet to a connected client.")
        .def_property_readonly("packet_id", &PacketSendEvent::getPacketId, "The ID of the packet.")
        .def_property(
            "payload", [](const PacketSendEvent &self) { return py::bytes(self.getPayload()); },
            [](PacketSendEvent &self, const py::bytes &payload) { self.setPayload(payload); },
            "The raw packet data, excluding the header.")
        .def_property_readonly("player", &PacketSendEvent::getPlayer, R"doc(
    The `Player` who is involved in this event.

    This may return `None` if the packet is sent before the player completes the login process.
)doc")
        .def_property_readonly("address", &PacketSendEvent::getAddress, py::return_value_policy::copy,
                               "The network address to which this packet is being sent.")
        .def_property_readonly("sub_client_id", &PacketSendEvent::getSubClientId, R"doc(
    The SubClient ID.

    Range is 0 to 3 (0 = primary client; 1-3 = split-screen clients).
)doc");

    py::class_<PluginEnableEvent, ServerEvent>(m, "PluginEnableEvent", "Called when a plugin is enabled.")
        .def_property_readonly("plugin", &PluginEnableEvent::getPlugin, py::return_value_policy::reference,
                               "The `Plugin` involved in this event.");

    py::class_<PluginDisableEvent, ServerEvent>(m, "PluginDisableEvent", "Called when a plugin is disabled.")
        .def_property_readonly("plugin", &PluginDisableEvent::getPlugin, py::return_value_policy::reference,
                               "The `Plugin` involved in this event.");

    py::class_<ScriptMessageEvent, ServerEvent, ICancellable>(
        m, "ScriptMessageEvent", "Called when a message is sent by the `/scriptevent` command.")
        .def_property_readonly("message_id", &ScriptMessageEvent::getMessageId, "The message id to send.")
        .def_property_readonly("message", &ScriptMessageEvent::getMessage, "The message to send.")
        .def_property_readonly("sender", &ScriptMessageEvent::getSender,
                               "The command sender who sent the script message.");

    py::class_<ServerCommandEvent, ServerEvent, ICancellable>(m, "ServerCommandEvent", R"doc(
    Called when a command is run by a non-player, early in the command handling process.

    You should not use this except for a few cases like logging commands, blocking commands on
    certain places, or applying modifiers.

    The command message contains a slash '/' at the start.
)doc")
        .def_property_readonly("sender", &ServerCommandEvent::getSender, "The command sender.")
        .def_property("command", &ServerCommandEvent::getCommand, &ServerCommandEvent::setCommand,
                      "The command that the server is attempting to execute from the console.");

    py::class_<ServerListPingEvent, ServerEvent, ICancellable>(m, "ServerListPingEvent",
                                                               "Called when a server ping is coming in.")
        .def_property_readonly("address", &ServerListPingEvent::getAddress, py::return_value_policy::copy,
                               "The address the ping is coming from.")
        .def_property("server_guid", &ServerListPingEvent::getServerGuid, &ServerListPingEvent::setServerGuid,
                      "The unique identifier of the server.")
        .def_property("local_port", &ServerListPingEvent::getLocalPort, &ServerListPingEvent::setLocalPort,
                      "The local port of the server.")
        .def_property("motd", &ServerListPingEvent::getMotd, &ServerListPingEvent::setMotd,
                      "The message of the day.")
        .def_property_readonly("network_protocol_version", &ServerListPingEvent::getNetworkProtocolVersion,
                               "The network protocol version of this server.")
        .def_property("minecraft_version_network", &ServerListPingEvent::getMinecraftVersionNetwork,
                      &ServerListPingEvent::setMinecraftVersionNetwork,
                      "The network version of Minecraft that is supported by this server.")
        .def_property("num_players", &ServerListPingEvent::getNumPlayers, &ServerListPingEvent::setNumPlayers,
                      "The number of players online.")
        .def_property("max_players", &ServerListPingEvent::getMaxPlayers, &ServerListPingEvent::setMaxPlayers,
                      "The maximum number of players allowed.")
        .def_property("level_name", &ServerListPingEvent::getLevelName, &ServerListPingEvent::setLevelName,
                      "The level name.")
        .def_property("game_mode", &ServerListPingEvent::getGameMode, &ServerListPingEvent::setGameMode,
                      "The current game mode.");

    auto server_load_event = py::class_<ServerLoadEvent, ServerEvent>(
        m, "ServerLoadEvent", "Called when either the server startup or reload has completed.");
    py::native_enum<ServerLoadEvent::LoadType>(server_load_event, "LoadType", "enum.Enum",
                                               "Represents the context in which the server was loaded.")
        .value("STARTUP", ServerLoadEvent::LoadType::Startup)
        .value("RELOAD", ServerLoadEvent::LoadType::Reload)
        .export_values()
        .finalize();
    server_load_event.def_property_readonly("type", &ServerLoadEvent::getType, "The load type of this event.");

    py::class_<WeatherEvent, Event>(m, "WeatherEvent", "Represents a Weather-related event.")
        .def_property_readonly("level", &WeatherChangeEvent::getLevel, py::return_value_policy::reference,
                               "The `Level` where this event is occurring.");
    py::class_<ThunderChangeEvent, WeatherEvent, ICancellable>(m, "ThunderChangeEvent",
                                                               "Called when the thunder state in a world is changing.")
        .def_property_readonly("to_thunder_state", &ThunderChangeEvent::toThunderState,
                               "`True` if the weather is being set to thundering, `False` otherwise.");
    py::class_<WeatherChangeEvent, WeatherEvent, ICancellable>(
        m, "WeatherChangeEvent", "Called when the weather (rain) state in a world is changing.")
        .def_property_readonly("to_weather_state", &WeatherChangeEvent::toWeatherState,
                               "`True` if the weather is being set to raining, `False` otherwise.");
}

}  // namespace endstone::python
