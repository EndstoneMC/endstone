

# Class endstone::PlayerTeleportEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerTeleportEvent**](classendstone_1_1PlayerTeleportEvent.md)



_Called when a player is teleported from one location to another._ 

* `#include <endstone/event/player/player_teleport_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































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
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setFrom**](#function-setfrom) (const [**Location**](classendstone_1_1Location.md) & from) <br>_Sets the location that this player moved from._  |
|  void | [**setTo**](#function-setto) (const [**Location**](classendstone_1_1Location.md) & to) <br>_Sets the location that this player moved to._  |
|   | [**~PlayerTeleportEvent**](#function-playerteleportevent) () override<br> |


## Public Functions inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) ([**Player**](classendstone_1_1Player.md) & player) <br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](classendstone_1_1PlayerEvent.md#function-getplayer) () const<br> |
|   | [**~PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) () override<br> |


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



### function isCancellable 


```C++
inline virtual bool endstone::PlayerTeleportEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


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

