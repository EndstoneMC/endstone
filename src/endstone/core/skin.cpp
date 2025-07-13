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

#include "endstone/core/skin.h"

namespace endstone::core {

Skin EndstoneSkin::fromMinecraft(const SerializedSkin &skin)
{
    const auto &data = skin.getImageData();
    auto image = Image::fromBuffer(
        Image::Type::RGBA, data.width, data.height,
        std::string_view{reinterpret_cast<const char *>(data.image_bytes.data()), data.image_bytes.size()});

    std::optional<std::string> cape_id;
    if (!skin.getCapeId().empty()) {
        cape_id = skin.getCapeId();
    }

    const auto &cape_data = skin.getCapeImageData();
    std::optional<Image> cape_image = std::nullopt;
    if (!cape_data.image_bytes.empty()) {
        auto result = Image::fromBuffer(Image::Type::RGBA, cape_data.width, cape_data.height,
                                        std::string_view{reinterpret_cast<const char *>(cape_data.image_bytes.data()),
                                                         cape_data.image_bytes.size()});
        if (result.has_value()) {
            cape_image = result.value();
        }
    }

    return {skin.getId(), image.value(), cape_id, cape_image};
}
}  // namespace endstone::core
