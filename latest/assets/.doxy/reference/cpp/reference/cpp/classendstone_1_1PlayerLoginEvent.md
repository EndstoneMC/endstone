

# Class endstone::PlayerLoginEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerLoginEvent**](classendstone_1_1PlayerLoginEvent.md)



_Called when a player attempts to login in._ 

* `#include <endstone/event/player/player_login_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerLoginEvent**](classendstone_1_1PlayerLoginEvent.md)) <br> |
|   | [**PlayerLoginEvent**](#function-playerloginevent) ([**Player**](classendstone_1_1Player.md) & player, std::string message="") <br> |
|  [**const**](classendstone_1_1Identifier.md) std::string & | [**getKickMessage**](#function-getkickmessage) () const<br>_Gets the current kick message that will be used if event is cancelled._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setKickMessage**](#function-setkickmessage) ([**const**](classendstone_1_1Identifier.md) std::string & message) <br>_Sets the kick message to display if event is cancelled._  |
|   | [**~PlayerLoginEvent**](#function-playerloginevent) () override<br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event._  |


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
endstone::PlayerLoginEvent::ENDSTONE_EVENT (
    PlayerLoginEvent
) 
```




<hr>



### function PlayerLoginEvent 

```C++
inline explicit endstone::PlayerLoginEvent::PlayerLoginEvent (
    Player & player,
    std::string message=""
) 
```




<hr>



### function getKickMessage 

_Gets the current kick message that will be used if event is cancelled._ 
```C++
inline const std::string & endstone::PlayerLoginEvent::getKickMessage () const
```





**Returns:**

Current kick message 





        

<hr>



### function setKickMessage 

_Sets the kick message to display if event is cancelled._ 
```C++
inline void endstone::PlayerLoginEvent::setKickMessage (
    const std::string & message
) 
```





**Parameters:**


* `message` New kick message 




        

<hr>



### function ~PlayerLoginEvent 

```C++
endstone::PlayerLoginEvent::~PlayerLoginEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_login_event.h`

