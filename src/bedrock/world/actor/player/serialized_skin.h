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

#include <cstdint>
#include <string>

#include "bedrock/core/image/image.h"
#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/network/connection_request.h"
#include "bedrock/network/sub_client_connection_request.h"
#include "bedrock/platform/result.h"
#include "bedrock/util/thread_owner.h"
#include "bedrock/world/actor/player/persona_types.h"

enum class TrustedSkinFlag : char {
    Unset = 0,
    False = 1,
    True = 2,
};

namespace Data {  // NOLINTBEGIN
constexpr std::uint32_t MAX_SKIN_WIDTH = 4096;
constexpr std::uint32_t MAX_SKIN_HEIGHT = 4096;
constexpr std::uint32_t MAX_ANIMATED_SKIN_SIZE = 1000;  // NOLINTEND
}  // namespace Data

class AnimatedImageData {
public:
    AnimatedImageData();
    AnimatedImageData(const AnimatedImageData &);
    AnimatedImageData(persona::AnimatedTextureType, persona::AnimationExpression, const mce::Image &, float);
    AnimatedImageData &operator=(const AnimatedImageData &);
    persona::AnimatedTextureType type;
    persona::AnimationExpression animation_expression;
    mce::Image image;
    float frames;
};
static_assert(sizeof(AnimatedImageData) == 64);

class SerializedPersonaPieceHandle {
public:
    SerializedPersonaPieceHandle(const std::string &, persona::PieceType, mce::UUID, bool, const std::string &);
    std::string piece_id;
    persona::PieceType piece_type;
    mce::UUID pack_id;
    bool is_default_piece;
    std::string product_id;
};

class SerializedSkinImpl;
class SerializedSkinRef {
public:
    SerializedSkinRef();
    SerializedSkinRef(bool);
    SerializedSkinRef(const ConnectionRequest &);
    SerializedSkinRef(const SubClientConnectionRequest &);
    SerializedSkinRef(const std::string &, const std::string &, const std::string &, const mce::Image &, const uint16_t &,
                   const uint16_t &, const mce::Image &, const uint16_t &, const uint16_t &, Json::Value &&,
                   const MinEngineVersion &, const std::string &, const std::string &, bool, bool, bool, bool,
                   const bool, bool);

    [[nodiscard]] const mce::Image &getImageData() const;
    [[nodiscard]] const mce::Image &getCapeImageData() const;
    [[nodiscard]] bool getIsPersona() const;

    // Endstone begin
    [[nodiscard]] const std::string &getId() const;
    [[nodiscard]] const std::string &getCapeId() const;
    // Endstone end

private:
    std::shared_ptr<Bedrock::Application::ThreadOwner<SerializedSkinImpl>> skin_impl_;
};
static_assert(sizeof(SerializedSkinRef) == 16);
