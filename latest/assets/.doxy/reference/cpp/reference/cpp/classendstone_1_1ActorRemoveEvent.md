

# Class endstone::ActorRemoveEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorRemoveEvent**](classendstone_1_1ActorRemoveEvent.md)



_Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _is removed._[More...](#detailed-description)

* `#include <endstone/event/actor/actor_remove_event.h>`



Inherits the following classes: [endstone::ActorEvent](classendstone_1_1ActorEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](#function-actorevent) ([**ActorType**](classendstone_1_1Identifier.md) & actor) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ActorRemoveEvent**](classendstone_1_1ActorRemoveEvent.md)) <br> |


## Public Functions inherited from endstone::ActorEvent

See [endstone::ActorEvent](classendstone_1_1ActorEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) ([**ActorType**](classendstone_1_1Identifier.md) & actor) <br> |
|  [**ActorType**](classendstone_1_1Identifier.md) & | [**getActor**](classendstone_1_1ActorEvent.md#function-getactor) () const<br>_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._ |
|   | [**~ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Detailed Description


This event should only be used for monitoring. Modifying the actor during or after this event leads to undefined behaviours. This event will not be called for Players. 


    
## Public Functions Documentation




### function ActorEvent 

```C++
inline explicit endstone::ActorRemoveEvent::ActorEvent (
    ActorType & actor
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::ActorRemoveEvent::ENDSTONE_EVENT (
    ActorRemoveEvent
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_remove_event.h`

