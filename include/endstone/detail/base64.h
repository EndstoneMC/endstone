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

#include <libbase64.h>

#include <string>
#include <string_view>

namespace endstone::detail {

template <class OutputBuffer, class InputIterator>
inline OutputBuffer base64_encode(InputIterator begin, InputIterator end)
{
    static OutputBuffer empty_buffer;
    if (begin == end) {
        return empty_buffer;
    }
    const std::size_t size = end - begin;
    const std::size_t buffer_size = (size / 3 + (size % 3 > 0)) << 2;
    OutputBuffer encoded(buffer_size, '=');
    const char *bytes = reinterpret_cast<const char *>(&*begin);
    char *out = reinterpret_cast<char *>(&encoded[0]);
    std::size_t encoded_size;
    ::base64_encode(bytes, size, out, &encoded_size, 0);
    return encoded;
}

template <class OutputBuffer>
inline OutputBuffer base64_encode(std::string_view data)
{
    return base64_encode<OutputBuffer>(std::begin(data), std::end(data));
}

inline std::string base64_encode(std::string_view data)
{
    return base64_encode<std::string>(std::begin(data), std::end(data));
}

}  // namespace endstone::detail
