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

#include <unordered_map>

#include "bedrock/platform/threading/mutex_details.h"

namespace Core {

template <class TKey, typename TValue, typename TValuePtrIn = std::add_pointer_t<TValue>, typename... TNewInstanceArgs>
class Cache {
public:
    using TValuePtr = TValuePtrIn;

    template <typename Factory>
    TValuePtr get(const TKey &key, const Factory &instance_factory)
    {
        // try get if the key already exists
        {
            std::lock_guard lock{access_};
            auto it = content_.find(key);
            if (it != content_.end()) {
                return &it->second;
            }
        }
        // emplace when it does not exist
        std::lock_guard lock{access_};
        return &content_.emplace(key, instance_factory()).first.second;
    }

private:
    Bedrock::Threading::SharedMutex access_;
    std::unordered_map<TKey, TValue> content_;
};

}  // namespace Core
