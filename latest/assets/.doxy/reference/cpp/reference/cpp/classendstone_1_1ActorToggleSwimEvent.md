

# Class endstone::ActorToggleSwimEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorToggleSwimEvent**](classendstone_1_1ActorToggleSwimEvent.md)



_Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _'s swimming state is toggled._

* `#include <endstone/event/actor/actor_toggle_swim_event.h>`



Inherits the following classes: [endstone::ActorEvent](classendstone_1_1ActorEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorToggleSwimEvent**](#function-actortoggleswimevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Mob**](classendstone_1_1Mob.md) &gt; & actor, [**bool**](classendstone_1_1Identifier.md) swimming) <br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isSwimming**](#function-isswimming) () const<br>_Returns whether the actor is now swimming or not._  |


## Public Functions inherited from endstone::ActorEvent

See [endstone::ActorEvent](classendstone_1_1ActorEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**ActorType**](classendstone_1_1ActorType.md) &gt; & actor) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**ActorType**](classendstone_1_1ActorType.md) &gt; & | [**getActor**](classendstone_1_1ActorEvent.md#function-getactor) () const<br>_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._ |
|  [**const**](classendstone_1_1Identifier.md) [**endstone::ActorType**](classendstone_1_1ActorType.md) & | [**getActorType**](classendstone_1_1ActorEvent.md#function-getactortype) () const<br>_Returns the type of the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._ |
|   | [**~ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br>_Gets a user-friendly identifier for this event._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br>_Any custom event that should not by synchronized with other events must use the specific constructor._  |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Functions Documentation




### function ActorToggleSwimEvent 

```C++
inline explicit endstone::ActorToggleSwimEvent::ActorToggleSwimEvent (
    const  NotNull < Mob > & actor,
    bool swimming
) 
```




<hr>



### function isSwimming 

_Returns whether the actor is now swimming or not._ 
```C++
inline bool endstone::ActorToggleSwimEvent::isSwimming () const
```





**Returns:**

the new swimming state 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_toggle_swim_event.h`

