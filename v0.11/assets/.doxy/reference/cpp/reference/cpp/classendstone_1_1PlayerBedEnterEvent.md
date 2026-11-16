

# Class endstone::PlayerBedEnterEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerBedEnterEvent**](classendstone_1_1PlayerBedEnterEvent.md)



_Called when a player is almost about to enter the bed._ 

* `#include <endstone/event/player/player_bed_enter_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerBedEnterEvent**](#function-playerbedenterevent) ([**Player**](classendstone_1_1Player.md) & player, [**Block**](classendstone_1_1Block.md) & bed) <br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getBed**](#function-getbed) () const<br>_Represents the default possible outcomes of this event._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual bool | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual void | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) (bool cancel) override<br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual bool | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual void | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) (bool cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Functions Documentation




### function PlayerBedEnterEvent 

```C++
inline explicit endstone::PlayerBedEnterEvent::PlayerBedEnterEvent (
    Player & player,
    Block & bed
) 
```




<hr>



### function getBed 

_Represents the default possible outcomes of this event._ 
```C++
inline Block & endstone::PlayerBedEnterEvent::getBed () const
```



The player will enter the bed. The dimension doesn't allow sleeping or saving the spawn point (eg,Nether, The End). Entering the bed is prevented due to it not being night nor thundering currently. Entering the bed is prevented due to the player being too far away. Entering the bed is prevented due to there being monsters nearby. Entering the bed is prevented due to there being some other problem.


This describes the default outcome of this event.




**Returns:**

The bed enter result representing the default outcome of this event


This controls the action to take with the bed that was clicked on.




**Returns:**

the action to take with the interacted bed


Sets the action to take with the interacted bed.



* Result::Allow will result in the player sleeping, regardless of the default outcome described by getBedEnterResult().
* Result::Deny will prevent the player from sleeping. This has the same effect as canceling the event
* Result::Default will result in the outcome described by getBedEnterResult().






**Parameters:**


* `use_bed` the action to take with the interacted bed

Gets the cancellation state of this event. Set to true if you want to prevent the player from sleeping.




**Note:**

Canceling the event has the same effect as setting useBed() to Result::Deny.




**Returns:**

boolean cancellation state  will still pass to other plugins.




**Note:**

Canceling this event will prevent use of the bed.




**Parameters:**


* `cancel` true if you wish to cancel this event

Returns the bed block involved in this event.




**Returns:**

The bed block involved in this event. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_bed_enter_event.h`

