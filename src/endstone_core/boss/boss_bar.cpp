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

#include "endstone/detail/boss/boss_bar.h"

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/boss_event_packet.h"
#include "endstone/detail/server.h"
#include "endstone/detail/util/error.h"

namespace endstone::detail {

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

Result<void> EndstoneBossBar::setProgress(float progress)
{
    if (progress < 0.0F || progress > 1.0F) {
        return nonstd::make_unexpected(make_error("Progress must be between 0.0 and 1.0"));
    }
    if (progress_ != progress) {
        progress_ = progress;
        broadcast(BossEventUpdateType::UpdatePercent);
    }
    return {};
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
            send(visible ? BossEventUpdateType::Add : BossEventUpdateType::Remove, *player);
        }
    }
}

void EndstoneBossBar::addPlayer(Player &player)
{
    players_.emplace(player.getUniqueId());
    if (visible_) {
        send(BossEventUpdateType::Add, player);
    }
}

void EndstoneBossBar::removePlayer(Player &player)
{
    players_.erase(player.getUniqueId());
    if (visible_) {
        send(BossEventUpdateType::Remove, player);
    }
}

void EndstoneBossBar::removeAll()
{
    for (const auto &player : getPlayers()) {
        removePlayer(*player);
    }
}

std::vector<Player *> EndstoneBossBar::getPlayers() const
{
    std::vector<Player *> players;
    const auto &server = entt::locator<EndstoneServer>::value();
    for (auto it = players_.begin(); it != players_.end();) {
        if (auto *player = server.getPlayer(*it); player) {
            players.emplace_back(player);
            ++it;
        }
        else {
            it = players_.erase(it);
        }
    }
    return players;
}

void EndstoneBossBar::send(BossEventUpdateType event_type, Player &player)
{
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::BossEvent);
    const auto pk = std::static_pointer_cast<BossEventPacket>(packet);
    const auto &handle = static_cast<EndstonePlayer &>(player).getHandle();
    pk->boss_id = handle.getOrCreateUniqueID();
    pk->player_id = handle.getOrCreateUniqueID();
    pk->event_type = event_type;
    pk->name = title_;
    pk->health_percent = progress_;
    pk->color = static_cast<BossBarColor>(color_);
    pk->overlay = static_cast<BossBarOverlay>(style_);
    pk->darken_screen = hasFlag(BarFlag::DarkenSky);
    handle.sendNetworkPacket(*packet);
}

void EndstoneBossBar::broadcast(BossEventUpdateType event_type)
{
    if (!visible_) {
        return;
    }
    for (const auto &player : getPlayers()) {
        send(event_type, *player);
    }
}

}  // namespace endstone::detail
