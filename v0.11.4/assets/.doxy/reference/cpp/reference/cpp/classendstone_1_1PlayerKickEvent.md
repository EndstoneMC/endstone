

# Class endstone::PlayerKickEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerKickEvent**](classendstone_1_1PlayerKickEvent.md)



_Called when a player gets kicked from the server._ 

* `#include <endstone/event/player/player_kick_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerKickEvent**](classendstone_1_1PlayerKickEvent.md)) <br> |
|   | [**PlayerKickEvent**](#function-playerkickevent) ([**Player**](classendstone_1_1Player.md) & player, std::string reason) <br> |
|  std::string | [**getReason**](#function-getreason) () const<br>_Gets the reason why the player is getting kicked._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setReason**](#function-setreason) (std::string reason) <br>_Sets the reason why the player is getting kicked._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerKickEvent::ENDSTONE_EVENT (
    PlayerKickEvent
) 
```




<hr>



### function PlayerKickEvent 

```C++
inline explicit endstone::PlayerKickEvent::PlayerKickEvent (
    Player & player,
    std::string reason
) 
```




<hr>



### function getReason 

_Gets the reason why the player is getting kicked._ 
```C++
inline std::string endstone::PlayerKickEvent::getReason () const
```





**Returns:**

string kick reason 





        

<hr>



### function setReason 

_Sets the reason why the player is getting kicked._ 
```C++
inline void endstone::PlayerKickEvent::setReason (
    std::string reason
) 
```





**Parameters:**


* `reason` kick reason 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_kick_event.h`

