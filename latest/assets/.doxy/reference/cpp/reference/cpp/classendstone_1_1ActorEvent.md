

# Class endstone::ActorEvent

**template &lt;[**typename**](classendstone_1_1Vector.md) [**ActorType**](classendstone_1_1Vector.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorEvent**](classendstone_1_1ActorEvent.md)



_Represents an Actor-related event._ 

* `#include <endstone/event/actor/actor_event.h>`



Inherits the following classes: [endstone::Event](classendstone_1_1Event.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](#function-actorevent) ([**ActorType**](classendstone_1_1Vector.md) & actor) <br> |
|  [**ActorType**](classendstone_1_1Vector.md) & | [**getActor**](#function-getactor) () const<br>_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._ |
|   | [**~ActorEvent**](#function-actorevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) ([**bool**](classendstone_1_1Vector.md) async=[**false**](classendstone_1_1Vector.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) ([**const**](classendstone_1_1Vector.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Vector.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Vector.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















































## Public Functions Documentation




### function ActorEvent 

```C++
inline explicit endstone::ActorEvent::ActorEvent (
    ActorType & actor
) 
```




<hr>



### function getActor 

_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._
```C++
inline ActorType & endstone::ActorEvent::getActor () const
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

