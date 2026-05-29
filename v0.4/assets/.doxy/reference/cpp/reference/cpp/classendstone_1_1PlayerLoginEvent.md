

# Class endstone::PlayerLoginEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerLoginEvent**](classendstone_1_1PlayerLoginEvent.md)



_Called when a player attempts to login in._ 

* `#include <endstone/event/player/player_login_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "PlayerLoginEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerLoginEvent**](#function-playerloginevent) ([**Player**](classendstone_1_1Player.md) & player, std::string message="") <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  const std::string & | [**getKickMessage**](#function-getkickmessage) () const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setKickMessage**](#function-setkickmessage) (const std::string & message) <br> |
|   | [**~PlayerLoginEvent**](#function-playerloginevent) () override<br> |


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
const std::string endstone::PlayerLoginEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerLoginEvent 

```C++
inline explicit endstone::PlayerLoginEvent::PlayerLoginEvent (
    Player & player,
    std::string message=""
) 
```




<hr>



### function getEventName 


```C++
inline virtual std::string endstone::PlayerLoginEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getKickMessage 


```C++
inline const std::string & endstone::PlayerLoginEvent::getKickMessage () const
```



Gets the current kick message that will be used if event is cancelled




**Returns:**

Current kick message 





        

<hr>



### function isCancellable 


```C++
inline virtual bool endstone::PlayerLoginEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function setKickMessage 


```C++
inline void endstone::PlayerLoginEvent::setKickMessage (
    const std::string & message
) 
```



Sets the kick message to display if event is cancelled




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

