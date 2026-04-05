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

#include <gtest/gtest.h>

#include "endstone/actor/actor.h"
#include "endstone/actor/item.h"
#include "endstone/actor/mob.h"
#include "endstone/command/block_command_sender.h"
#include "endstone/command/command_sender.h"
#include "endstone/command/console_command_sender.h"
#include "endstone/core/type.h"
#include "endstone/inventory/meta/book_meta.h"
#include "endstone/inventory/meta/crossbow_meta.h"
#include "endstone/inventory/meta/item_meta.h"
#include "endstone/inventory/meta/map_meta.h"
#include "endstone/inventory/meta/writable_book_meta.h"
#include "endstone/permissions/permissible.h"
#include "endstone/player.h"

using namespace endstone;

class ObjectTypeTest : public ::testing::Test {
protected:
    static void SetUpTestSuite()
    {
        core::registerTypes();
    }
};

// --- Permissible / CommandSender / Actor hierarchy ---

TEST_F(ObjectTypeTest, PlayerIsPlayer)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(Player), typeid(Player)));
}

TEST_F(ObjectTypeTest, PlayerIsMob)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(Player), typeid(Mob)));
}

TEST_F(ObjectTypeTest, PlayerIsActor)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(Player), typeid(Actor)));
}

TEST_F(ObjectTypeTest, PlayerIsCommandSender)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(Player), typeid(CommandSender)));
}

TEST_F(ObjectTypeTest, PlayerIsPermissible)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(Player), typeid(Permissible)));
}

TEST_F(ObjectTypeTest, MobIsNotPlayer)
{
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(Mob), typeid(Player)));
}

TEST_F(ObjectTypeTest, PlayerIsNotItem)
{
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(Player), typeid(Item)));
}

TEST_F(ObjectTypeTest, ItemIsActor)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(Item), typeid(Actor)));
}

TEST_F(ObjectTypeTest, ItemIsNotMob)
{
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(Item), typeid(Mob)));
}

TEST_F(ObjectTypeTest, ConsoleIsCommandSender)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(ConsoleCommandSender), typeid(CommandSender)));
}

TEST_F(ObjectTypeTest, ConsoleIsNotActor)
{
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(ConsoleCommandSender), typeid(Actor)));
}

TEST_F(ObjectTypeTest, BlockSenderIsNotConsole)
{
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(BlockCommandSender), typeid(ConsoleCommandSender)));
}

TEST_F(ObjectTypeTest, ActorIsNotConsole)
{
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(Actor), typeid(ConsoleCommandSender)));
}

// --- ItemMeta hierarchy ---

TEST_F(ObjectTypeTest, BookMetaIsWritableBookMeta)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(BookMeta), typeid(WritableBookMeta)));
}

TEST_F(ObjectTypeTest, BookMetaIsItemMeta)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(BookMeta), typeid(ItemMeta)));
}

TEST_F(ObjectTypeTest, WritableBookMetaIsItemMeta)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(WritableBookMeta), typeid(ItemMeta)));
}

TEST_F(ObjectTypeTest, WritableBookMetaIsNotBookMeta)
{
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(WritableBookMeta), typeid(BookMeta)));
}

TEST_F(ObjectTypeTest, MapMetaIsItemMeta)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(MapMeta), typeid(ItemMeta)));
}

TEST_F(ObjectTypeTest, MapMetaIsNotWritableBookMeta)
{
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(MapMeta), typeid(WritableBookMeta)));
}

TEST_F(ObjectTypeTest, CrossbowMetaIsItemMeta)
{
    EXPECT_TRUE(core::isTypeInstanceOf(typeid(CrossbowMeta), typeid(ItemMeta)));
}

// --- Cross-hierarchy rejection ---

TEST_F(ObjectTypeTest, PlayerIsNotItemMeta)
{
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(Player), typeid(ItemMeta)));
}

TEST_F(ObjectTypeTest, ItemMetaIsNotPermissible)
{
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(ItemMeta), typeid(Permissible)));
}

TEST_F(ObjectTypeTest, UnregisteredTypeReturnsFalse)
{
    struct Unknown : Object {};
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(Unknown), typeid(Permissible)));
    EXPECT_FALSE(core::isTypeInstanceOf(typeid(Player), typeid(Unknown)));
}
