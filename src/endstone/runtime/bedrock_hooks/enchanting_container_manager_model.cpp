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

#include "bedrock/world/containers/managers/enchanting_container_manager_model.h"

#include <cstddef>
#include <string>
#include <utility>

#include <gsl/util>

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/player_enchant_options_packet.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/containers/container_enum.h"
#include "bedrock/world/containers/models/container_model.h"
#include "bedrock/world/item/enchanting/enchant_utils.h"
#include "endstone/check.h"
#include "endstone/core/block/block.h"
#include "endstone/core/enchantments/enchantment.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/inventory/inventory.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/enchantment/prepare_item_enchant_event.h"
#include "endstone/runtime/hook.h"

namespace {
constexpr std::size_t kOfferCount = 3;

class EndstoneContainerModelInventory final : public endstone::core::EndstoneInventoryBase<endstone::Inventory> {
public:
    explicit EndstoneContainerModelInventory(std::shared_ptr<ContainerModel> model) : model_(std::move(model)) {}

    [[nodiscard]] int getSize() const override { return model_->getContainerSize(); }

    [[nodiscard]] int getMaxStackSize() const override
    {
        const auto *container = model_->_getContainer();
        return container == nullptr ? Container::LARGE_MAX_STACK_SIZE : container->getMaxStackSize();
    }

    [[nodiscard]] std::optional<endstone::ItemStack> getItem(int index) const override
    {
        const auto &item = model_->getItemStack(index);
        if (item.isNull()) {
            return std::nullopt;
        }
        return endstone::core::EndstoneItemStack::fromMinecraft(item);
    }

    void setItem(int index, std::optional<endstone::ItemStack> item) override
    {
        const auto item_stack =
            item.has_value() ? endstone::core::EndstoneItemStack::toMinecraft(item.value()) : ItemStack::EMPTY_ITEM;
        model_->setItem(index, item_stack);
    }

    [[nodiscard]] std::vector<std::optional<endstone::ItemStack>> getContents() const override
    {
        std::vector<std::optional<endstone::ItemStack>> contents;
        contents.reserve(getSize());
        for (int i = 0; i < getSize(); ++i) {
            contents.emplace_back(getItem(i));
        }
        return contents;
    }

    [[nodiscard]] bool isEmpty() const override
    {
        for (int i = 0; i < getSize(); ++i) {
            if (!model_->getItemStack(i).isNull()) {
                return false;
            }
        }
        return true;
    }

private:
    [[nodiscard]] Container &getContainer() const override
    {
        auto *container = model_->_getContainer();
        endstone::Preconditions::checkState(container != nullptr, "The inventory's backing container is unavailable.");
        return *container;
    }

    std::shared_ptr<ContainerModel> model_;
};

const endstone::Enchantment *getEndstoneEnchantment(const EnchantmentInstance &instance)
{
    const auto *enchant = Enchant::getEnchant(instance.getEnchantType());
    if (enchant == nullptr) {
        return nullptr;
    }
    return endstone::Enchantment::get(endstone::EnchantmentId::minecraft(enchant->getStringId().getString()));
}

bool getOffer(const ItemEnchantOption &option, std::optional<endstone::EnchantmentOffer> &offer)
{
    const auto instances = option.enchants.getAllEnchants();
    if (instances.empty()) {
        offer = std::nullopt;
        return true;
    }
    if (option.cost <= 0) {
        return false;
    }

    endstone::EnchantmentOffer::Enchantments enchants;
    for (const auto &instance : instances) {
        const auto *enchantment = getEndstoneEnchantment(instance);
        if (enchantment == nullptr || instance.getEnchantLevel() <= 0 ||
            !enchants.emplace(enchantment, instance.getEnchantLevel()).second) {
            return false;
        }
    }
    offer.emplace(std::move(enchants), option.cost);
    return true;
}

bool isSameOffer(const std::optional<endstone::EnchantmentOffer> &lhs,
                 const std::optional<endstone::EnchantmentOffer> &rhs)
{
    if (!lhs || !rhs) {
        return lhs.has_value() == rhs.has_value();
    }
    return lhs->getCost() == rhs->getCost() && lhs->getEnchants() == rhs->getEnchants();
}

bool areSameOffers(const endstone::PrepareItemEnchantEvent::Offers &lhs,
                   const endstone::PrepareItemEnchantEvent::Offers &rhs)
{
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (!isSameOffer(lhs[i], rhs[i])) {
            return false;
        }
    }
    return true;
}

