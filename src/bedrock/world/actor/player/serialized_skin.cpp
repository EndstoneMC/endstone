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

#include "bedrock/world/actor/player/serialized_skin.h"

#include "bedrock/core/image/image.h"
#include "bedrock/forward.h"
#include "bedrock/world/actor/player/persona_types.h"

class SerializedSkinImpl {
public:
    SerializedSkinImpl();
    SerializedSkinImpl(bool);
    SerializedSkinImpl(const ConnectionRequest &);
    SerializedSkinImpl(const SubClientConnectionRequest &);

    [[nodiscard]] const mce::Image &getImageData() const
    {
        return skin_image_;
    }

    [[nodiscard]] const mce::Image &getCapeImageData() const
    {
        return cape_image_;
    }

    // Endstone begin
    [[nodiscard]] const std::string &getId() const
    {
        return id_;
    }

    [[nodiscard]] const std::string &getCapeId() const
    {
        return cape_id_;
    }
    // Endstone end

private:
    std::string id_;
    std::string play_fab_id_;
    std::string full_id_;
    std::string resource_patch_;
    std::string default_geometry_name_;
    mce::Image skin_image_;
    mce::Image cape_image_;
    std::vector<AnimatedImageData> skin_animated_images_;
    Json::Value geometry_data_;
    MinEngineVersion geometry_data_min_engine_version_;
    Json::Value geometry_data_mutable_;
    std::string animation_data_;
    std::string cape_id_;
    std::vector<SerializedPersonaPieceHandle> persona_pieces_;
    persona::ArmSize::Type arm_size_type_;
    std::unordered_map<persona::PieceType, TintMapColor> piece_tint_colors_;
    Color skin_color_;
    TrustedSkinFlag is_trusted_skin_;
    bool is_premium_;
    bool is_persona_;
    bool is_persona_cape_on_classic_skin_;
    bool is_primary_user_;
    bool overrides_player_appearance_;
};

const mce::Image &SerializedSkin::getImageData() const
{
    return skin_impl_->get().getImageData();
}

const mce::Image &SerializedSkin::getCapeImageData() const
{
    return skin_impl_->get().getCapeImageData();
}

const std::string &SerializedSkin::getId() const
{
    return skin_impl_->get().getId();
}

const std::string &SerializedSkin::getCapeId() const
{
    return skin_impl_->get().getCapeId();
}
