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

namespace endstone {

class ICancellable {
public:
    virtual ~ICancellable() = default;
    [[nodiscard]] virtual bool isCancelled() const = 0;
    virtual void setCancelled(bool cancel) = 0;
    virtual void cancel() = 0;
};

/**
 * @brief A type characterizing events that may be cancelled by a plugin or the server.
 */
template <typename EventType>
class Cancellable : public EventType, public ICancellable {
public:
    using EventType::EventType;

    /**
     * @brief Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will
     * still pass to other plugins
     *
     * @return true if this event is cancelled
     */
    [[nodiscard]] bool isCancelled() const final
    {
        return EventType::cancelled_;
    }

    /**
     * @brief Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will
     * still pass to other plugins.
     *
     * @param cancel true if you wish to cancel this event
     */
    void setCancelled(bool cancel) final
    {
        EventType::cancelled_ = cancel;
    }

    /**
     * @brief Cancel this event. A cancelled event will not be executed in the server, but will still pass to other
     * plugins.
     */
    void cancel() final
    {
        setCancelled(true);
    }

private:
    [[nodiscard]] bool isCancellable() const final
    {
        return true;
    }
};
}  // namespace endstone
