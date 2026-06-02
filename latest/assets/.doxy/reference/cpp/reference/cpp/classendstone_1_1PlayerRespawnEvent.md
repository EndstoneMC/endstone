

# Class endstone::PlayerRespawnEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerRespawnEvent**](classendstone_1_1PlayerRespawnEvent.md)



_Called when a player respawns._ 

* `#include <endstone/event/player/player_respawn_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**RespawnReason**](#enum-respawnreason)  <br>_An enum to specify the reason a respawn occurred._  |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerRespawnEvent**](classendstone_1_1PlayerRespawnEvent.md)) <br> |
|   | [**PlayerRespawnEvent**](#function-playerrespawnevent) ([**Player**](classendstone_1_1Player.md) & player, [**RespawnReason**](classendstone_1_1PlayerRespawnEvent.md#enum-respawnreason) reason) <br> |
|  [**RespawnReason**](classendstone_1_1PlayerRespawnEvent.md#enum-respawnreason) | [**getRespawnReason**](#function-getrespawnreason) () const<br>_Gets the reason this respawn occurred._  |
|   | [**~PlayerRespawnEvent**](#function-playerrespawnevent) () override<br> |


## Public Functions inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) ([**Player**](classendstone_1_1Player.md) & player) <br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](classendstone_1_1PlayerEvent.md#function-getplayer) () const<br>_Returns the player involved in this event._  |
|   | [**~PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br>_Gets a user-friendly identifier for this event._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br>_Any custom event that should not by synchronized with other events must use the specific constructor._  |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















## Protected Attributes inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|  std::reference\_wrapper&lt; [**Player**](classendstone_1_1Player.md) &gt; | [**player\_**](classendstone_1_1PlayerEvent.md#variable-player_)  <br> |


























































## Public Types Documentation




### enum RespawnReason 

_An enum to specify the reason a respawn occurred._ 
```C++
enum endstone::PlayerRespawnEvent::RespawnReason {
    Death,
    EndPortal
};
```




<hr>
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerRespawnEvent::ENDSTONE_EVENT (
    PlayerRespawnEvent
) 
```




<hr>



### function PlayerRespawnEvent 

```C++
inline endstone::PlayerRespawnEvent::PlayerRespawnEvent (
    Player & player,
    RespawnReason reason
) 
```




<hr>



### function getRespawnReason 

_Gets the reason this respawn occurred._ 
```C++
inline RespawnReason endstone::PlayerRespawnEvent::getRespawnReason () const
```





**Returns:**

the reason the respawn occurred. 





        

<hr>



### function ~PlayerRespawnEvent 

```C++
endstone::PlayerRespawnEvent::~PlayerRespawnEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_respawn_event.h`

