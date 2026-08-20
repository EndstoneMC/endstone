

# Class endstone::PlayerArmSwingEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerArmSwingEvent**](classendstone_1_1PlayerArmSwingEvent.md)



_Called when a player swings their arm._ 

* `#include <endstone/event/player/player_arm_swing_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerArmSwingEvent**](classendstone_1_1PlayerArmSwingEvent.md)) <br> |
|   | [**PlayerArmSwingEvent**](#function-playerarmswingevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; item) <br> |
|  [**const**](classendstone_1_1Identifier.md) std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; & | [**getItem**](#function-getitem) () const<br>_Gets the item the player was holding when they swung their arm._  |


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
endstone::PlayerArmSwingEvent::ENDSTONE_EVENT (
    PlayerArmSwingEvent
) 
```




<hr>



### function PlayerArmSwingEvent 

```C++
inline endstone::PlayerArmSwingEvent::PlayerArmSwingEvent (
    const  NotNull < Player > & player,
    std::optional< ItemStack > item
) 
```




<hr>



### function getItem 

_Gets the item the player was holding when they swung their arm._ 
```C++
inline const std::optional< ItemStack > & endstone::PlayerArmSwingEvent::getItem () const
```





**Returns:**

the item in the player's hand, or std::nullopt if the hand was empty. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_arm_swing_event.h`

