

# Class endstone::PlayerCommandEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerCommandEvent**](classendstone_1_1PlayerCommandEvent.md)



_Called whenever a player runs a command._ 

* `#include <endstone/event/player/player_command_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**NAME**](#variable-name)   = `"PlayerCommandEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerCommandEvent**](#function-playercommandevent) ([**Player**](classendstone_1_1Player.md) & player, std::string command) <br> |
|  std::string | [**getCommand**](#function-getcommand) () const<br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**void**](classendstone_1_1Vector.md) | [**setCommand**](#function-setcommand) (std::string command) <br> |
|   | [**~PlayerCommandEvent**](#function-playercommandevent) () override<br> |


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
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::PlayerCommandEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerCommandEvent 

```C++
inline explicit endstone::PlayerCommandEvent::PlayerCommandEvent (
    Player & player,
    std::string command
) 
```




<hr>



### function getCommand 

```C++
inline std::string endstone::PlayerCommandEvent::getCommand () const
```



Gets the command that the player is attempting to send.




**Returns:**

[**Command**](classendstone_1_1Command.md) the player is attempting to send 





        

<hr>



### function getEventName 

```C++
inline virtual std::string endstone::PlayerCommandEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function setCommand 

```C++
inline void endstone::PlayerCommandEvent::setCommand (
    std::string command
) 
```



Sets the command that the player will send.




**Parameters:**


* `command` New command that the player will send 




        

<hr>



### function ~PlayerCommandEvent 

```C++
endstone::PlayerCommandEvent::~PlayerCommandEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_command_event.h`

