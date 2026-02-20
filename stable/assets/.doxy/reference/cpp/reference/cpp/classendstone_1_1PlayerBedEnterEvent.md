

# Class endstone::PlayerBedEnterEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerBedEnterEvent**](classendstone_1_1PlayerBedEnterEvent.md)



_Called when a player is almost about to enter the bed._ 

* `#include <endstone/event/player/player_bed_enter_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerBedEnterEvent**](#function-playerbedenterevent) ([**Player**](classendstone_1_1Player.md) & player, [**Block**](classendstone_1_1Block.md) & bed) <br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getBed**](#function-getbed) () const<br>_Returns the bed block involved in this event._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br> |
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

_Returns the bed block involved in this event._ 
```C++
inline Block & endstone::PlayerBedEnterEvent::getBed () const
```





**Returns:**

The bed block involved in this event. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_bed_enter_event.h`

