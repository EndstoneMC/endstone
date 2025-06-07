

# Class endstone::PlayerChatEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerChatEvent**](classendstone_1_1PlayerChatEvent.md)



_Called when a player sends a chat message._ 

* `#include <endstone/event/player/player_chat_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**NAME**](#variable-name)   = `"PlayerChatEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerChatEvent**](#function-playerchatevent) ([**Player**](classendstone_1_1Player.md) & player, std::string message) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  std::string | [**getMessage**](#function-getmessage) () const<br> |
|  [**void**](classendstone_1_1Vector.md) | [**setMessage**](#function-setmessage) (std::string message) <br> |
|   | [**~PlayerChatEvent**](#function-playerchatevent) () override<br> |


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
const std::string endstone::PlayerChatEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerChatEvent 

```C++
inline explicit endstone::PlayerChatEvent::PlayerChatEvent (
    Player & player,
    std::string message
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::PlayerChatEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getMessage 

```C++
inline std::string endstone::PlayerChatEvent::getMessage () const
```



Gets the message that the player is attempting to send.




**Returns:**

Message the player is attempting to send 





        

<hr>



### function setMessage 

```C++
inline void endstone::PlayerChatEvent::setMessage (
    std::string message
) 
```



Sets the message that the player will send.




**Parameters:**


* `message` New message that the player will send 




        

<hr>



### function ~PlayerChatEvent 

```C++
endstone::PlayerChatEvent::~PlayerChatEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_chat_event.h`

