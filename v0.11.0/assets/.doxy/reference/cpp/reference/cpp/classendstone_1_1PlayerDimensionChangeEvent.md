

# Class endstone::PlayerDimensionChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerDimensionChangeEvent**](classendstone_1_1PlayerDimensionChangeEvent.md)



_Called when a player switches to another dimension._ 

* `#include <endstone/event/player/player_dimension_change_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerDimensionChangeEvent**](classendstone_1_1PlayerDimensionChangeEvent.md)) <br> |
|   | [**PlayerDimensionChangeEvent**](#function-playerdimensionchangeevent) ([**Player**](classendstone_1_1Player.md) & player, [**Dimension**](classendstone_1_1Dimension.md) & from, [**Dimension**](classendstone_1_1Dimension.md) & to) <br> |
|  [**Dimension**](classendstone_1_1Dimension.md) & | [**getFrom**](#function-getfrom) () const<br>_Gets the dimension the player is switching from._  |
|  [**Dimension**](classendstone_1_1Dimension.md) & | [**getTo**](#function-getto) () const<br>_Gets the dimension the player is switching to._  |


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
|   | [**Event**](classendstone_1_1Event.md#function-event-12) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















## Protected Attributes inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|  std::reference\_wrapper&lt; [**Player**](classendstone_1_1Player.md) &gt; | [**player\_**](classendstone_1_1PlayerEvent.md#variable-player_)  <br> |


























































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
    Player & player,
    Dimension & from,
    Dimension & to
) 
```




<hr>



### function getFrom 

_Gets the dimension the player is switching from._ 
```C++
inline Dimension & endstone::PlayerDimensionChangeEvent::getFrom () const
```





**Returns:**

player's previous dimension 





        

<hr>



### function getTo 

_Gets the dimension the player is switching to._ 
```C++
inline Dimension & endstone::PlayerDimensionChangeEvent::getTo () const
```





**Returns:**

player's new dimension 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_dimension_change_event.h`

