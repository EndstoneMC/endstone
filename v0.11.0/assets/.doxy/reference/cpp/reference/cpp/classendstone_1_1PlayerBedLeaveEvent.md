

# Class endstone::PlayerBedLeaveEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerBedLeaveEvent**](classendstone_1_1PlayerBedLeaveEvent.md)



_Called when a player is leaving a bed._ 

* `#include <endstone/event/player/player_bed_leave_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerBedLeaveEvent**](#function-playerbedleaveevent) ([**Player**](classendstone_1_1Player.md) & player, [**Block**](classendstone_1_1Block.md) & bed) <br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getBed**](#function-getbed) () const<br>_Returns the bed block involved in this event._  |
|   | [**~PlayerBedLeaveEvent**](#function-playerbedleaveevent) () override<br> |


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




### function PlayerBedLeaveEvent 

```C++
inline explicit endstone::PlayerBedLeaveEvent::PlayerBedLeaveEvent (
    Player & player,
    Block & bed
) 
```




<hr>



### function getBed 

_Returns the bed block involved in this event._ 
```C++
inline Block & endstone::PlayerBedLeaveEvent::getBed () const
```





**Returns:**

The bed block involved in this event. 





        

<hr>



### function ~PlayerBedLeaveEvent 

```C++
endstone::PlayerBedLeaveEvent::~PlayerBedLeaveEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_bed_leave_event.h`

