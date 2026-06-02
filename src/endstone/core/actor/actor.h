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

#include "bedrock/entity/components/post_tick_position_delta_component.h"
#include "bedrock/entity/weak_entity_ref.h"
#include "bedrock/server/commands/command_utils.h"
#include "bedrock/world/actor/provider/actor_offset.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/actor/actor.h"
#include "endstone/core/level/dimension.h"
#include "endstone/core/permissions/permissible_base.h"
#include "endstone/core/server.h"
#include "endstone/core/type.h"

class Actor;

namespace endstone::core {

class EndstoneLevel;
class EndstoneServer;
class EndstoneActor;

struct EndstoneActorComponent {
    std::shared_ptr<Actor> actor;
};

class ActorPermissibleBase {
public:
    static PermissibleBase &get();
};

template <typename Interface, typename Handle>
    requires std::is_base_of_v<Actor, Interface> && std::is_base_of_v<::Actor, Handle>
class EndstoneActorBase : public Interface {
public:
    EndstoneActorBase(EndstoneServer &server, Handle &handle) : server_(server), handle_(handle.getWeakEntity())
    {
        ActorPermissibleBase::get();
    }

    // Permissible
    [[nodiscard]] PermissionLevel getPermissionLevel() const override
    {
        return ActorPermissibleBase::get().getPermissionLevel();
    }

    [[nodiscard]] bool isPermissionSet(std::string name) const override
    {
        return ActorPermissibleBase::get().isPermissionSet(name);
    }

    [[nodiscard]] bool isPermissionSet(const Permission &perm) const override
    {
        return ActorPermissibleBase::get().isPermissionSet(perm);
    }

    [[nodiscard]] bool hasPermission(std::string name) const override
    {
        return ActorPermissibleBase::get().hasPermission(name);
    }

    [[nodiscard]] bool hasPermission(const Permission &perm) const override
    {
        return ActorPermissibleBase::get().hasPermission(perm);
    }

    PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name, bool value) override
    {
        return ActorPermissibleBase::get().addAttachment(plugin, name, value);
    }

    PermissionAttachment *addAttachment(Plugin &plugin) override
    {
        return ActorPermissibleBase::get().addAttachment(plugin);
    }

    bool removeAttachment(PermissionAttachment &attachment) override
    {
        return ActorPermissibleBase::get().removeAttachment(attachment);
    }

    void recalculatePermissions() override { ActorPermissibleBase::get().recalculatePermissions(); }

    [[nodiscard]] std::unordered_set<PermissionAttachmentInfo *> getEffectivePermissions() const override
    {
        return ActorPermissibleBase::get().getEffectivePermissions();
    }

