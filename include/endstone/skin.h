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
#include <utility>

namespace endstone {

/**
 * @brief Represents a player skin.
 */
class Skin {
public:
    struct ImageData {
        int height;
        int width;
        std::string data;
    };

    Skin() = default;

    Skin(std::string skin_id, ImageData skin_data, std::optional<std::string> cape_id = std::nullopt,
         std::optional<ImageData> cape_data = std::nullopt)
        : skin_id_(std::move(skin_id)), skin_data_(std::move(skin_data)), cape_id_(std::move(cape_id)),
          cape_data_(std::move(cape_data))
    {
    }

    /**
     * @brief Gets the skin id.
     *
     * @return the skin id.
     */
    [[nodiscard]] const std::string &getSkinId() const
    {
        return skin_id_;
    }

    /**
     * @brief Gets the skin data.
     *
     * @return the skin data.
     */
    [[nodiscard]] const ImageData &getSkinData() const
    {
        return skin_data_;
    }

    /**
     * @brief Gets the cape id.
     *
     * @return the cape id.
     */
    [[nodiscard]] const std::optional<std::string> &getCapeId() const
    {
        return cape_id_;
    }

    /**
     * @brief Gets the cape data.
     *
     * @return the cape data.
     */
    [[nodiscard]] const std::optional<ImageData> &getCapeData() const
    {
        return cape_data_;
    }

private:
    std::string skin_id_;
    ImageData skin_data_;
    std::optional<std::string> cape_id_;
    std::optional<ImageData> cape_data_;
};

}  // namespace endstone
