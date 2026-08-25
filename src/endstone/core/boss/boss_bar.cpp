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

#include "endstone/core/boss/boss_bar.h"

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/boss_event_packet.h"
#include "endstone/check.h"
#include "endstone/core/player.h"

namespace endstone::core {

std::string EndstoneBossBar::getTitle() const
{
    return title_;
}

void EndstoneBossBar::setTitle(std::string title)
{
    if (title_ != title) {
        title_ = std::move(title);
        broadcast(BossEventUpdateType::UpdateName);
    }
}

BarColor EndstoneBossBar::getColor() const
{
    return color_;
}

void EndstoneBossBar::setColor(BarColor color)
{
    if (color_ != color) {
        color_ = color;
        broadcast(BossEventUpdateType::UpdateStyle);
    }
}

BarStyle EndstoneBossBar::getStyle() const
{
    return style_;
}

void EndstoneBossBar::setStyle(BarStyle style)
{
    if (style_ != style) {
        style_ = style;
        broadcast(BossEventUpdateType::UpdateStyle);
    }
}

bool EndstoneBossBar::hasFlag(BarFlag flag) const
{
    return flags_.test(static_cast<int>(flag));
}

void EndstoneBossBar::addFlag(BarFlag flag)
{
    if (!hasFlag(flag)) {
        flags_.set(static_cast<int>(flag));
        broadcast(BossEventUpdateType::UpdateProperties);
    }
}

void EndstoneBossBar::removeFlag(BarFlag flag)
{
    if (hasFlag(flag)) {
        flags_.reset(static_cast<int>(flag));
        broadcast(BossEventUpdateType::UpdateProperties);
    }
}

float EndstoneBossBar::getProgress() const
{
    return progress_;
}

void EndstoneBossBar::setProgress(float progress)
{
    Preconditions::checkArgument(progress >= 0.0 && progress <= 1.0, "Progress must be between 0.0 and 1.0");
    if (progress_ != progress) {
        progress_ = progress;
        broadcast(BossEventUpdateType::UpdatePercent);
    }
}

bool EndstoneBossBar::isVisible() const
{
    return visible_;
}

void EndstoneBossBar::setVisible(bool visible)
{
    if (visible_ != visible) {
        visible_ = visible;
        for (const auto &player : getPlayers()) {
            send(visible ? BossEventUpdateType::Add : BossEventUpdateType::Remove, player);
        }
    }
}

void EndstoneBossBar::addPlayer(const NotNull<Player> &player)
{
    if (players_.emplace(player.get()).second && visible_) {
        send(BossEventUpdateType::Add, player);
    }
}

void EndstoneBossBar::removePlayer(const NotNull<Player> &player)
{
    if (players_.erase(player.get()) > 0 && visible_) {
        send(BossEventUpdateType::Remove, player);
    }
}

void EndstoneBossBar::removeAll()
{
    for (const auto &player : getPlayers()) {
        removePlayer(player);
    }
    players_.clear();
}

std::vector<NotNull<Player>> EndstoneBossBar::getPlayers() const
{
    std::vector<NotNull<Player>> players;
    for (auto it = players_.begin(); it != players_.end();) {
        auto player = it->lock();
        if (player && static_cast<const EndstonePlayer *>(player.get())->tryGetHandle()) {
            players.emplace_back(std::move(player));
            ++it;
        }
        else {
            it = players_.erase(it);
        }
    }
    return players;
}

void EndstoneBossBar::update(const NotNull<Player> &player)
{
    if (visible_ && players_.contains(player.get())) {
        send(BossEventUpdateType::Add, player);
    }
}

void EndstoneBossBar::send(BossEventUpdateType event_type, const NotNull<Player> &player)
{
    const auto *handle = player.cast<EndstonePlayer>()->tryGetHandle();
    if (!handle) {
        return;
    }

    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::BossEvent);
    const auto pk = std::static_pointer_cast<BossEventPacket>(packet);
    pk->payload.boss_id = handle->getOrCreateUniqueID();
    pk->payload.player_id = ActorUniqueID(0);
    pk->payload.event_type = event_type;
    pk->payload.name = title_;
    pk->payload.health_percent = progress_;
    pk->payload.color = static_cast<BossBarColor>(color_);
    pk->payload.overlay = static_cast<BossBarOverlay>(style_);
    // BarFlag::DarkenSky / CreateFog dropped from BossEventPacket in BDS 1.26.32 (cereal-only migration)
    handle->sendNetworkPacket(*packet);
}

void EndstoneBossBar::broadcast(BossEventUpdateType event_type)
{
    if (!visible_) {
        return;
    }
    for (const auto &player : getPlayers()) {
        send(event_type, player);
    }
}

}  // namespace endstone::core
