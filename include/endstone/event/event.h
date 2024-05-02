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

#include <string>

namespace endstone {

/**
 * @brief Represents an event.
 */
class Event {
public:
    explicit Event(bool async = false) : async_(async){};
    Event(const Event &) = delete;             // deleted copy constructor
    Event &operator=(const Event &) = delete;  // deleted copy assignment operator

    virtual ~Event() = default;

    /**
     * Gets a user-friendly identifier for this event.
     *
     * @return name of this event
     */
    [[nodiscard]] virtual std::string getEventName() const = 0;

    /**
     * Whether the event can be cancelled by a plugin or the server.
     *
     * @return true if this event can be cancelled
     */
    [[nodiscard]] virtual bool isCancellable() const = 0;

    /**
     * Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still
     * pass to other plugins
     *
     * @return true if this event is cancelled
     */
    [[nodiscard]] bool isCancelled() const
    {
        if (!isCancellable()) {
            return false;
        }
        return cancelled_;
    };

    /**
     * Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still
     * pass to other plugins.
     *
     * @param cancel true if you wish to cancel this event
     */
    void setCancelled(bool cancel)
    {
        if (isCancellable()) {
            cancelled_ = cancel;
        }
    }

    /**
     * Any custom event that should not by synchronized with other events must use the specific constructor.
     *
     * @return false by default, true if the event fires asynchronously
     */
    [[nodiscard]] bool isAsynchronous() const
    {
        return async_;
    }

private:
    bool async_;
    bool cancelled_{false};
};

}  // namespace endstone
