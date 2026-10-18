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

#include <optional>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/color.h"
#include "bedrock/core/math/vec2.h"
#include "bedrock/shared_types/world/level/world_position.h"
#include "bedrock/world/actor/actor_unique_id.h"

class Player;

class Waypoint {
protected:
    Waypoint();
    Waypoint(const std::optional<mce::Color> &color, WorldPosition world_position);

public:
    virtual ~Waypoint();
    virtual void setWorldPosition(const WorldPosition &world_position);
    [[nodiscard]] const WorldPosition &getWorldPosition() const;
    virtual void setColor(const std::optional<mce::Color> &color);
    [[nodiscard]] const std::optional<mce::Color> &getColor() const;
    virtual void setIsVisible(bool is_visible);
    [[nodiscard]] bool isVisible() const;
    virtual void setClientPositionAuthority(bool client_position_authority);
    [[nodiscard]] bool getClientPositionAuthority() const;
    virtual void setTexturePath(const std::optional<std::string> &texture_path);
    [[nodiscard]] const std::optional<std::string> &getTexturePath() const;
    virtual void setIconSize(const Vec2 &icon_size);
    [[nodiscard]] const Vec2 &getIconSize() const;
    [[nodiscard]] virtual std::optional<ActorUniqueID> tryGetActorID() const;
    virtual void update(const Player &);

protected:
    WorldPosition world_pos_;
    std::optional<mce::Color> color_;
    std::optional<std::string> texture_path_;
    Vec2 icon_size_;
    bool is_visible_;
    bool client_position_authority_;
};
BEDROCK_STATIC_ASSERT_SIZE(Waypoint, 104, 96);
