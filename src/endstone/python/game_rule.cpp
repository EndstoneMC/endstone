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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {
void init_game_rule(py::module_ &m)
{
    def_registry_type(py::class_<GameRule>(m, "GameRule", "Represents a game rule."))
        .def_property_readonly("id", &GameRule::getId, "The identifier of this game rule.")
        .def_property_readonly("translation_key", &GameRule::getTranslationKey,
                               "Raises, as Bedrock does not localize game rule names.")
        .def_static("get", &GameRule::get, py::arg("name"), R"doc(
    Attempts to get the `GameRule` with the given name.

    Args:
        name: The identifier of the game rule (e.g. `minecraft:dofiretick`).

    Returns:
        The `GameRule`, or `None` if no game rule with that name exists.
)doc",
                    py::return_value_policy::reference)
        .def_static(
            "__class_getitem__", [](const py::object &) { return py::type::of<GameRule>(); }, py::arg("item"))
        .def_property_readonly_static("COMMAND_BLOCK_OUTPUT", id(GameRule::CommandBlockOutput))
        .def_property_readonly_static("COMMAND_BLOCKS_ENABLED", id(GameRule::CommandBlocksEnabled))
        .def_property_readonly_static("DO_DAY_LIGHT_CYCLE", id(GameRule::DoDayLightCycle))
        .def_property_readonly_static("DO_ENTITY_DROPS", id(GameRule::DoEntityDrops))
        .def_property_readonly_static("DO_FIRE_TICK", id(GameRule::DoFireTick))
        .def_property_readonly_static("DO_IMMEDIATE_RESPAWN", id(GameRule::DoImmediateRespawn))
        .def_property_readonly_static("DO_INSOMNIA", id(GameRule::DoInsomnia))
        .def_property_readonly_static("DO_LIMITED_CRAFTING", id(GameRule::DoLimitedCrafting))
        .def_property_readonly_static("DO_MOB_LOOT", id(GameRule::DoMobLoot))
        .def_property_readonly_static("DO_MOB_SPAWNING", id(GameRule::DoMobSpawning))
        .def_property_readonly_static("DO_TILE_DROPS", id(GameRule::DoTileDrops))
        .def_property_readonly_static("DO_WEATHER_CYCLE", id(GameRule::DoWeatherCycle))
        .def_property_readonly_static("DROWNING_DAMAGE", id(GameRule::DrowningDamage))
        .def_property_readonly_static("FALL_DAMAGE", id(GameRule::FallDamage))
        .def_property_readonly_static("FIRE_DAMAGE", id(GameRule::FireDamage))
        .def_property_readonly_static("FREEZE_DAMAGE", id(GameRule::FreezeDamage))
        .def_property_readonly_static("FUNCTION_COMMAND_LIMIT", id(GameRule::FunctionCommandLimit))
        .def_property_readonly_static("KEEP_INVENTORY", id(GameRule::KeepInventory))
        .def_property_readonly_static("LOCATOR_BAR", id(GameRule::LocatorBar))
        .def_property_readonly_static("MAX_COMMAND_CHAIN_LENGTH", id(GameRule::MaxCommandChainLength))
        .def_property_readonly_static("MOB_GRIEFING", id(GameRule::MobGriefing))
        .def_property_readonly_static("NATURAL_REGENERATION", id(GameRule::NaturalRegeneration))
        .def_property_readonly_static("PLAYERS_SLEEPING_PERCENTAGE", id(GameRule::PlayersSleepingPercentage))
        .def_property_readonly_static("PLAYER_WAYPOINTS", id(GameRule::PlayerWaypoints))
        .def_property_readonly_static("PROJECTILES_CAN_BREAK_BLOCKS", id(GameRule::ProjectilesCanBreakBlocks))
        .def_property_readonly_static("PVP", id(GameRule::Pvp))
        .def_property_readonly_static("RANDOM_TICK_SPEED", id(GameRule::RandomTickSpeed))
        .def_property_readonly_static("RECIPES_UNLOCK", id(GameRule::RecipesUnlock))
        .def_property_readonly_static("RESPAWN_BLOCKS_EXPLODE", id(GameRule::RespawnBlocksExplode))
        .def_property_readonly_static("SEND_COMMAND_FEEDBACK", id(GameRule::SendCommandFeedback))
        .def_property_readonly_static("SHOW_BORDER_EFFECT", id(GameRule::ShowBorderEffect))
        .def_property_readonly_static("SHOW_COORDINATES", id(GameRule::ShowCoordinates))
        .def_property_readonly_static("SHOW_DAYS_PLAYED", id(GameRule::ShowDaysPlayed))
        .def_property_readonly_static("SHOW_DEATH_MESSAGES", id(GameRule::ShowDeathMessages))
        .def_property_readonly_static("SHOW_RECIPE_MESSAGES", id(GameRule::ShowRecipeMessages))
        .def_property_readonly_static("SHOW_TAGS", id(GameRule::ShowTags))
        .def_property_readonly_static("SPAWN_RADIUS", id(GameRule::SpawnRadius))
        .def_property_readonly_static("TNT_EXPLODES", id(GameRule::TntExplodes))
        .def_property_readonly_static("TNT_EXPLOSION_DROP_DECAY", id(GameRule::TntExplosionDropDecay));
}
}  // namespace endstone::python
