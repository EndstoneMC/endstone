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
class SerializedSkin {
public:
    SerializedSkin();
    SerializedSkin(bool);
    SerializedSkin(const SerializedSkin &);
    SerializedSkin(SerializedSkin &&);
    SerializedSkin(const ConnectionRequest &);
    SerializedSkin(const SubClientConnectionRequest &);
    SerializedSkin(const std::string &, const std::string &, const std::string &, const mce::Image &, const uint16_t &,
                   const uint16_t &, const mce::Image &, const uint16_t &, const uint16_t &, Json::Value &&,
                   const MinEngineVersion &, const std::string &, const std::string &, bool, bool, bool, bool,
                   const bool, bool);
    Bedrock::Result<void> read(ReadOnlyBinaryStream &);
    void write(BinaryStream &) const;
    void setGeometryData(const Json::Value &, const MinEngineVersion &);
    void setPersonaAppearanceData(const persona::ArmSize::Type &, const Color &,
                                  const std::vector<SerializedPersonaPieceHandle>,
                                  const std::unordered_map<persona::PieceType, TintMapColor> &);
    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] const std::string &getPlayFabId() const;
    [[nodiscard]] const std::string &getFullId() const;
    void setPlayFabId(const std::string &);
    void setAnimatedImageData(const std::vector<AnimatedImageData> &);
    [[nodiscard]] const std::string &getResourcePatch() const;
    void setResourcePatch(const std::string &);
    [[nodiscard]] const std::string &getDefaultGeometryName() const;
    mce::Image &getImageData();
    mce::Image &getCapeImageData();
    [[nodiscard]] const mce::Image &getImageData() const;
    mce::Image &getImageDataMutable();
    [[nodiscard]] const mce::Image &getCapeImageData() const;
    std::vector<AnimatedImageData> &getAnimatedImageData();
    [[nodiscard]] const std::vector<AnimatedImageData> &getAnimatedImageData() const;
    [[nodiscard]] float getAnimationFrames(persona::AnimatedTextureType) const;
    [[nodiscard]] bool useBlinkingAnimation() const;
    [[nodiscard]] const Json::Value &getGeometryData() const;
    [[nodiscard]] const MinEngineVersion &getGeometryDataMinEngineVersion() const;
    [[nodiscard]] Json::Value &getGeometryDataMutable() const;
    [[nodiscard]] const std::vector<SerializedPersonaPieceHandle> &getPersonaPieces() const;
    [[nodiscard]] const std::unordered_map<persona::PieceType, TintMapColor> &getPieceTintColors() const;
    [[nodiscard]] const persona::ArmSize::Type &getArmSize() const;
    [[nodiscard]] std::string getArmSizeAsString() const;
    void setArmSizeFromString(const std::string &);
    [[nodiscard]] const Color &getSkinColor() const;
    [[nodiscard]] std::string getGeometryDataStr() const;
    [[nodiscard]] const std::string &getAnimationData() const;
    [[nodiscard]] bool getIsPremium() const;
    [[nodiscard]] bool getIsPersona() const;
    [[nodiscard]] bool getIsPersonaCapeOnClassicSkin() const;
    void setIsPersonaCapeOnClassicSkin(bool);
    void setImageData(const mce::Image *);
    void setCapeImageData(const mce::Image *);
    void setCapeId(const std::string &);
    [[nodiscard]] bool isTrustedSkin() const;
    void setIsTrustedSkin(bool);
    [[nodiscard]] bool isPrimaryUser() const;
    void setIsPrimaryUser(const bool);
    void updateGeometryName();
    [[nodiscard]] bool isValid() const;
    [[nodiscard]] bool overridesPlayerAppearance() const;
    static bool isSkinDisabled(const SerializedSkin &, const LevelData &);
    void releaseThread();
    void clear();

private:
    std::shared_ptr<Bedrock::Application::ThreadOwner<SerializedSkinImpl>> skin_impl_;
};
static_assert(sizeof(SerializedSkin) == 16);
