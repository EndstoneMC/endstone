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

#include <functional>
#include <thread>
#include <vector>

#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/forward.h"
#include "bedrock/world/events/event_result.h"

template <typename ListenerType>
class EventCoordinatorPimpl {
public:
    virtual ~EventCoordinatorPimpl() = default;

protected:
    std::vector<ListenerType *> listeners_;                                      // +8
    std::vector<std::function<EventResult(ListenerType &)>> events_to_process_;  // +32
    std::vector<ListenerType *> pending_registrations_;                          // +56
    bool has_pending_registrations_;                                             // +80

private:
    std::thread::id thread_id_;        // +84
    bool thread_id_initialized_;       //
    std::int32_t thread_check_index_;  //
};

template <typename ListenerType>
class EventCoordinator : public EventCoordinatorPimpl<ListenerType> {};
