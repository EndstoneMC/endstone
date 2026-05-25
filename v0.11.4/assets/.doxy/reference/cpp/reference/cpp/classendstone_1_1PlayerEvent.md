

# Class endstone::PlayerEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerEvent**](classendstone_1_1PlayerEvent.md)



_Represents a player related event._ 

* `#include <endstone/event/player/player_event.h>`



Inherits the following classes: [endstone::Event](classendstone_1_1Event.md)


Inherited by the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md),  [endstone::PlayerBedLeaveEvent](classendstone_1_1PlayerBedLeaveEvent.md),  [endstone::PlayerDimensionChangeEvent](classendstone_1_1PlayerDimensionChangeEvent.md),  [endstone::PlayerJoinEvent](classendstone_1_1PlayerJoinEvent.md),  [endstone::PlayerQuitEvent](classendstone_1_1PlayerQuitEvent.md),  [endstone::PlayerRespawnEvent](classendstone_1_1PlayerRespawnEvent.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerEvent**](#function-playerevent) ([**Player**](classendstone_1_1Player.md) & player) <br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](#function-getplayer) () const<br> |
|   | [**~PlayerEvent**](#function-playerevent) () override<br> |


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














## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::reference\_wrapper&lt; [**Player**](classendstone_1_1Player.md) &gt; | [**player\_**](#variable-player_)  <br> |








































## Public Functions Documentation




### function PlayerEvent 

```C++
inline explicit endstone::PlayerEvent::PlayerEvent (
    Player & player
) 
```




<hr>



### function getPlayer 

```C++
inline Player & endstone::PlayerEvent::getPlayer () const
```



Returns the player involved in this event




**Returns:**

[**Player**](classendstone_1_1Player.md) who is involved in this event 





        

<hr>



### function ~PlayerEvent 

```C++
endstone::PlayerEvent::~PlayerEvent () override
```




<hr>
## Protected Attributes Documentation




### variable player\_ 

```C++
std::reference_wrapper<Player> endstone::PlayerEvent::player_;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_event.h`

