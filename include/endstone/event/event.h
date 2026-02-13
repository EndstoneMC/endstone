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

#include <stdexcept>
#include <string>

namespace endstone {

enum class EventResult {
    /**
     * Deny the event.
     */
    Deny,
    /**
     * Neither deny nor allow the event. The server will proceed with its normal handling.
     */
    Default,
    /**
     * Allow / Force the event.
     */
    Allow
};

/**
 * @brief Represents an event.
 */
class Event {
public:
    explicit Event(bool async = false) : async_(async) {};
    Event(const Event &) = delete;
    Event &operator=(const Event &) = delete;
    Event(Event &&) = default;
    Event &operator=(Event &&) = default;

    virtual ~Event() = default;

    /**
     * Gets a user-friendly identifier for this event.
     *
     * @return name of this event
     */
    [[nodiscard]] virtual std::string getEventName() const = 0;

    /**
     * Any custom event that should not by synchronized with other events must use the specific constructor.
     *
     * @return false by default, true if the event fires asynchronously
     */
    [[nodiscard]] bool isAsynchronous() const { return async_; }

private:
    [[nodiscard]] virtual bool isCancellable() const { return false; }

    template <class T>
    friend class Cancellable;
    friend class EventHandler;

    bool async_;
    bool cancelled_{false};
};

#define ENDSTONE_EVENT(type)                                \
public:                                                     \
    inline static constexpr auto NAME = #type;              \
    [[nodiscard]] std::string getEventName() const override \
    {                                                       \
        return NAME;                                        \
    }

}  // namespace endstone
