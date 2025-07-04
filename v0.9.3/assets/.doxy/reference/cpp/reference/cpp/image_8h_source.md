

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
    enum class Type {
        Invalid = 0,
        Grayscale = 1,
        RGB = 3,
        RGBA = 4,
    };

    explicit Image(const Type type, const int width, const int height)
        : type_(type), width_(width), height_(height), data_(width * height * 4, '\0')
    {
    }

private:
    Image(const Type type, const int width, const int height, const std::string_view data)
        : type_(type), width_(width), height_(height), data_(data)
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

    [[nodiscard]] int getDepth() const
    {
        switch (type_) {
        case Type::Grayscale:
            return 1;
        case Type::RGB:
            return 3;
        case Type::RGBA:
            return 4;
        default:
            return 0;
        }
    }

    [[nodiscard]] Color getColor(const int x, const int y) const
    {
        const size_t idx = (static_cast<size_t>(y) * width_ + x) * 4;
        std::uint8_t r = 0, g = 0, b = 0, a = 255;
        switch (type_) {
        case Type::Grayscale:
            r = g = b = static_cast<uint8_t>(data_[idx]);
            break;
        case Type::RGB:
            r = static_cast<uint8_t>(data_[idx]);
            g = static_cast<uint8_t>(data_[idx + 1]);
            b = static_cast<uint8_t>(data_[idx + 2]);
            break;
        case Type::RGBA:
            r = static_cast<uint8_t>(data_[idx]);
            g = static_cast<uint8_t>(data_[idx + 1]);
            b = static_cast<uint8_t>(data_[idx + 2]);
            a = static_cast<uint8_t>(data_[idx + 3]);
            break;
        default:
            break;
        }
        return {r, g, b, a};
    }

    void setColor(const int x, const int y, const Color color)
    {
        const size_t idx = (static_cast<size_t>(y) * width_ + x) * 4;
        switch (type_) {
        case Type::Grayscale:
            data_[idx] = static_cast<char>(color.getRed());
            break;
        case Type::RGB:
            data_[idx] = static_cast<char>(color.getRed());
            data_[idx + 1] = static_cast<char>(color.getGreen());
            data_[idx + 2] = static_cast<char>(color.getBlue());
            break;
        case Type::RGBA:
            data_[idx] = static_cast<char>(color.getRed());
            data_[idx + 1] = static_cast<char>(color.getGreen());
            data_[idx + 2] = static_cast<char>(color.getBlue());
            data_[idx + 3] = static_cast<char>(color.getAlpha());
            break;
        default:
            return;
        }
    }

    [[nodiscard]] std::string_view getData() const
    {
        return data_;
    }

    static Result<Image> fromArray(Type type, const int width, const int height,
                                    const std::vector<unsigned char> &array)
    {
        const std::string_view buffer(reinterpret_cast<const char *>(array.data()), array.size());
        return fromBuffer(type, width, height, buffer);
    }

    static Result<Image> fromBuffer(Type type, const int width, const int height, std::string_view buffer)
    {
        size_t depth = 0;
        switch (type) {
        case Type::Grayscale:
            depth = 1;
            break;
        case Type::RGB:
            depth = 3;
            break;
        case Type::RGBA:
            depth = 4;
            break;
        default:
            return nonstd::make_unexpected("Image::fromBuffer: invalid image type");
        }

        ENDSTONE_CHECKF(buffer.size() == width * height * depth,
                        "Image::fromBuffer: invalid buffer size, got {} bytes, expected {} bytes.", buffer.size(),
                        width * height * depth);

        return Image(type, width, height, buffer);
    }

private:
    Type type_;
    int width_;
    int height_;
    std::string data_;
};
}  // namespace endstone
```


