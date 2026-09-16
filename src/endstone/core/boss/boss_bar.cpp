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

#include <mutex>
#include <unordered_set>
#include <vector>

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/boss_event_packet.h"
#include "endstone/check.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"

namespace endstone::core {

namespace {
// TODO(v0.12): drop once createBossBar returns a shared_ptr and the server can track bars by weak_ptr.
struct BossBarRegistry {
    std::mutex mutex;
    std::unordered_set<EndstoneBossBar *> bars;
};

BossBarRegistry &getBossBarRegistry()
{
    static auto *registry = new BossBarRegistry();
    return *registry;
}

struct BossBarState {
    std::string title;
    float progress;
    BarColor color;
    BarStyle style;
};

void sendBossEvent(const ::Player &handle, BossEventUpdateType event_type, const BossBarState &state)
{
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::BossEvent);
    const auto pk = std::static_pointer_cast<BossEventPacket>(packet);
    pk->payload.boss_id = handle.getOrCreateUniqueID();
    pk->payload.event_type = event_type;
    pk->payload.name = state.title;
    pk->payload.health_percent = state.progress;
    pk->payload.color = static_cast<BossBarColor>(state.color);
    pk->payload.overlay = static_cast<BossBarOverlay>(state.style);
    // BarFlag::DarkenSky / CreateFog dropped from BossEventPacket in BDS 1.26.32 (cereal-only migration)
    handle.sendNetworkPacket(*packet);
}
}  // namespace

EndstoneBossBar::EndstoneBossBar(std::string title, BarColor color, BarStyle style, const std::vector<BarFlag> &flags)
    : title_(std::move(title)), color_(color), style_(style)
{
    for (auto const &flag : flags) {
        flags_.set(static_cast<int>(flag));
    }
    auto &registry = getBossBarRegistry();
    std::lock_guard lock(registry.mutex);
    registry.bars.insert(this);
}

EndstoneBossBar::~EndstoneBossBar()
{
    auto &registry = getBossBarRegistry();
    std::lock_guard lock(registry.mutex);
    registry.bars.erase(this);
}

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
    const auto &server = EndstoneServer::getInstance();
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

void EndstoneBossBar::resend(Player &player)
{
    const auto uuid = player.getUniqueId();
    std::vector<BossBarState> states;
    {
        auto &registry = getBossBarRegistry();
        std::lock_guard lock(registry.mutex);
        for (const auto *bar : registry.bars) {
            if (bar->visible_ && bar->players_.contains(uuid)) {
                states.push_back({bar->title_, bar->progress_, bar->color_, bar->style_});
            }
        }
    }
    const auto &handle = static_cast<EndstonePlayer &>(player).getHandle();
    for (const auto &state : states) {
        sendBossEvent(handle, BossEventUpdateType::Add, state);
    }
}

void EndstoneBossBar::send(BossEventUpdateType event_type, Player &player)
{
    sendBossEvent(static_cast<EndstonePlayer &>(player).getHandle(), event_type, {title_, progress_, color_, style_});
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

}  // namespace endstone::core
