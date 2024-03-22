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

/**
 * Represents an event's priority in execution.
 *
 * Listeners with lower priority are called first will listeners with higher priority are called last.
 * Listeners are called in following order:
 * LOWEST -> LOW -> NORMAL -> HIGH -> HIGHEST -> MONITOR
 */
enum class EventPriority {
    /**
     * Event call is of very low importance and should be run first, to allow other plugins to further customise the
     * outcome
     */
    Lowest = 0,
    /**
     * Event call is of low importance
     */
    Low = 1,
    /**
     * Event call is neither important nor unimportant, and may be run normally
     */
    Normal = 2,
    /**
     * Event call is of high importance
     */
    High = 3,
    /**
     * Event call is critical and must have the final say in what happens to the event
     */
    Highest = 4,
    /**
     * Event is listened to purely for monitoring the outcome of an event.
     * No modifications to the event should be made under this priority
     */
    Monitor = 5
};

}  // namespace endstone
