

# Class endstone::ActorTeleportEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorTeleportEvent**](classendstone_1_1ActorTeleportEvent.md)



_Called when a non-player entity is teleported from one location to another._ [More...](#detailed-description)

* `#include <endstone/event/actor/actor_teleport_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**NAME**](#variable-name)   = `"ActorTeleportEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorTeleportEvent**](#function-actorteleportevent) ([**Actor**](classendstone_1_1Actor.md) & actor, [**Location**](classendstone_1_1Location.md) from, [**Location**](classendstone_1_1Location.md) to) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**const**](classendstone_1_1Vector.md) [**Location**](classendstone_1_1Location.md) & | [**getFrom**](#function-getfrom) () const<br>_Gets the location that this actor moved from._  |
|  [**const**](classendstone_1_1Vector.md) [**Location**](classendstone_1_1Location.md) & | [**getTo**](#function-getto) () const<br>_Gets the location that this actor moved to._  |
|  [**void**](classendstone_1_1Vector.md) | [**setFrom**](#function-setfrom) ([**const**](classendstone_1_1Vector.md) [**Location**](classendstone_1_1Location.md) & from) <br>_Sets the location that this actor moved from._  |
|  [**void**](classendstone_1_1Vector.md) | [**setTo**](#function-setto) ([**const**](classendstone_1_1Vector.md) [**Location**](classendstone_1_1Location.md) & to) <br>_Sets the location that this actor moved to._  |
|   | [**~ActorTeleportEvent**](#function-actorteleportevent) () override<br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Vector.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Vector.md) cancel) <br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Vector.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Vector.md) cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Detailed Description


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

_Gets the location that this actor moved from._ 
```C++
inline const  Location & endstone::ActorTeleportEvent::getFrom () const
```





**Returns:**

[**Location**](classendstone_1_1Location.md) this actor moved from 





        

<hr>



### function getTo 

_Gets the location that this actor moved to._ 
```C++
inline const  Location & endstone::ActorTeleportEvent::getTo () const
```





**Returns:**

[**Location**](classendstone_1_1Location.md) this actor moved to 





        

<hr>



### function setFrom 

_Sets the location that this actor moved from._ 
```C++
inline void endstone::ActorTeleportEvent::setFrom (
    const  Location & from
) 
```





**Parameters:**


* `from` New location this actor moved from 




        

<hr>



### function setTo 

_Sets the location that this actor moved to._ 
```C++
inline void endstone::ActorTeleportEvent::setTo (
    const  Location & to
) 
```





**Parameters:**


* `to` New [**Location**](classendstone_1_1Location.md) this actor moved to 




        

<hr>



### function ~ActorTeleportEvent 

```C++
endstone::ActorTeleportEvent::~ActorTeleportEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_teleport_event.h`

