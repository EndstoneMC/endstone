

# Class endstone::ActorTeleportEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorTeleportEvent**](classendstone_1_1ActorTeleportEvent.md)



_Called when a non-player entity is teleported from one location to another._ [More...](#detailed-description)

* `#include <endstone/event/actor/actor_teleport_event.h>`



Inherits the following classes: [endstone::ActorEvent](classendstone_1_1ActorEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "ActorTeleportEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorTeleportEvent**](#function-actorteleportevent) ([**Actor**](classendstone_1_1Actor.md) & actor, [**Location**](classendstone_1_1Location.md) from, [**Location**](classendstone_1_1Location.md) to) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  const [**Location**](classendstone_1_1Location.md) & | [**getFrom**](#function-getfrom) () const<br> |
|  const [**Location**](classendstone_1_1Location.md) & | [**getTo**](#function-getto) () const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setFrom**](#function-setfrom) (const [**Location**](classendstone_1_1Location.md) & from) <br> |
|  void | [**setTo**](#function-setto) (const [**Location**](classendstone_1_1Location.md) & to) <br> |
|   | [**~ActorTeleportEvent**](#function-actorteleportevent) () override<br> |


## Public Functions inherited from endstone::ActorEvent

See [endstone::ActorEvent](classendstone_1_1ActorEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) ([**Actor**](classendstone_1_1Actor.md) & actor) <br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**getActor**](classendstone_1_1ActorEvent.md#function-getactor) () const<br> |
|   | [**~ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) () override<br> |


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
















































































# Detailed Description


This may be as a result of natural causes (Enderman, Shulker), pathfinding (Wolf), or commands (/teleport). 


    
## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::ActorTeleportEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ActorTeleportEvent 

```C++
inline explicit endstone::ActorTeleportEvent::ActorTeleportEvent (
    Actor & actor,
    Location from,
    Location to
) 
```




<hr>



### function getEventName 


```C++
inline virtual std::string endstone::ActorTeleportEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getFrom 

```C++
inline const Location & endstone::ActorTeleportEvent::getFrom () const
```




<hr>



### function getTo 

```C++
inline const Location & endstone::ActorTeleportEvent::getTo () const
```




<hr>



### function isCancellable 


```C++
inline virtual bool endstone::ActorTeleportEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function setFrom 

```C++
inline void endstone::ActorTeleportEvent::setFrom (
    const Location & from
) 
```




<hr>



### function setTo 

```C++
inline void endstone::ActorTeleportEvent::setTo (
    const Location & to
) 
```




<hr>



### function ~ActorTeleportEvent 

```C++
endstone::ActorTeleportEvent::~ActorTeleportEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_teleport_event.h`

