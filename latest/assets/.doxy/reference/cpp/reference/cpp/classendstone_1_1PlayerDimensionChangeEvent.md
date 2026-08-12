

# Class endstone::PlayerDimensionChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerDimensionChangeEvent**](classendstone_1_1PlayerDimensionChangeEvent.md)



_Called when a player switches to another dimension._ 

* `#include <endstone/event/player/player_dimension_change_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerDimensionChangeEvent**](classendstone_1_1PlayerDimensionChangeEvent.md)) <br> |
|   | [**PlayerDimensionChangeEvent**](#function-playerdimensionchangeevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; & from, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; & to) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; & | [**getFrom**](#function-getfrom) () const<br>_Gets the dimension the player is switching from._  |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; & | [**getTo**](#function-getto) () const<br>_Gets the dimension the player is switching to._  |


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
endstone::PlayerDimensionChangeEvent::ENDSTONE_EVENT (
    PlayerDimensionChangeEvent
) 
```




<hr>



### function PlayerDimensionChangeEvent 

```C++
inline explicit endstone::PlayerDimensionChangeEvent::PlayerDimensionChangeEvent (
    const  NotNull < Player > & player,
    const  NotNull < Dimension > & from,
    const  NotNull < Dimension > & to
) 
```




<hr>



### function getFrom 

_Gets the dimension the player is switching from._ 
```C++
inline const  NotNull < Dimension > & endstone::PlayerDimensionChangeEvent::getFrom () const
```





**Returns:**

player's previous dimension 





        

<hr>



### function getTo 

_Gets the dimension the player is switching to._ 
```C++
inline const  NotNull < Dimension > & endstone::PlayerDimensionChangeEvent::getTo () const
```





**Returns:**

player's new dimension 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_dimension_change_event.h`

