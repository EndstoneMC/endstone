

# File image.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**util**](dir_89b85071337bf933dea6c29b4c6a4410.md) **>** [**image.h**](image_8h.md)

[Go to the documentation of this file](image_8h.md)


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

#include <string>

#include "endstone/util/color.h"

namespace endstone {
class Image {
public:
    Image(const int width, const int height) : width_(width), height_(height), data_(width * height * 4, '\0') {}

private:
    Image(const int width, const int height, const std::string_view data) : width_(width), height_(height), data_(data)
    {
    }

public:
    [[nodiscard]] int getWidth() const
    {
        return width_;
    }

    [[nodiscard]] int getHeight() const
    {
        return height_;
    }

    [[nodiscard]] Color getColor(const int x, const int y) const
    {
        const size_t idx = (static_cast<size_t>(y) * width_ + x) * 4;
        return Color(static_cast<uint8_t>(data_[idx]), static_cast<uint8_t>(data_[idx + 1]),
                     static_cast<uint8_t>(data_[idx + 2]), static_cast<uint8_t>(data_[idx + 3]));
    }

    void setColor(const int x, const int y, const Color color)
    {
        const size_t idx = (static_cast<size_t>(y) * width_ + x) * 4;
        data_[idx + 0] = static_cast<char>(color.getRed());
        data_[idx + 1] = static_cast<char>(color.getGreen());
        data_[idx + 2] = static_cast<char>(color.getBlue());
        data_[idx + 3] = static_cast<char>(color.getAlpha());
    }

    [[nodiscard]] std::string_view getData() const
    {
        return data_;
    }

    static Result<Image> fromBuffer(const int width, const int height, std::string_view buffer)
    {
        ENDSTONE_CHECK(width > 0 && height > 0, "Width and height must be positive");
        ENDSTONE_CHECKF(buffer.size() == width * height * 4,
                        "Buffer size {} does not match expected size {} (width * height * 4)", buffer.size(),
                        width * height * 4);
        return Image(width, height, buffer);
    }

private:
    int width_;
    int height_;
    std::string data_;
};
}  // namespace endstone
```


