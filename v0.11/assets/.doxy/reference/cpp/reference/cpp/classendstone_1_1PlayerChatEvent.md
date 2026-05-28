

# Class endstone::PlayerChatEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerChatEvent**](classendstone_1_1PlayerChatEvent.md)



_Called when a player sends a chat message._ 

* `#include <endstone/event/player/player_chat_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerChatEvent**](classendstone_1_1PlayerChatEvent.md)) <br> |
|   | [**PlayerChatEvent**](#function-playerchatevent) ([**Player**](classendstone_1_1Player.md) & player, std::string message, std::optional&lt; std::vector&lt; [**Player**](classendstone_1_1Player.md) \* &gt; &gt; recipients, std::string format="&lt;{0}&gt; {1}") <br> |
|  std::string | [**getFormat**](#function-getformat) () const<br>_Gets the format to use to display this chat message._  |
|  std::string | [**getMessage**](#function-getmessage) () const<br>_Gets the message that the player is attempting to send._  |
|  std::vector&lt; [**Player**](classendstone_1_1Player.md) \* &gt; | [**getRecipients**](#function-getrecipients) () const<br>_Gets a set of recipients that this chat message will be displayed to._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setFormat**](#function-setformat) (std::string format) <br>_Sets the format to use to display this chat message._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setMessage**](#function-setmessage) (std::string message) <br>_Sets the message that the player will send._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setPlayer**](#function-setplayer) ([**Player**](classendstone_1_1Player.md) & player) <br>_Sets the player that this message will display as._  |


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
endstone::PlayerChatEvent::ENDSTONE_EVENT (
    PlayerChatEvent
) 
```




<hr>



### function PlayerChatEvent 

```C++
inline explicit endstone::PlayerChatEvent::PlayerChatEvent (
    Player & player,
    std::string message,
    std::optional< std::vector< Player * > > recipients,
    std::string format="<{0}> {1}"
) 
```




<hr>



### function getFormat 

_Gets the format to use to display this chat message._ 
```C++
inline std::string endstone::PlayerChatEvent::getFormat () const
```



See [the format string syntax](https://en.cppreference.com/w/cpp/utility/format/spec.html)




**Returns:**

format string 





        

<hr>



### function getMessage 

_Gets the message that the player is attempting to send._ 
```C++
inline std::string endstone::PlayerChatEvent::getMessage () const
```





**Returns:**

Message the player is attempting to send 





        

<hr>



### function getRecipients 

_Gets a set of recipients that this chat message will be displayed to._ 
```C++
inline std::vector< Player * > endstone::PlayerChatEvent::getRecipients () const
```





**Returns:**

All Players who will see this chat message 





        

<hr>



### function setFormat 

_Sets the format to use to display this chat message._ 
```C++
inline void endstone::PlayerChatEvent::setFormat (
    std::string format
) 
```





**Parameters:**


* `format` format string 




        

<hr>



### function setMessage 

_Sets the message that the player will send._ 
```C++
inline void endstone::PlayerChatEvent::setMessage (
    std::string message
) 
```





**Parameters:**


* `message` New message that the player will send 




        

<hr>



### function setPlayer 

_Sets the player that this message will display as._ 
```C++
inline void endstone::PlayerChatEvent::setPlayer (
    Player & player
) 
```





**Parameters:**


* `player` New player which this event will execute as 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_chat_event.h`

