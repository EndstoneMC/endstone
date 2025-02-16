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

namespace brstd {

struct no_value_t {};

struct no_mapped_container_t {};

template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
class associative_adapter {
public:
    static constexpr bool no_mapped_container = std::is_same_v<MappedContainer, no_mapped_container_t>;

private:
    using key_type = Key;
    using key_compare = Compare;
    using size_type = typename std::vector<key_type>::size_type;
    using key_container_type = KeyContainer;
    using mapped_container_type = MappedContainer;

public:
    struct containers {
        key_container_type keys;       // +0
        mapped_container_type values;  // +24
    };

    auto begin() noexcept
    {
        return containers_.keys.begin();
    }

    auto begin() const noexcept
    {
        return containers_.keys.begin();
    }

    auto end() noexcept
    {
        return containers_.keys.end();
    }

    auto end() const noexcept
    {
        return containers_.keys.end();
    }

private:
    containers containers_;  // +0
    key_compare compare_;    // +32
};

};  // namespace brstd
