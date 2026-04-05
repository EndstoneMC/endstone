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

#include "endstone/core/type.h"

#include <typeindex>
#include <unordered_map>

#include <entt/meta/factory.hpp>
#include <entt/meta/resolve.hpp>

#include "endstone/actor/actor.h"
#include "endstone/actor/item.h"
#include "endstone/actor/mob.h"
#include "endstone/command/block_command_sender.h"
#include "endstone/command/command_sender.h"
#include "endstone/command/console_command_sender.h"
#include "endstone/inventory/meta/book_meta.h"
#include "endstone/inventory/meta/crossbow_meta.h"
#include "endstone/inventory/meta/item_meta.h"
#include "endstone/inventory/meta/map_meta.h"
#include "endstone/inventory/meta/writable_book_meta.h"
#include "endstone/permissions/permissible.h"
#include "endstone/player.h"

namespace endstone::core {

namespace {
std::unordered_map<std::type_index, entt::meta_type> MetaLookup;

template <typename T>
entt::meta_factory<T> registerType()
{
    auto factory = entt::meta_factory<T>{};
    MetaLookup[std::type_index(typeid(T))] = entt::resolve<T>();
    return factory;
}
}  // namespace

void registerTypes()
{
    // Permissible / CommandSender / Actor hierarchy
    registerType<Permissible>();
    registerType<CommandSender>().base<Permissible>();
    registerType<ConsoleCommandSender>().base<CommandSender>();
    registerType<BlockCommandSender>().base<CommandSender>();
    registerType<Actor>().base<CommandSender>();
    registerType<Mob>().base<Actor>();
    registerType<Player>().base<Mob>();
    registerType<Item>().base<Actor>();

    // ItemMeta hierarchy
    registerType<ItemMeta>();
    registerType<MapMeta>().base<ItemMeta>();
    registerType<WritableBookMeta>().base<ItemMeta>();
    registerType<BookMeta>().base<WritableBookMeta>();
    registerType<CrossbowMeta>().base<ItemMeta>();
}

bool isTypeInstanceOf(const std::type_info &from, const std::type_info &target)
{
    if (from == target) {
        return true;
    }
    auto it_from = MetaLookup.find(std::type_index(from));
    auto it_to = MetaLookup.find(std::type_index(target));
    if (it_from == MetaLookup.end() || it_to == MetaLookup.end()) {
        return false;
    }
    return it_from->second.can_cast(it_to->second);
}

bool isInstanceOf(const Object &obj, const std::type_info &target)
{
    return isTypeInstanceOf(obj.getClassTypeId(), target);
}

}  // namespace endstone::core
