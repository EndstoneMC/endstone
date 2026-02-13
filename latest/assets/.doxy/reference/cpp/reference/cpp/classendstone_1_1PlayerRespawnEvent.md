

# Class endstone::PlayerRespawnEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerRespawnEvent**](classendstone_1_1PlayerRespawnEvent.md)



_Called when a player respawns._ 

* `#include <endstone/event/player/player_respawn_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Identifier.md) std::string | [**NAME**](#variable-name)   = `"PlayerRespawnEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerRespawnEvent**](#function-playerrespawnevent) ([**Player**](classendstone_1_1Player.md) & player) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|   | [**~PlayerRespawnEvent**](#function-playerrespawnevent) () override<br> |


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
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















## Protected Attributes inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|  std::reference\_wrapper&lt; [**Player**](classendstone_1_1Player.md) &gt; | [**player\_**](classendstone_1_1PlayerEvent.md#variable-player_)  <br> |


























































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::PlayerRespawnEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerRespawnEvent 

```C++
inline explicit endstone::PlayerRespawnEvent::PlayerRespawnEvent (
    Player & player
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::PlayerRespawnEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function ~PlayerRespawnEvent 

```C++
endstone::PlayerRespawnEvent::~PlayerRespawnEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_respawn_event.h`

