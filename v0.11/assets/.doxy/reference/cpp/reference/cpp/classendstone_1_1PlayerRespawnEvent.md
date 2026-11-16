

# Class endstone::PlayerRespawnEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerRespawnEvent**](classendstone_1_1PlayerRespawnEvent.md)



_Called when a player respawns._ 

* `#include <endstone/event/player/player_respawn_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) (PlayerRespawnEvent) <br> |
|   | [**PlayerRespawnEvent**](#function-playerrespawnevent) ([**Player**](classendstone_1_1Player.md) & player) <br> |
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
|   | [**Event**](classendstone_1_1Event.md#function-event-13) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) (const Event &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) (Event &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  Event & | [**operator=**](classendstone_1_1Event.md#function-operator) (const Event &) = delete<br> |
|  Event & | [**operator=**](classendstone_1_1Event.md#function-operator_1) (Event &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















## Protected Attributes inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|  std::reference\_wrapper&lt; [**Player**](classendstone_1_1Player.md) &gt; | [**player\_**](classendstone_1_1PlayerEvent.md#variable-player_)  <br> |


























































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
inline explicit endstone::PlayerRespawnEvent::PlayerRespawnEvent (
    Player & player
) 
```




<hr>



### function ~PlayerRespawnEvent 

```C++
endstone::PlayerRespawnEvent::~PlayerRespawnEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_respawn_event.h`

