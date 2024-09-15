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

#include <entt/entt.hpp>

namespace Details {
template <typename T>
struct EventHandlerDispatcher {

    template <typename... TypeList>
    struct Impl
        : EventHandlerDispatcher<ActorGameplayHandler>::AddHandleEvent<
              EventHandlerDispatcher<ActorGameplayHandler>::Impl<entt::type_list<
                  const ActorGameplayEvent<CoordinatorResult>, MutableActorGameplayEvent<CoordinatorResult>>>,
              HandlerResult,
              EventVariantImpl<const ActorDummyTestEvent, const ActorAcquiredItemEvent,
                               const ActorAnimationChangedEvent, const ActorAttackEvent,
                               const ActorCarriedItemChangedEvent, const ActorDefinitionTriggeredEvent,
                               const ActorDefinitionEndedEvent, const ActorDiedEvent, const ActorDroppedItemEvent,
                               const ActorEquippedArmorEvent, const ActorHurtEvent, const ActorHealthChangedEvent,
                               const ActorKilledEvent, const ActorPlacedItemEvent, const ActorRemovedEvent,
                               const ActorRemoveEffectEvent, const ActorStartRidingEvent, const ActorUseItemEvent,
                               const KnockBackEvent, const MountTamingEvent>> /*location=0x0*/ {  // size=0x8
    protected:
        virtual Details::EventHandlerDispatcher<ActorGameplayHandler>::HandleEventResult<
            const ActorGameplayEvent<void>>::type
        handleEvent(/*this,*/ ActorGameplayEvent<void> const &event);  // vtable_elem_location=2
    };

    typedef Impl<entt::type_list<const ActorGameplayEvent<void>, const ActorGameplayEvent<CoordinatorResult>,
                                 MutableActorGameplayEvent<CoordinatorResult>>>
        type;
};
}  // namespace Details

template <typename T>
class EventHandlerDispatcher : public Details::EventHandlerDispatcher<T>::type {};
