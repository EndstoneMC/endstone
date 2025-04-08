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

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "endstone/inventory/item_stack.h"

using endstone::ItemMeta;
using endstone::ItemStack;

// Test that modifying the lore on a pure ItemStack works correctly.
TEST(ItemStackMetaTest, TestSetLore)
{
    // Create a new pure ItemStack.
    ItemStack item("minecraft:diamond_sword");

    // Get a fresh copy of the ItemMeta.
    auto meta = item.getItemMeta();
    std::vector<std::string> lore = {"A powerful blade", "of destiny"};
    meta->setLore(lore);

    // Apply the modified meta back to the item.
    bool success = item.setItemMeta(meta.get());
    EXPECT_TRUE(success) << "Setting ItemMeta should succeed";

    // Verify that the lore is set correctly.
    auto result_meta = item.getItemMeta();
    EXPECT_TRUE(result_meta->hasLore());
    EXPECT_EQ(lore, result_meta->getLore()) << "Lore should be set as expected";
}

// Test that removing the lore on a pure ItemStack works as expected.
TEST(ItemStackMetaTest, TestRemoveLore)
{
    // Create a new pure ItemStack and set initial lore.
    ItemStack item("minecraft:diamond_sword");
    {
        auto meta = item.getItemMeta();
        std::vector<std::string> lore = {"A powerful blade", "of destiny"};
        meta->setLore(lore);
        EXPECT_TRUE(item.setItemMeta(meta.get())) << "Setting initial lore should succeed";
    }

    // Now remove the lore by setting it to an empty vector.
    {
        auto meta = item.getItemMeta();
        meta->setLore(std::vector<std::string>());
        EXPECT_TRUE(item.setItemMeta(meta.get())) << "Removing lore should succeed";
    }

    // Verify that lore is now removed (empty).
    auto result_meta = item.getItemMeta();
    EXPECT_FALSE(result_meta->hasLore()) << "Lore should be removed";
}

// Test clearing the meta with a null pointer.
TEST(ItemStackMetaTest, TestClearItemMeta)
{
    // Create a new pure ItemStack and set initial lore.
    ItemStack item("minecraft:diamond_sword");
    {
        auto meta = item.getItemMeta();
        std::vector<std::string> lore = {"A powerful blade", "of destiny"};
        meta->setLore(lore);
        EXPECT_TRUE(item.setItemMeta(meta.get())) << "Setting initial lore should succeed";
    }

    EXPECT_TRUE(item.setItemMeta(nullptr)) << "Clearing meta via setItemMeta(nullptr) should succeed";

    // Verify that lore is now removed (empty).
    auto result_meta = item.getItemMeta();
    EXPECT_FALSE(item.hasItemMeta());
    EXPECT_FALSE(result_meta->hasLore()) << "Lore should be cleared";
}
