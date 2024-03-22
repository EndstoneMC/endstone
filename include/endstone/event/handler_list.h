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

#include <map>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "endstone/event/event_handler.h"
#include "endstone/event/event_priority.h"

namespace endstone {

/**
 * A list of event handlers. Should be instantiated on a per event basis.
 */
class HandlerList {
public:
    explicit HandlerList(std::string event) : event_(std::move(event)) {}

    /**
     * Register a new handler
     *
     * @param handler Event handler to register
     * @return the pointer to the registered handler
     */
    EventHandler *registerHandler(std::unique_ptr<EventHandler> handler)
    {
        if (handler->getEventType() != event_) {
            return nullptr;
        }

        std::lock_guard lock(mtx_);
        valid_ = false;
        auto &vector =
            handlers_.emplace(handler->getPriority(), std::vector<std::unique_ptr<EventHandler>>{}).first->second;
        auto &it = vector.emplace_back(std::move(handler));
        return it.get();
    }

    /**
     * Remove a handler from a specific order slot
     *
     * @param handler Event handler to remove
     */
    void unregister(EventHandler &handler)
    {
        std::lock_guard lock(mtx_);
        auto &vector =
            handlers_.emplace(handler.getPriority(), std::vector<std::unique_ptr<EventHandler>>{}).first->second;
        auto it = std::find_if(vector.begin(), vector.end(),
                               [&](const std::unique_ptr<EventHandler> &h) { return h.get() == &handler; });
        if (it != vector.end()) {
            valid_ = false;
            vector.erase(it);
        }
    }

    /**
     * Remove a specific plugin's handlers from this handler
     *
     * @param plugin Plugin to remove
     */
    void unregister(Plugin &plugin)
    {
        std::lock_guard lock(mtx_);
        for (auto &[priority, vector] : handlers_) {
            vector.erase(
                std::remove_if(vector.begin(), vector.end(),
                               [&](const std::unique_ptr<EventHandler> &h) { return &h->getPlugin() == &plugin; }),
                vector.end());
            valid_ = false;
        }
    }

    /**
     * Get the baked registered handlers associated with this handler list
     *
     * @return the array of registered handlers
     */
    std::vector<EventHandler *> getHandlers() const
    {
        std::lock_guard lock(mtx_);
        if (!valid_) {
            bake();
        }
        return baked_handlers_;
    }

protected:
    void bake() const
    {
        if (valid_) {
            return;
        }

        baked_handlers_.clear();
        for (const auto &[priority, vector] : handlers_) {
            for (const auto &handler : vector) {
                baked_handlers_.push_back(handler.get());
            }
        }
        valid_ = true;
    }

private:
    mutable std::mutex mtx_;
    std::map<EventPriority, std::vector<std::unique_ptr<EventHandler>>> handlers_;
    mutable std::vector<EventHandler *> baked_handlers_;
    mutable bool valid_{false};
    std::string event_;
};

}  // namespace endstone
