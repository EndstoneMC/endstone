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

#include "bedrock/util/index_set.h"

std::vector<std::uint64_t> const &IndexSet::getPacked() const
{
    return packed_;
}

bool IndexSet::contains(std::size_t index) const
{
    if (index < sparse_.size()) {
        const auto packed_id = sparse_[index];
        if (packed_id < packed_.size() && packed_.at(packed_id) == index) {
            return true;
        }
    }
    return false;
}

void IndexSet::insert(std::size_t index)
{
    auto next_packed_id = packed_.size();
    packed_.emplace_back(index);
    if (sparse_.size() <= index) {
        sparse_.resize(index + 1);
    }
    sparse_[index] = next_packed_id;
}

bool IndexSet::operator==(const IndexSet &other) const
{
    if (packed_.size() != other.packed_.size()) {
        return false;
    }

    if (packed_.empty()) {
        return true;
    }

    for (std::uint64_t index : packed_) {
        if (index >= other.sparse_.size()) {
            return false;
        }
        auto packed_id = other.sparse_[index];
        if (packed_id >= other.packed_.size() || other.packed_[packed_id] != index) {
            return false;
        }
    }
    return true;
}
