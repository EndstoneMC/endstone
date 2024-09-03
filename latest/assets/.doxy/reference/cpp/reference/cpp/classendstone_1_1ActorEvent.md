

# Class endstone::ActorEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorEvent**](classendstone_1_1ActorEvent.md)



_Represents an Actor-related event._ 

* `#include <endstone/event/actor/actor_event.h>`



Inherits the following classes: [endstone::Event](classendstone_1_1Event.md)


Inherited by the following classes: [endstone::ActorDeathEvent](classendstone_1_1ActorDeathEvent.md),  [endstone::ActorKnockbackEvent](classendstone_1_1ActorKnockbackEvent.md),  [endstone::ActorRemoveEvent](classendstone_1_1ActorRemoveEvent.md),  [endstone::ActorSpawnEvent](classendstone_1_1ActorSpawnEvent.md),  [endstone::ActorTeleportEvent](classendstone_1_1ActorTeleportEvent.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](#function-actorevent) ([**Actor**](classendstone_1_1Actor.md) & actor) <br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**getActor**](#function-getactor) () const<br>_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._ |
|   | [**~ActorEvent**](#function-actorevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
| virtual bool | [**isCancellable**](classendstone_1_1Event.md#function-iscancellable) () const = 0<br> |
|  bool | [**isCancelled**](classendstone_1_1Event.md#function-iscancelled) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  void | [**setCancelled**](classendstone_1_1Event.md#function-setcancelled) (bool cancel) <br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















































## Public Functions Documentation




### function ActorEvent 

```C++
inline explicit endstone::ActorEvent::ActorEvent (
    Actor & actor
) 
```




<hr>



### function getActor 

_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._
```C++
inline Actor & endstone::ActorEvent::getActor () const
```





**Returns:**

[**Actor**](classendstone_1_1Actor.md) which is involved in this event 





        

<hr>



### function ~ActorEvent 

```C++
endstone::ActorEvent::~ActorEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_event.h`

