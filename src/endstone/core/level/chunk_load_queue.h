// Copyright (c) 2026, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <algorithm>
#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "bedrock/platform/uuid.h"
#include "endstone/level/chunk.h"
#include "endstone/plugin/plugin.h"

namespace endstone::core {

class ChunkLoadQueue {
public:
    struct Request {
        int x;
        int z;
        Plugin *plugin;
        std::function<void(Nullable<Chunk>)> callback;
        bool generate;
        std::uint64_t remaining_ticks;
        mce::UUID area_id = mce::UUID::EMPTY;
    };

    void add(std::shared_ptr<Request> request) { requests_.push_back(std::move(request)); }

    template <typename Poll, typename Release>
    void process(Poll &&poll, Release &&release, Logger &logger)
    {
        const auto requests = requests_;
        for (const auto &request : requests) {
            if (!request->callback) {
                continue;
            }
            Nullable<Chunk> chunk;
            try {
                if (const auto result = poll(*request)) {
                    chunk = *result;
                }
                else if (--request->remaining_ticks > 0) {
                    continue;
                }
            }
            catch (const std::exception &e) {
                logger.error("Could not load chunk ({}, {}): {}", request->x, request->z, e.what());
            }
            const auto plugin_name = request->plugin->getName();
            auto callback = std::exchange(request->callback, {});
            std::erase(requests_, request);
            try {
                callback(std::move(chunk));
            }
            catch (const std::exception &e) {
                logger.error("Could not execute chunk load callback for {}: {}", plugin_name, e.what());
            }
            catch (...) {
                logger.error("Could not execute chunk load callback for {}.", plugin_name);
            }
            if (request->area_id != mce::UUID::EMPTY) {
                release(std::exchange(request->area_id, mce::UUID::EMPTY));
            }
        }
    }

    template <typename Release>
    void cancel(Plugin &plugin, Release &&release)
    {
        std::vector<mce::UUID> released;
        for (const auto &request : requests_) {
            if (request->plugin != &plugin) {
                continue;
            }
            request->callback = {};
            if (request->area_id != mce::UUID::EMPTY) {
                released.push_back(std::exchange(request->area_id, mce::UUID::EMPTY));
            }
        }
        std::erase_if(requests_, [](const auto &request) { return !request->callback; });
        for (const auto &area_id : released) {
            release(area_id);
        }
    }

private:
    std::vector<std::shared_ptr<Request>> requests_;
};

}  // namespace endstone::core
