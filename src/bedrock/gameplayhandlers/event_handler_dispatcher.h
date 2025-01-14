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

#include <type_traits>

#include "bedrock/world/events/event_variant.h"
#include <entt/entt.hpp>

namespace Details {
template <typename EventHandlerInterface>
struct EventHandlerDispatcher {
    template <typename EventVariant>
    struct HandleEventResult {
        using type =
            std::invoke_result_t<decltype(&EventHandlerInterface::handleEvent), EventHandlerInterface, EventVariant>;
    };

    template <typename EventVariants>
    struct Impl;

    template <typename Base, typename Result, typename Events>
    struct AddHandleEvent;

    template <>
    struct Impl<entt::type_list<>> : EventHandlerInterface {};

    template <typename Result>
    struct AddHandleEvent<Impl<entt::type_list<>>, Result, entt::type_list<>> : Impl<entt::type_list<>> {};

    template <typename Result, typename Event>
    struct AddHandleEvent<Impl<entt::type_list<>>, Result, entt::type_list<Event>>
        : AddHandleEvent<Impl<entt::type_list<>>, Result, entt::type_list<>> {
        virtual Result handleEvent(Event &) = 0;
    };

    template <typename Result, typename Event, typename... Rest>
    struct AddHandleEvent<Impl<entt::type_list<>>, Result, entt::type_list<Event, Rest...>>
        : AddHandleEvent<Impl<entt::type_list<>>, Result, entt::type_list<Rest...>> {
        using AddHandleEvent<Impl<entt::type_list<>>, Result, entt::type_list<Rest...>>::handleEvent;
        virtual Result handleEvent(Event &) = 0;
    };

    template <typename Result, typename... Events>
    struct AddHandleEvent<Impl<entt::type_list<>>, Result, EventVariantImpl<Events...>>
        : AddHandleEvent<Impl<entt::type_list<>>, Result, entt::type_list<Events...>> {};

    template <typename EventVariant>
    struct Impl<entt::type_list<EventVariant>>
        : AddHandleEvent<Impl<entt::type_list<>>, typename HandleEventResult<EventVariant>::type, EventVariant> {
    protected:
        virtual typename HandleEventResult<EventVariant>::type handleEvent(const EventVariant &) = 0;
    };

    template <typename EventVariant, typename... Rest>
    struct Impl<entt::type_list<EventVariant, Rest...>>
        : AddHandleEvent<Impl<entt::type_list<Rest...>>, typename HandleEventResult<EventVariant>::type, EventVariant> {
    protected:
        using AddHandleEvent<Impl<entt::type_list<Rest...>>, typename HandleEventResult<EventVariant>::type,
                             EventVariant>::handleEvent;
        virtual typename HandleEventResult<EventVariant>::type handleEvent(const EventVariant &) = 0;
    };
};
}  // namespace Details

template <typename EventHandlerInterface>
class EventHandlerDispatcher {};
