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

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/item/item_stack.h"

class ItemActor : public Actor {
public:
    static constexpr int LIFETIME = 6000;
    static constexpr int INFINITE_PICKUP_DELAY = 65535;

    ItemActor(ActorDefinitionGroup *, const ActorDefinitionIdentifier &, EntityContext &);
    void reloadHardcoded(ActorInitializationMethod, const VariantParameterList &) override = 0;
    ~ItemActor() override;
    virtual void postNormalTick();
    void playerTouch(Player &) override;
    std::unique_ptr<Packet> tryCreateAddActorPacket() override;
    ActorUniqueID getSourceUniqueID() const override;
    bool isInvulnerableTo(const ActorDamageSource &) const override;
    bool canSynchronizeNewEntity() const override;
    void handleEntityEvent(ActorEvent, int) override;
    bool isFireImmune() const override;

protected:
    bool _hurt(const ActorDamageSource &, float, bool, bool) override;
    virtual void addAdditionalSaveData(CompoundTag &) const;
    void readAdditionalSaveData(const CompoundTag &, DataLoadHelper &) override;

public:
    int getLifeTime() const;
    const ItemStack &getItemStack() const;
    ItemStack &getItemStack();
    void setItemStack(const ItemStack &);
    int getAge() const;
    void setBobOffset(float);
    float getBobOffset() const;
    int getTickCount() const;
    void setDefaultPickUpDelay();
    void setNoPickUpDelay();
    void setNeverPickUp();
    void setPickUpDelay(int);
    void setIsFromFishing(bool);
    bool hasPickUpDelay();
    bool isInItemFrame();
    bool isFromFishing() const;
    void setSourceEntity(const Actor *);
    static ItemActor *tryGetFromEntity(EntityContext &entity, bool include_removed = false);

    // Endstone begins
    int getPickUpDelay() const;
    void setLifeTime(int lifetime);
    // Endstone ends

    struct ItemRenderAdjustments {
        bool use_adjustments;
        float first_rendered_yaw;
    };

private:
    ItemStack item_;
    int age_;
    int pickup_delay_;
    int throw_time_;
    float bob_offs_;
    int health_;
    int lifetime_;
    bool is_in_item_frame_;
    bool is_from_fishing_;
    std::optional<ItemRenderAdjustments> render_adjustment_;
};