bool applyOffer(ItemEnchantOption &option, const std::optional<endstone::EnchantmentOffer> &offer,
                const std::optional<endstone::EnchantmentOffer> &original_offer)
{
    if (!offer) {
        option.cost = 0;
        option.enchants.setEnchantInstances({});
        option.enchant_net_id = {};
        return true;
    }

    option.cost = offer->getCost();
    if (!original_offer || offer->getEnchants() != original_offer->getEnchants()) {
        ItemEnchants::Enchantments instances;
        for (const auto &[enchantment, level] : offer->getEnchants()) {
            const auto *endstone_enchantment = dynamic_cast<const endstone::core::EndstoneEnchantment *>(enchantment);
            if (endstone_enchantment == nullptr || level <= 0) {
                return false;
            }
            const auto type = endstone_enchantment->getHandle().getEnchantType();
            instances[0].emplace_back(type, level);
        }
        option.enchants.setEnchantInstances(std::move(instances));
    }
    return true;
}
}  // namespace

void EnchantingContainerManagerModel::recalculateOptions()
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::PrepareItemEnchantEvent>()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&EnchantingContainerManagerModel::recalculateOptions, this);
        return;
    }

    {
        auto callback = std::exchange(options_changed_callback_, {});
        const auto restore_callback =
            gsl::finally([this, &callback] { options_changed_callback_ = std::move(callback); });
        player_.addOrRemoveComponent<endstone::core::InternalSuppressEnchantOptionsFlagComponent>(true);
        const auto clear_suppression = gsl::finally([this] {
            player_.addOrRemoveComponent<endstone::core::InternalSuppressEnchantOptionsFlagComponent>(false);
        });
        ENDSTONE_HOOK_CALL_ORIGINAL(&EnchantingContainerManagerModel::recalculateOptions, this);
    }

    const auto publish_options = gsl::finally([this] {
        if (options_changed_callback_) {
            options_changed_callback_(*this);
        }
        const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::PlayerEnchantOptions);
        static_cast<PlayerEnchantOptionsPacket &>(*packet).payload.options = enchant_options_;
        player_.sendNetworkPacket(*packet);
    });

    const auto input_container_it = containers_.find(std::string(ContainerCollectionNames::EnchantingInputContainer));
    if (input_container_it == containers_.end() || input_container_it->second == nullptr) {
        return;
    }
    const auto &input_container = input_container_it->second;
    const auto &input = input_container->getItemStack(0);
    if (input.isNull() || input.getCount() <= 0 || enchant_options_.size() != kOfferCount) {
        return;
    }

    endstone::PrepareItemEnchantEvent::Offers offers;
    for (std::size_t i = 0; i < offers.size(); ++i) {
        if (!getOffer(enchant_options_[i], offers[i])) {
            return;
        }
    }
    const auto original_offers = offers;

    EndstoneContainerModelInventory inventory{input_container};
    endstone::PrepareItemEnchantEvent event{
        inventory,
        player_.getEndstoneActor<endstone::core::EndstonePlayer>(),
        endstone::core::EndstoneBlock::at(player_.getDimensionBlockSource(), block_pos_),
        endstone::core::EndstoneItemStack::fromMinecraft(input),
        std::move(offers),
        static_cast<int>(
            EnchantUtils::getBookCasePositions(player_.getDimensionBlockSource(), static_cast<Vec3>(block_pos_))
                .size()),
    };
    server.getPluginManager().callEvent(event);

    if (event.isCancelled()) {
        enchant_options_.clear();
    }
    else if (!areSameOffers(event.getOffers(), original_offers)) {
        auto updated_options = enchant_options_;
        for (std::size_t i = 0; i < event.getOffers().size(); ++i) {
            if (!isSameOffer(event.getOffers()[i], original_offers[i]) &&
                !applyOffer(updated_options[i], event.getOffers()[i], original_offers[i])) {
                return;
            }
        }
        enchant_options_ = std::move(updated_options);
    }
}
