

# Class endstone::PlayerCommandEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerCommandEvent**](classendstone_1_1PlayerCommandEvent.md)



_Called whenever a player runs a command._ 

* `#include <endstone/event/player/player_command_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "PlayerCommandEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerCommandEvent**](#function-playercommandevent) ([**Player**](classendstone_1_1Player.md) & player, std::string command) <br> |
|  std::string | [**getCommand**](#function-getcommand) () const<br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setCommand**](#function-setcommand) (std::string command) <br> |
|   | [**~PlayerCommandEvent**](#function-playercommandevent) () override<br> |


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



### function isCancellable 


```C++
inline virtual bool endstone::PlayerCommandEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


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

