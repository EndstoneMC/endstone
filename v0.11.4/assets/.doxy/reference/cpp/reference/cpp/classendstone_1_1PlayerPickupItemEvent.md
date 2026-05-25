

# Class endstone::PlayerPickupItemEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerPickupItemEvent**](classendstone_1_1PlayerPickupItemEvent.md)



_Called when a player picks an item up from the ground._ 

* `#include <endstone/event/player/player_pickup_item_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerPickupItemEvent**](classendstone_1_1PlayerPickupItemEvent.md)) <br> |
|   | [**PlayerPickupItemEvent**](#function-playerpickupitemevent) ([**Player**](classendstone_1_1Player.md) & player, [**Item**](classendstone_1_1Item.md) & item) <br> |
|  [**Item**](classendstone_1_1Item.md) & | [**getItem**](#function-getitem) () const<br>_Gets the_ [_**Item**_](classendstone_1_1Item.md) _picked up by the entity._ |


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




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerPickupItemEvent::ENDSTONE_EVENT (
    PlayerPickupItemEvent
) 
```




<hr>



### function PlayerPickupItemEvent 

```C++
inline explicit endstone::PlayerPickupItemEvent::PlayerPickupItemEvent (
    Player & player,
    Item & item
) 
```




<hr>



### function getItem 

_Gets the_ [_**Item**_](classendstone_1_1Item.md) _picked up by the entity._
```C++
inline Item & endstone::PlayerPickupItemEvent::getItem () const
```





**Returns:**

[**Item**](classendstone_1_1Item.md) 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_pickup_item_event.h`

