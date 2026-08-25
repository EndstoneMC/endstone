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

#include <functional>

class Actor;
class CompoundTag;

class InternalComponentRegistry {
public:
    struct ComponentInfo {
        std::function<void(const CompoundTag &, CompoundTag &)> legacy_data_conversion_func_;
        std::function<void(Actor &, const CompoundTag &)> create_and_load_component_func_;
        std::function<void(const Actor &, CompoundTag &)> save_component_func_;
    };
};
