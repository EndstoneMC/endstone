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

#include "bedrock/gamerefs/gamerefs_shareptr/owner_storage_shareptr.h"
#include "bedrock/gamerefs/gamerefs_shareptr/stack_result_storage_shareptr.h"
#include "bedrock/gamerefs/gamerefs_shareptr/weak_storage_shareptr.h"

template <typename T>
struct GameRefs {
    using OwnerStorage = OwnerStorageSharePtr<T>;
    using StackResultStorage = StackResultStorageSharePtr<T>;
    using WeakStorage = WeakStorageSharePtr<T>;
    using StackRef = T;
};
