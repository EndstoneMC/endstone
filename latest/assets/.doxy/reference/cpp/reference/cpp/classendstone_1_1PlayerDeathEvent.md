

# Class endstone::PlayerDeathEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerDeathEvent**](classendstone_1_1PlayerDeathEvent.md)



_Called when a_ [_**Player**_](classendstone_1_1Player.md) _dies._

* `#include <endstone/event/player/player_death_event.h>`



Inherits the following classes: [endstone::ActorDeathEvent](classendstone_1_1ActorDeathEvent.md),  [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)


















































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "PlayerDeathEvent"<br> |


## Public Static Attributes inherited from endstone::ActorDeathEvent

See [endstone::ActorDeathEvent](classendstone_1_1ActorDeathEvent.md)

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](classendstone_1_1ActorDeathEvent.md#variable-name)   = = "ActorDeathEvent"<br> |


















































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerDeathEvent**](#function-playerdeathevent) ([**Player**](classendstone_1_1Player.md) & player, std::string death\_message) <br> |
|  const std::string & | [**getDeathMessage**](#function-getdeathmessage) () const<br>_Get the death message that will appear to everyone on the server._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setDeathMessage**](#function-setdeathmessage) (const std::string & death\_message) <br>_Set the death message that will appear to everyone on the server._  |
|   | [**~PlayerDeathEvent**](#function-playerdeathevent) () override<br> |


## Public Functions inherited from endstone::ActorDeathEvent

See [endstone::ActorDeathEvent](classendstone_1_1ActorDeathEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ActorDeathEvent**](classendstone_1_1ActorDeathEvent.md#function-actordeathevent) ([**Actor**](classendstone_1_1Actor.md) & actor) <br> |
| virtual std::string | [**getEventName**](classendstone_1_1ActorDeathEvent.md#function-geteventname) () override const<br> |
| virtual bool | [**isCancellable**](classendstone_1_1ActorDeathEvent.md#function-iscancellable) () override const<br> |
|   | [**~ActorDeathEvent**](classendstone_1_1ActorDeathEvent.md#function-actordeathevent) () override<br> |


## Public Functions inherited from endstone::ActorEvent

See [endstone::ActorEvent](classendstone_1_1ActorEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) ([**Actor**](classendstone_1_1Actor.md) & actor) <br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**getActor**](classendstone_1_1ActorEvent.md#function-getactor) () const<br>_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._ |
|   | [**~ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) () override<br> |


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
const std::string endstone::PlayerDeathEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerDeathEvent 

```C++
inline explicit endstone::PlayerDeathEvent::PlayerDeathEvent (
    Player & player,
    std::string death_message
) 
```




<hr>



### function getDeathMessage 

_Get the death message that will appear to everyone on the server._ 
```C++
inline const std::string & endstone::PlayerDeathEvent::getDeathMessage () const
```





**Returns:**

Message to appear to other players on the server. 





        

<hr>



### function getEventName 


```C++
inline virtual std::string endstone::PlayerDeathEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::ActorDeathEvent::getEventName*](classendstone_1_1ActorDeathEvent.md#function-geteventname)


<hr>



### function isCancellable 


```C++
inline virtual bool endstone::PlayerDeathEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::ActorDeathEvent::isCancellable*](classendstone_1_1ActorDeathEvent.md#function-iscancellable)


<hr>



### function setDeathMessage 

_Set the death message that will appear to everyone on the server._ 
```C++
inline void endstone::PlayerDeathEvent::setDeathMessage (
    const std::string & death_message
) 
```





**Parameters:**


* `death_message` Message to appear to other players on the server. 




        

<hr>



### function ~PlayerDeathEvent 

```C++
endstone::PlayerDeathEvent::~PlayerDeathEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_death_event.h`

