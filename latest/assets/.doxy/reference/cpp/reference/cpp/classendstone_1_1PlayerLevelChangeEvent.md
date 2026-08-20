

# Class endstone::PlayerLevelChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerLevelChangeEvent**](classendstone_1_1PlayerLevelChangeEvent.md)



_Called when a player's level changes._ 

* `#include <endstone/event/player/player_level_change_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerLevelChangeEvent**](classendstone_1_1PlayerLevelChangeEvent.md)) <br> |
|   | [**PlayerLevelChangeEvent**](#function-playerlevelchangeevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**int**](classendstone_1_1Identifier.md) old\_level, [**int**](classendstone_1_1Identifier.md) new\_level) <br> |
|  [**int**](classendstone_1_1Identifier.md) | [**getNewLevel**](#function-getnewlevel) () const<br>_Gets the player's level after the change._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getOldLevel**](#function-getoldlevel) () const<br>_Gets the player's level before the change._  |


## Public Functions inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & | [**getPlayer**](classendstone_1_1PlayerEvent.md#function-getplayer) () const<br>_Returns the player involved in this event._  |
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
|  [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; | [**player\_**](classendstone_1_1PlayerEvent.md#variable-player_)  <br> |


























































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerLevelChangeEvent::ENDSTONE_EVENT (
    PlayerLevelChangeEvent
) 
```




<hr>



### function PlayerLevelChangeEvent 

```C++
inline endstone::PlayerLevelChangeEvent::PlayerLevelChangeEvent (
    const  NotNull < Player > & player,
    int old_level,
    int new_level
) 
```




<hr>



### function getNewLevel 

_Gets the player's level after the change._ 
```C++
inline int endstone::PlayerLevelChangeEvent::getNewLevel () const
```





**Returns:**

the player's new level 





        

<hr>



### function getOldLevel 

_Gets the player's level before the change._ 
```C++
inline int endstone::PlayerLevelChangeEvent::getOldLevel () const
```





**Returns:**

the player's previous level 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_level_change_event.h`

