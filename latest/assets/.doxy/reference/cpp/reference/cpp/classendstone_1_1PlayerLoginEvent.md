

# Class endstone::PlayerLoginEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerLoginEvent**](classendstone_1_1PlayerLoginEvent.md)



_Called when a player attempts to login in._ 

* `#include <endstone/event/player/player_login_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"PlayerLoginEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerLoginEvent**](#function-playerloginevent) ([**Player**](classendstone_1_1Player.md) & player, std::string message="") <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  const std::string & | [**getKickMessage**](#function-getkickmessage) () const<br> |
|  void | [**setKickMessage**](#function-setkickmessage) (const std::string & message) <br> |
|   | [**~PlayerLoginEvent**](#function-playerloginevent) () override<br> |


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

