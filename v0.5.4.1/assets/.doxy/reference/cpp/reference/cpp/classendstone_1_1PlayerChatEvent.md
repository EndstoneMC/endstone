

# Class endstone::PlayerChatEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerChatEvent**](classendstone_1_1PlayerChatEvent.md)



_Called when a player sends a chat message._ 

* `#include <endstone/event/player/player_chat_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "PlayerChatEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerChatEvent**](#function-playerchatevent) ([**Player**](classendstone_1_1Player.md) & player, std::string message) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  std::string | [**getMessage**](#function-getmessage) () const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setMessage**](#function-setmessage) (std::string message) <br> |
|   | [**~PlayerChatEvent**](#function-playerchatevent) () override<br> |


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



### function isCancellable 


```C++
inline virtual bool endstone::PlayerChatEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


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

