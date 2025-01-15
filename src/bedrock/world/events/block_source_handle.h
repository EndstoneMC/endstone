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

#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/block_source_listener.h"

class BlockSourceHandle : public BlockSourceListener {
public:
    BlockSourceHandle() = delete;
    BlockSourceHandle(const BlockSourceHandle &) = delete;
    BlockSourceHandle &operator=(const BlockSourceHandle &) = delete;
    ~BlockSourceHandle() override = 0;
    void onSourceDestroyed(BlockSource &) override = 0;

    explicit operator bool() const
    {
        return source_ != nullptr;
    }

    BlockSource &operator*()
    {
        return *source_;
    }

    BlockSource *operator->()
    {
        return source_;
    }

private:
    BlockSource *source_;  // +8
};
