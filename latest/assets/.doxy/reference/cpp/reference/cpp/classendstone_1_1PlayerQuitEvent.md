

# Class endstone::PlayerQuitEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerQuitEvent**](classendstone_1_1PlayerQuitEvent.md)



_Called when a player leaves a server._ 

* `#include <endstone/event/player/player_quit_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "PlayerQuitEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerQuitEvent**](#function-playerquitevent) ([**Player**](classendstone_1_1Player.md) & player, std::string quit\_message) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  std::string | [**getQuitMessage**](#function-getquitmessage) () const<br>_Gets the quit message to send to all online players._  |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setQuitMessage**](#function-setquitmessage) (std::string message) <br>_Sets the quit message to send to all online players._  |
|   | [**~PlayerQuitEvent**](#function-playerquitevent) () override<br> |


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
const std::string endstone::PlayerQuitEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerQuitEvent 

```C++
inline explicit endstone::PlayerQuitEvent::PlayerQuitEvent (
    Player & player,
    std::string quit_message
) 
```




<hr>



### function getEventName 


```C++
inline virtual std::string endstone::PlayerQuitEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getQuitMessage 

_Gets the quit message to send to all online players._ 
```C++
inline std::string endstone::PlayerQuitEvent::getQuitMessage () const
```





**Returns:**

Message to appear to other players on the server. 





        

<hr>



### function isCancellable 


```C++
inline virtual bool endstone::PlayerQuitEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function setQuitMessage 

_Sets the quit message to send to all online players._ 
```C++
inline void endstone::PlayerQuitEvent::setQuitMessage (
    std::string message
) 
```





**Parameters:**


* `message` Message to appear to other players on the server. 




        

<hr>



### function ~PlayerQuitEvent 

```C++
endstone::PlayerQuitEvent::~PlayerQuitEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_quit_event.h`

