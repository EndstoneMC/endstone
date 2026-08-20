

# Class endstone::PlayerExpChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerExpChangeEvent**](classendstone_1_1PlayerExpChangeEvent.md)



_Called when a player's experience changes._ 

* `#include <endstone/event/player/player_exp_change_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerExpChangeEvent**](classendstone_1_1PlayerExpChangeEvent.md)) <br> |
|   | [**PlayerExpChangeEvent**](#function-playerexpchangeevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**int**](classendstone_1_1Identifier.md) amount) <br> |
|  [**int**](classendstone_1_1Identifier.md) | [**getAmount**](#function-getamount) () const<br>_Gets the amount of experience gained by the player._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setAmount**](#function-setamount) ([**int**](classendstone_1_1Identifier.md) amount) <br>_Sets the amount of experience the player will be given._  |


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
endstone::PlayerExpChangeEvent::ENDSTONE_EVENT (
    PlayerExpChangeEvent
) 
```




<hr>



### function PlayerExpChangeEvent 

```C++
inline endstone::PlayerExpChangeEvent::PlayerExpChangeEvent (
    const  NotNull < Player > & player,
    int amount
) 
```




<hr>



### function getAmount 

_Gets the amount of experience gained by the player._ 
```C++
inline int endstone::PlayerExpChangeEvent::getAmount () const
```





**Returns:**

the amount of experience gained 





        

<hr>



### function setAmount 

_Sets the amount of experience the player will be given._ 
```C++
inline void endstone::PlayerExpChangeEvent::setAmount (
    int amount
) 
```





**Parameters:**


* `amount` the amount of experience to give 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_exp_change_event.h`