    // Object
    [[nodiscard]] const std::type_info &getClassTypeId() const override { return typeid(Interface); }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return core::isInstanceOf(*this, target);
    }

    // CommandSender
    void sendMessage(const Message &message) const override {}

    void sendErrorMessage(const Message &message) const override {}

    [[nodiscard]] Server &getServer() const override { return server_; }

    [[nodiscard]] std::string getName() const override { return CommandUtils::getActorName(getHandle()); }

    // Actor
    [[nodiscard]] const ActorType &getType() const override
    {
        const auto &canonical = getHandle().getActorIdentifier().getCanonicalName();
        return server_.getRegistry<ActorType>().getOrThrow(ActorTypeId{canonical});
    }

    [[nodiscard]] std::uint64_t getRuntimeId() const override { return getHandle().getRuntimeID().raw_id; }

    [[nodiscard]] Location getLocation() const override
    {
        auto [x, y, z] = getHandle().getPosition();
        y -= ActorOffset::getHeightOffset(getHandle().getEntity());
        const auto &[pitch, yaw] = getHandle().getRotation();
        return {getDimension(), x, y, z, pitch, yaw};
    }

    [[nodiscard]] Vector getVelocity() const override
    {
        if (getHandle().hasCategory(ActorCategory::Mob) || getHandle().hasCategory(ActorCategory::Ridable)) {
            auto *vehicle = getHandle().getVehicle();
            if (!vehicle) {
                vehicle = &getHandle();
            }
            if (auto *component = vehicle->template tryGetComponent<PostTickPositionDeltaComponent>()) {
                const auto &delta = component->value;
                return {delta.x, delta.y, delta.z};
            }
        }
        const auto &delta = getHandle().getPosDelta();
        return {delta.x, delta.y, delta.z};
    }

    [[nodiscard]] bool isOnGround() const override { return getHandle().isOnGround(); }

    [[nodiscard]] bool isInWater() const override { return getHandle().isInWater(); }

    [[nodiscard]] bool isInLava() const override { return getHandle().isInLava(); }

    [[nodiscard]] Level &getLevel() const override
    {
        {
            return *server_.getLevel();
        }
    }

    [[nodiscard]] Dimension &getDimension() const override
    {
        return *server_.getEndstoneLevel()->getDimension(getHandle().getDimension().getDimensionId());
    }

    void setRotation(float yaw, float pitch) override { getHandle().setRotationWrapped({pitch, yaw}); }

    bool teleport(const Location &location) override
    {
        if (getHandle().isRemoved()) {
            return false;
        }

        setRotation(location.getYaw(), location.getPitch());
        Vec3 to_location{location.getX(), location.getY(), location.getZ()};
        if (&location.getDimension() != &getDimension()) {
            const auto to_dimension =
                static_cast<EndstoneDimension &>(location.getDimension()).getHandle().getDimensionId();
            getHandle().getLevel().entityChangeDimension(getHandle(), to_dimension, to_location);
        }
        else {
            getHandle().teleportTo(to_location, true, 3, 1, false);
        }
        return true;
    }

    bool teleport(const Actor &target) override { return teleport(target.getLocation()); }

    [[nodiscard]] std::int64_t getId() const override { return getHandle().getOrCreateUniqueID().raw_id; }

    void remove() override { getHandle().remove(); }

    [[nodiscard]] bool isDead() const override { return !getHandle().isAlive(); }

    [[nodiscard]] bool isValid() const override
    {
        const auto *handle = handle_.tryUnwrap<Handle>();
        if (!handle) {
            return false;
        }
        return handle->isAlive();
    }

    [[nodiscard]] std::vector<std::string> getScoreboardTags() const override { return getHandle().getTags(); }

    [[nodiscard]] bool addScoreboardTag(std::string tag) const override { return getHandle().addTag(tag); }

    [[nodiscard]] bool removeScoreboardTag(std::string tag) const override { return getHandle().removeTag(tag); }

    [[nodiscard]] bool isNameTagVisible() const override { return getHandle().canShowNameTag(); }

    void setNameTagVisible(bool visible) override { getHandle().setNameTagVisible(visible); }

    [[nodiscard]] bool isNameTagAlwaysVisible() const override
    {
        return getHandle().entity_data.getInt8(static_cast<SynchedActorData::ID>(ActorDataIDs::NAMETAG_ALWAYS_SHOW)) !=
               0;
    }

    void setNameTagAlwaysVisible(bool visible) override
    {
        getHandle().entity_data.template set<SynchedActorData::TypeInt8>(
            static_cast<SynchedActorData::ID>(ActorDataIDs::NAMETAG_ALWAYS_SHOW), visible);
    }

    [[nodiscard]] std::string getNameTag() const override { return getHandle().getNameTag(); }

    void setNameTag(std::string name) override { getHandle().setNameTag(name); }

    [[nodiscard]] std::string getScoreTag() const override { return getHandle().getScoreTag(); }

    void setScoreTag(std::string score) override { getHandle().setScoreTag(score); }

    Handle &getHandle() const
    {
        auto *ptr = handle_.tryUnwrap<Handle>(/*include_removed*/ true);
        if (!ptr) {
            throw std::runtime_error("Trying to access an actor that is no longer valid.");
        }
        return *ptr;
    }

protected:
    EndstoneServer &server_;

private:
    WeakEntityRef handle_;
};

class EndstoneActor : public EndstoneActorBase<Actor, ::Actor> {
public:
    using EndstoneActorBase::EndstoneActorBase;
};
}  // namespace endstone::core
