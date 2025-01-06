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

#include <memory>
#include <unordered_set>

#include <gsl/gsl>

#include "bedrock/core/file/path.h"
#include "bedrock/resources/preload_cache.h"

class PackAccessStrategy {
public:
    virtual ~PackAccessStrategy() = 0;

private:
    bool asset_set_populated_;
    std::unordered_set<Core::Path> asset_set_;
    gsl::not_null<std::shared_ptr<Bedrock::Resources::PreloadCache>> preloaded_;
};
