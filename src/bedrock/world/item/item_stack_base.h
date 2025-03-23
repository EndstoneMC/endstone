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

#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <string_view>

#include "bedrock/nbt/compound_tag.h"
#include "bedrock/shared_ptr.h"
#include "bedrock/world/item/item_descriptor.h"
#include "bedrock/world/level/block/block_legacy.h"
#include "bedrock/world/level/tick.h"

class Item;

class ItemStackBase {
protected:
    ItemStackBase();
    explicit ItemStackBase(const BlockLegacy &block, int count = 1);
    explicit ItemStackBase(std::string_view name, int count = 1, int aux_value = 0,
                           CompoundTag const *user_data = nullptr);
    explicit ItemStackBase(Item const &item, int count = 1, int aux_value = 0, CompoundTag const *user_data = nullptr);
    ItemStackBase(const ItemStackBase &);
    ItemStackBase &operator=(const ItemStackBase &);

public:
    virtual ~ItemStackBase();
    virtual void reinit(Item const &, int, int);
    virtual void reinit(BlockLegacy const &, int);
    virtual void reinit(std::string_view, int, int);
    [[nodiscard]] ItemDescriptor getDescriptor() const;
    virtual void setNull(std::optional<std::string>);
    [[nodiscard]] virtual std::string toString() const;
    [[nodiscard]] virtual std::string toDebugString() const;

    [[nodiscard]] bool isNull() const;
    [[nodiscard]] bool hasCustomHoverName() const;
    [[nodiscard]] std::string getCustomName() const;
    [[nodiscard]] std::int16_t getId() const;
    [[nodiscard]] std::uint16_t getAuxValue() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] const Item *getItem() const;
    [[nodiscard]] bool hasUserData() const;
    void setUserData(std::unique_ptr<CompoundTag>);
    [[nodiscard]] const CompoundTag *getUserData() const;
    CompoundTag *getUserData();
    [[nodiscard]] bool isBlock() const;
    [[nodiscard]] const Block *getBlock() const;
    void set(std::uint8_t count);
    [[nodiscard]] std::uint8_t getCount() const;  // Endstone

    static const std::string TAG_DISPLAY;
    static const std::string TAG_DISPLAY_NAME;
    static const std::string TAG_LORE;
    static constexpr int ID_MASK = 0xffff;
    static constexpr int ENCHANT_MASK = 0x8000;
    static constexpr int AUX_VALUE_MASK = 0x7fff;

protected:
    void init(const BlockLegacy &block, int count);
    void init(const Item &item, int count, int aux_value, const CompoundTag *user_data, bool do_remap);
    void init(int id, int count, int aux_value, bool do_remap);

    static const std::string TAG_CHARGED_ITEM;
    WeakPtr<Item> item_{nullptr};                      // +8
    std::unique_ptr<CompoundTag> user_data_{nullptr};  // +16
    const Block *block_{nullptr};                      // +24
    std::int16_t aux_value_{0};                        // +32
    std::uint8_t count_{0};                            // +34

private:
    bool valid_deprecated_{true};  // +35
    bool _setItem(int id, bool do_remap);

protected:
    bool show_pick_up_{true};                              // +36
    bool was_picked_up_{false};                            // +37
    std::chrono::steady_clock::time_point pick_up_time_;   // +40
    std::vector<const BlockLegacy *> can_place_on_;        // +56
    std::size_t can_place_on_hash_{0};                     // +80
    std::vector<const BlockLegacy *> can_destroy_;         // +88
    std::size_t can_destroy_hash_{0};                      // +112
    Tick blocking_tick_{};                                 // +120
    std::unique_ptr<ItemInstance> charged_item_{nullptr};  // +128

private:
    void _updateCompareHashes();
    void _checkForItemWorldCompatibility();
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackBase, 128, 128);
