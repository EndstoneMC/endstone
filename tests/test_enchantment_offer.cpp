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

#include <stdexcept>
#include <string>
#include <utility>

#include <gtest/gtest.h>

#include "endstone/enchantments/enchantment_offer.h"

namespace {

class TestEnchantment final : public endstone::Enchantment {
public:
    explicit TestEnchantment(endstone::EnchantmentId id) : id_(std::move(id)) {}

    [[nodiscard]] endstone::EnchantmentId getId() const override { return id_; }
    [[nodiscard]] std::string getTranslationKey() const override { return "test.enchantment"; }
    [[nodiscard]] int getMaxLevel() const override { return 5; }
    [[nodiscard]] int getStartLevel() const override { return 1; }
    [[nodiscard]] bool conflictsWith(const endstone::Enchantment &) const override { return false; }
    [[nodiscard]] bool canEnchantItem(const endstone::ItemStack &) const override { return true; }

private:
    endstone::EnchantmentId id_;
};

}  // namespace

TEST(EnchantmentOfferTest, StoresAndUpdatesTheCompleteOffer)
{
    TestEnchantment sharpness{endstone::Enchantment::Sharpness};
    TestEnchantment unbreaking{endstone::Enchantment::Unbreaking};
    endstone::EnchantmentOffer offer{
        endstone::EnchantmentOffer::Enchantments{{&sharpness, 3}},
        12,
    };

    EXPECT_EQ(offer.getEnchants(), (endstone::EnchantmentOffer::Enchantments{{&sharpness, 3}}));
    EXPECT_EQ(offer.getCost(), 12);

    offer.setEnchants({{&sharpness, 4}, {&unbreaking, 2}});
    offer.setCost(18);
    EXPECT_EQ(offer.getEnchants(), (endstone::EnchantmentOffer::Enchantments{{&sharpness, 4}, {&unbreaking, 2}}));
    EXPECT_EQ(offer.getCost(), 18);
}

TEST(EnchantmentOfferTest, RejectsInvalidValuesWithoutChangingTheOffer)
{
    TestEnchantment sharpness{endstone::Enchantment::Sharpness};
    endstone::EnchantmentOffer offer{
        endstone::EnchantmentOffer::Enchantments{{&sharpness, 3}},
        12,
    };

    EXPECT_THROW((endstone::EnchantmentOffer{endstone::EnchantmentOffer::Enchantments{}, 1}), std::invalid_argument);
    EXPECT_THROW((endstone::EnchantmentOffer{endstone::EnchantmentOffer::Enchantments{{nullptr, 1}}, 1}),
                 std::invalid_argument);
    EXPECT_THROW((endstone::EnchantmentOffer{endstone::EnchantmentOffer::Enchantments{{&sharpness, 0}}, 1}),
                 std::invalid_argument);
    EXPECT_THROW((endstone::EnchantmentOffer{endstone::EnchantmentOffer::Enchantments{{&sharpness, 1}}, 0}),
                 std::invalid_argument);
    EXPECT_THROW((endstone::EnchantmentOffer{endstone::EnchantmentOffer::Enchantments{{&sharpness, 1}}, 256}),
                 std::invalid_argument);

    EXPECT_THROW(offer.setEnchants({}), std::invalid_argument);
    EXPECT_THROW(offer.setCost(0), std::invalid_argument);
    EXPECT_THROW(offer.setCost(256), std::invalid_argument);
    EXPECT_EQ(offer.getEnchants(), (endstone::EnchantmentOffer::Enchantments{{&sharpness, 3}}));
    EXPECT_EQ(offer.getCost(), 12);
}
