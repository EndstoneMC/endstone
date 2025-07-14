

# Class endstone::PlayerPickupItemEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerPickupItemEvent**](classendstone_1_1PlayerPickupItemEvent.md)



_Called when a player picks an item up from the ground._ 

* `#include <endstone/event/player/player_pickup_item_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerPickupItemEvent**](#function-playerpickupitemevent) ([**Player**](classendstone_1_1Player.md) & player, const [**ItemStack**](classendstone_1_1ItemStack.md) & item) <br> |
|  const [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getItem**](#function-getitem) () <br>_Gets the Item picked up by the entity._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual bool | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual void | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) (bool cancel) <br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual bool | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual void | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) (bool cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Functions Documentation




### function PlayerPickupItemEvent 

```C++
inline explicit endstone::PlayerPickupItemEvent::PlayerPickupItemEvent (
    Player & player,
    const ItemStack & item
) 
```




<hr>



### function getItem 

_Gets the Item picked up by the entity._ 
```C++
inline const ItemStack & endstone::PlayerPickupItemEvent::getItem () 
```





**Returns:**

Item 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_pickup_item_event.h`

