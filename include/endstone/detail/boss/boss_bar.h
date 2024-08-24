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

#include <string>
#include <utility>

#include "endstone/boss/boss_bar.h"

namespace endstone::detail {

class EndstoneBossBar : public BossBar {
public:
    EndstoneBossBar(std::string title, BarColor color, BarStyle style)
        : title_(std::move(title)), color_(color), style_(style)
    {
    }

    [[nodiscard]] std::string getTitle() const override;
    void setTitle(const std::string &title) override;
    [[nodiscard]] BarColor getColor() const override;
    void setColor(const BarColor &color) override;
    [[nodiscard]] BarStyle getStyle() const override;
    void setStyle(const BarStyle &style) override;
    [[nodiscard]] float getProgress() const override;
    void setProgress(float progress) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool shouldDarkenScreen() const override;
    void setDarkenScreen(bool darken_screen) override;
    void addPlayer(Player &player) override;
    void removePlayer(Player &player) override;
    void removeAll() override;
    [[nodiscard]] std::vector<Player *> getPlayers() const override;

private:
    std::string title_;
    float progress_{1.0F};
    BarColor color_;
    BarStyle style_;
    bool darken_screen_{false};
    bool visible_{true};
    std::vector<Player *> players_;
};

}  // namespace endstone::detail
