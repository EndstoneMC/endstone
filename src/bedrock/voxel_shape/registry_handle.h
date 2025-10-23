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

namespace VoxelShapes {
class RegistryHandle {
    using value_type = std::uint16_t;

public:
    RegistryHandle();
    explicit RegistryHandle(value_type index);
    [[nodiscard]] bool isValid() const;
    explicit operator bool() const;
    [[nodiscard]] value_type value() const;
    bool operator==(const RegistryHandle &);

private:
    static constexpr value_type INVALID_VALUE = -1;
    value_type value_;
};
}  // namespace VoxelShapes
