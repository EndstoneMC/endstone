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

#include "bedrock/forward.h"
#include "bedrock/platform/threading/mutex_details.h"

namespace Core {
class StorageAreaStateListener {
public:
    virtual ~StorageAreaStateListener() = 0;

private:
    std::shared_ptr<Core::StorageAreaState> file_storage_area_;  // +8
    Bedrock::Threading::Mutex mutex_;                            // +24
};
}  // namespace Core
