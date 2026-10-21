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

#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "endstone/event/event_handler.h"
#include "endstone/event/event_priority.h"
#include "endstone/util/pointers.h"

namespace endstone {

/**
 * A list of event handlers. Should be instantiated on a per-event basis.
 */
class HandlerList {
public:
    explicit HandlerList(std::string event) : event_(std::move(event)) {}

    /**
     * Register a new handler.
     *
     * @param handler Event handler to register
     * @return The registered handler, or a null handle if it handles a different event
     */
    Nullable<EventHandler> registerHandler(const NotNull<EventHandler> &handler)
    {
        if (handler->getEventType() != event_) {
            return nullptr;
        }

        std::lock_guard lock(mtx_);
        valid_ = false;
        handlers_.emplace(handler->getPriority(), std::vector<NotNull<EventHandler>>{}).first->second.push_back(handler);
        return handler;
    }

    /**
     * Remove a handler from a specific order slot.
     *
     * @param handler Event handler to remove
     * @return The handlers this list no longer holds, for the caller to release once it holds no lock
     */
    [[nodiscard]] std::vector<NotNull<EventHandler>> unregister(const EventHandler &handler)
    {
        std::vector<NotNull<EventHandler>> removed;
        std::lock_guard lock(mtx_);
        const auto slot = handlers_.find(handler.getPriority());
        if (slot == handlers_.end()) {
            return removed;
        }
        auto &vector = slot->second;
        const auto it = std::find_if(vector.begin(), vector.end(),
                                     [&](const NotNull<EventHandler> &h) { return &*h == &handler; });
        if (it != vector.end()) {
            removed.push_back(*it);
            vector.erase(it);
            dropBaked(removed);
        }
        return removed;
    }

    /**
     * Remove a specific plugin's handlers from this handler.
     *
     * @param plugin Plugin to remove
     * @return The handlers this list no longer holds, for the caller to release once it holds no lock
     */
    [[nodiscard]] std::vector<NotNull<EventHandler>> unregister(const NotNull<Plugin> &plugin)
    {
        std::vector<NotNull<EventHandler>> removed;
        std::lock_guard lock(mtx_);
        for (auto &[priority, vector] : handlers_) {
            const auto first = std::stable_partition(
                vector.begin(), vector.end(), [&](const NotNull<EventHandler> &h) { return h->getPlugin() != plugin; });
            removed.insert(removed.end(), first, vector.end());
            vector.erase(first, vector.end());
        }
        if (!removed.empty()) {
            dropBaked(removed);
        }
        return removed;
    }

    /**
     * Checks whether this handler list has no registered handler.
     *
     * @return `true` if nothing is listening for this event
     */
    [[nodiscard]] bool empty() const
    {
        std::lock_guard lock(mtx_);
        if (!valid_) {
            bake();
        }
        return baked_handlers_.empty();
    }

    /**
     * Get the baked registered handlers associated with this handler list.
     *
     * @return the array of registered handlers
     */
    std::vector<NotNull<EventHandler>> getHandlers() const
    {
        std::lock_guard lock(mtx_);
        if (!valid_) {
            bake();
        }
        return baked_handlers_;
    }

protected:
    void dropBaked(std::vector<NotNull<EventHandler>> &removed)
    {
        removed.insert(removed.end(), baked_handlers_.begin(), baked_handlers_.end());
        baked_handlers_.clear();
        valid_ = false;
    }

    void bake() const
    {
        if (valid_) {
            return;
        }

        baked_handlers_.clear();
        for (const auto &[priority, vector] : handlers_) {
            for (const auto &handler : vector) {
                baked_handlers_.push_back(handler);
            }
        }
        valid_ = true;
    }

private:
    mutable std::mutex mtx_;
    std::map<EventPriority, std::vector<NotNull<EventHandler>>> handlers_;
    mutable std::vector<NotNull<EventHandler>> baked_handlers_;
    mutable bool valid_{false};
    std::string event_;
};

}  // namespace endstone
