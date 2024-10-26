

# Class endstone::PlayerKickEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerKickEvent**](classendstone_1_1PlayerKickEvent.md)



_Called when a player gets kicked from the server._ 

* `#include <endstone/event/player/player_kick_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "PlayerKickEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerKickEvent**](#function-playerkickevent) ([**Player**](classendstone_1_1Player.md) & player, std::string reason) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  std::string | [**getReason**](#function-getreason) () const<br>_Gets the reason why the player is getting kicked._  |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setReason**](#function-setreason) (std::string reason) <br>_Sets the reason why the player is getting kicked._  |
|   | [**~PlayerKickEvent**](#function-playerkickevent) () override<br> |


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



### function isCancellable 


```C++
inline virtual bool endstone::PlayerKickEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


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

