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

#include <vector>

#include "bedrock/platform/brstd/associative_adapter.h"

namespace brstd {
template <typename Key, typename T, typename Compare = std::less<Key>, typename KeyContainer = std::vector<Key>,
          typename MappedContainer = std::vector<T>>
class flat_map : public associative_adapter<Key, T, Compare, KeyContainer, MappedContainer> {};

static_assert(flat_map<int, int>::no_mapped_container == false);
};  // namespace brstd
