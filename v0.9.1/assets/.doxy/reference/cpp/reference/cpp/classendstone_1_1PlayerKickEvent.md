

# Class endstone::PlayerKickEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerKickEvent**](classendstone_1_1PlayerKickEvent.md)



_Called when a player gets kicked from the server._ 

* `#include <endstone/event/player/player_kick_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"PlayerKickEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerKickEvent**](#function-playerkickevent) ([**Player**](classendstone_1_1Player.md) & player, std::string reason) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  std::string | [**getReason**](#function-getreason) () const<br>_Gets the reason why the player is getting kicked._  |
|  void | [**setReason**](#function-setreason) (std::string reason) <br>_Sets the reason why the player is getting kicked._  |
|   | [**~PlayerKickEvent**](#function-playerkickevent) () override<br> |


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
const std::string endstone::PlayerKickEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerKickEvent 

```C++
inline explicit endstone::PlayerKickEvent::PlayerKickEvent (
    Player & player,
    std::string reason
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::PlayerKickEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


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



### function ~PlayerKickEvent 

```C++
endstone::PlayerKickEvent::~PlayerKickEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_kick_event.h`

