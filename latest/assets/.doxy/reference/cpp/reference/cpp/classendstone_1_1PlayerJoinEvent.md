

# Class endstone::PlayerJoinEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerJoinEvent**](classendstone_1_1PlayerJoinEvent.md)



_Called when a player joins a server._ 

* `#include <endstone/event/player/player_join_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "PlayerJoinEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerJoinEvent**](#function-playerjoinevent) ([**Player**](classendstone_1_1Player.md) & player, std::string join\_message) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  std::string | [**getJoinMessage**](#function-getjoinmessage) () const<br>_Gets the join message to send to all online players._  |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setJoinMessage**](#function-setjoinmessage) (std::string message) <br>_Sets the join message to send to all online players._  |
|   | [**~PlayerJoinEvent**](#function-playerjoinevent) () override<br> |


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
const std::string endstone::PlayerJoinEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerJoinEvent 

```C++
inline explicit endstone::PlayerJoinEvent::PlayerJoinEvent (
    Player & player,
    std::string join_message
) 
```




<hr>



### function getEventName 


```C++
inline virtual std::string endstone::PlayerJoinEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getJoinMessage 

_Gets the join message to send to all online players._ 
```C++
inline std::string endstone::PlayerJoinEvent::getJoinMessage () const
```





**Returns:**

Message to appear to other players on the server. 





        

<hr>



### function isCancellable 


```C++
inline virtual bool endstone::PlayerJoinEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function setJoinMessage 

_Sets the join message to send to all online players._ 
```C++
inline void endstone::PlayerJoinEvent::setJoinMessage (
    std::string message
) 
```





**Parameters:**


* `message` Message to appear to other players on the server. 




        

<hr>



### function ~PlayerJoinEvent 

```C++
endstone::PlayerJoinEvent::~PlayerJoinEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_join_event.h`

