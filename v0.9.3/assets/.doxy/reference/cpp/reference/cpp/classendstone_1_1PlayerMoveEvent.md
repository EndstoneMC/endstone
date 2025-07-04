

# Class endstone::PlayerMoveEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerMoveEvent**](classendstone_1_1PlayerMoveEvent.md)



_Called when a player moves._ 

* `#include <endstone/event/player/player_move_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)


Inherited by the following classes: [endstone::PlayerJumpEvent](classendstone_1_1PlayerJumpEvent.md),  [endstone::PlayerTeleportEvent](classendstone_1_1PlayerTeleportEvent.md)
























## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**Location**](classendstone_1_1Location.md) | [**from\_**](#variable-from_)  <br> |
|  [**Location**](classendstone_1_1Location.md) | [**to\_**](#variable-to_)  <br> |
















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerMoveEvent**](classendstone_1_1PlayerMoveEvent.md)) <br> |
|   | [**PlayerMoveEvent**](#function-playermoveevent) ([**Player**](classendstone_1_1Player.md) & player, [**Location**](classendstone_1_1Location.md) from, [**Location**](classendstone_1_1Location.md) to) <br> |
|  const [**Location**](classendstone_1_1Location.md) & | [**getFrom**](#function-getfrom) () const<br>_Gets the location this player moved from._  |
|  const [**Location**](classendstone_1_1Location.md) & | [**getTo**](#function-getto) () const<br>_Gets the location this player moved to._  |
|  Result&lt; void &gt; | [**setFrom**](#function-setfrom) (const [**Location**](classendstone_1_1Location.md) & from) <br>_Sets the location to mark as where the player moved from._  |
|  Result&lt; void &gt; | [**setTo**](#function-setto) (const [**Location**](classendstone_1_1Location.md) & to) <br>_Sets the location that this player will move to._  |


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
















































































## Public Attributes Documentation




### variable from\_ 

```C++
Location endstone::PlayerMoveEvent::from_;
```




<hr>



### variable to\_ 

```C++
Location endstone::PlayerMoveEvent::to_;
```




<hr>
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerMoveEvent::ENDSTONE_EVENT (
    PlayerMoveEvent
) 
```




<hr>



### function PlayerMoveEvent 

```C++
inline explicit endstone::PlayerMoveEvent::PlayerMoveEvent (
    Player & player,
    Location from,
    Location to
) 
```




<hr>



### function getFrom 

_Gets the location this player moved from._ 
```C++
inline const Location & endstone::PlayerMoveEvent::getFrom () const
```





**Returns:**

[**Location**](classendstone_1_1Location.md) the player moved from 





        

<hr>



### function getTo 

_Gets the location this player moved to._ 
```C++
inline const Location & endstone::PlayerMoveEvent::getTo () const
```





**Returns:**

[**Location**](classendstone_1_1Location.md) the player moved to 





        

<hr>



### function setFrom 

_Sets the location to mark as where the player moved from._ 
```C++
inline Result< void > endstone::PlayerMoveEvent::setFrom (
    const Location & from
) 
```





**Parameters:**


* `from` New location to mark as the players previous location 




        

<hr>



### function setTo 

_Sets the location that this player will move to._ 
```C++
inline Result< void > endstone::PlayerMoveEvent::setTo (
    const Location & to
) 
```





**Parameters:**


* `to` New [**Location**](classendstone_1_1Location.md) this player will move to 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_move_event.h`

