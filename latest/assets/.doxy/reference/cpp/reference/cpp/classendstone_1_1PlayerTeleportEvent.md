

# Class endstone::PlayerTeleportEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerTeleportEvent**](classendstone_1_1PlayerTeleportEvent.md)



_Called when a player is teleported from one location to another._ 

* `#include <endstone/event/player/player_teleport_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "PlayerTeleportEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerTeleportEvent**](#function-playerteleportevent) ([**Player**](classendstone_1_1Player.md) & player, [**Location**](classendstone_1_1Location.md) from, [**Location**](classendstone_1_1Location.md) to) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  const [**Location**](classendstone_1_1Location.md) & | [**getFrom**](#function-getfrom) () const<br>_Gets the location that this player moved from._  |
|  const [**Location**](classendstone_1_1Location.md) & | [**getTo**](#function-getto) () const<br>_Gets the location that this player moved to._  |
|  void | [**setFrom**](#function-setfrom) (const [**Location**](classendstone_1_1Location.md) & from) <br>_Sets the location that this player moved from._  |
|  void | [**setTo**](#function-setto) (const [**Location**](classendstone_1_1Location.md) & to) <br>_Sets the location that this player moved to._  |
|   | [**~PlayerTeleportEvent**](#function-playerteleportevent) () override<br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual bool | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual void | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) (bool cancel) <br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual bool | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual void | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) (bool cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::PlayerTeleportEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerTeleportEvent 

```C++
inline explicit endstone::PlayerTeleportEvent::PlayerTeleportEvent (
    Player & player,
    Location from,
    Location to
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::PlayerTeleportEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getFrom 

_Gets the location that this player moved from._ 
```C++
inline const Location & endstone::PlayerTeleportEvent::getFrom () const
```





**Returns:**

[**Location**](classendstone_1_1Location.md) this player moved from 





        

<hr>



### function getTo 

_Gets the location that this player moved to._ 
```C++
inline const Location & endstone::PlayerTeleportEvent::getTo () const
```





**Returns:**

[**Location**](classendstone_1_1Location.md) this player moved to 





        

<hr>



### function setFrom 

_Sets the location that this player moved from._ 
```C++
inline void endstone::PlayerTeleportEvent::setFrom (
    const Location & from
) 
```





**Parameters:**


* `from` New location this player moved from 




        

<hr>



### function setTo 

_Sets the location that this player moved to._ 
```C++
inline void endstone::PlayerTeleportEvent::setTo (
    const Location & to
) 
```





**Parameters:**


* `to` New [**Location**](classendstone_1_1Location.md) this player moved to 




        

<hr>



### function ~PlayerTeleportEvent 

```C++
endstone::PlayerTeleportEvent::~PlayerTeleportEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_teleport_event.h`

