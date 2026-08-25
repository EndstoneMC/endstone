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

#include <bitset>
#include <memory>
#include <memory>
#include <set>
#include <string>
#include <utility>

#include "bedrock/network/packet/boss_event_packet.h"
#include "endstone/boss/boss_bar.h"

namespace endstone::core {

class EndstoneBossBar : public BossBar {
public:
    EndstoneBossBar(std::string title, BarColor color, BarStyle style, const std::vector<BarFlag> &flags = {})
        : title_(std::move(title)), color_(color), style_(style)
    {
        for (auto const &flag : flags) {
            flags_.set(static_cast<int>(flag));
        }
    }

    [[nodiscard]] std::string getTitle() const override;
    void setTitle(std::string title) override;
    [[nodiscard]] BarColor getColor() const override;
    void setColor(BarColor color) override;
    [[nodiscard]] BarStyle getStyle() const override;
    void setStyle(BarStyle style) override;
    [[nodiscard]] bool hasFlag(BarFlag flag) const override;
    void addFlag(BarFlag flag) override;
    void removeFlag(BarFlag flag) override;
    [[nodiscard]] float getProgress() const override;
    void setProgress(float progress) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    void addPlayer(const NotNull<Player> &player) override;
    void removePlayer(const NotNull<Player> &player) override;
    void removeAll() override;
    [[nodiscard]] std::vector<NotNull<Player>> getPlayers() const override;

    void update(const NotNull<Player> &player);

private:
    void send(BossEventUpdateType event_type, const NotNull<Player> &player);
    void broadcast(BossEventUpdateType event_type);

    std::string title_;
    float progress_{1.0F};
    BarColor color_;
    BarStyle style_;
    std::bitset<2> flags_;
    bool visible_{true};
    mutable std::set<std::weak_ptr<Player>, std::owner_less<std::weak_ptr<Player>>> players_;
};

}  // namespace endstone::core
