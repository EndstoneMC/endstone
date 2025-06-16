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

#include "bedrock/crypto/encoding/base64.h"

#include <libbase64.h>

namespace Util {
std::string base64_decode(const std::string &input)
{
    std::string output;
    output.reserve(input.size() * 3 / 4);
    std::size_t decoded_size;
    ::base64_decode(input.data(), input.size(), output.data(), &decoded_size, 0);
    output.resize(decoded_size);
    return output;
}
}  // namespace Util
