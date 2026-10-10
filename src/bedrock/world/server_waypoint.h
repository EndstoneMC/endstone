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

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/world/waypoint.h"

class ServerWaypoint : public Waypoint {
    enum class UpdateFlags : std::uint32_t {
        Position = 1,
        Visibility = 2,
        TexturePath = 4,
        IconSize = 8,
        Color = 16,
        ClientPositionAuthority = 32,
        All = 63,
    };

public:
    struct Payload {
        bool operator==(const Payload &) const;
        std::uint32_t update_flag;
        std::optional<bool> is_visible;
        std::optional<WorldPosition> world_position;
        std::optional<std::string> texture_path;
        std::optional<Vec2> icon_size;
        std::optional<mce::Color> color;
        std::optional<bool> client_position_authority;
        std::optional<ActorUniqueID> actor_id;
    };
    struct Texture {
        int lower_bound;
        std::optional<int> upper_bound;
        std::string texture_path;
        Vec2 icon_size;
    };
    struct TextureSelector {
        [[nodiscard]] std::optional<Texture> tryGetTexture(const Vec3 &viewing_pos, const Vec3 &observed_pos) const;
        std::vector<Texture> textures;
        static const std::size_t MAX_TEXTURES = 100;
    };
    ServerWaypoint(const TextureSelector &texture_selector, const std::optional<mce::Color> &color,
                   WorldPosition world_position);
    void setWorldPosition(const WorldPosition &world_position) override;
    void setColor(const std::optional<mce::Color> &color) override;
    void setTextureSelector(const TextureSelector &texture_selector);
    void setIsVisible(bool is_visible) override;
    void setClientPositionAuthority(bool client_position_authority) override;
    void setIsEnabled(bool is_enabled);
    void setTexturePath(const std::optional<std::string> &texture_path) override;
    void setIconSize(const Vec2 &icon_size) override;
    std::uint32_t consumeChanges();
    [[nodiscard]] std::uint32_t getUpdateFlags() const;
    Payload generatePayload(std::uint32_t update_flag);
    [[nodiscard]] virtual bool isValid() const;
    [[nodiscard]] virtual bool calculateIsVisible(const Player &viewing_player) const;
    ENDSTONE_HOOK void update(const Player &viewing_player) override;
    [[nodiscard]] std::optional<Texture> tryGetTexture(const Vec3 &, const Vec3 &) const;

private:
    bool is_enabled_;
    TextureSelector texture_selector_;
    std::uint32_t update_flags_;
};
BEDROCK_STATIC_ASSERT_SIZE(ServerWaypoint, 144, 128);
