

# File skin.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**skin.h**](skin_8h.md)

[Go to the documentation of this file](skin_8h.md)


```C++
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

#include "endstone/util/image.h"

namespace endstone {

class Skin {
public:
    Skin(std::string id, Image image, std::optional<std::string> cape_id = std::nullopt,
         std::optional<Image> cape_image = std::nullopt)
        : id_(std::move(id)), image_(std::move(image)), cape_id_(std::move(cape_id)), cape_image_(std::move(cape_image))
    {
    }

    [[nodiscard]] const std::string &getId() const
    {
        return id_;
    }

    [[nodiscard]] const Image &getImage() const
    {
        return image_;
    }

    [[nodiscard]] const std::optional<std::string> &getCapeId() const
    {
        return cape_id_;
    }

    [[nodiscard]] const Image *getCapeImage() const
    {
        return cape_image_.has_value() ? &cape_image_.value() : nullptr;
    }

private:
    std::string id_;
    Image image_;
    std::optional<std::string> cape_id_;
    std::optional<Image> cape_image_;
};

}  // namespace endstone
```


