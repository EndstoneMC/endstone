

# Class endstone::PlayerDropItemEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerDropItemEvent**](classendstone_1_1PlayerDropItemEvent.md)



_Called when a player drops an item from their inventory._ 

* `#include <endstone/event/player/player_drop_item_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerDropItemEvent**](#function-playerdropitemevent) ([**Player**](classendstone_1_1Player.md) & player, const [**ItemStack**](classendstone_1_1ItemStack.md) & drop) <br> |
|  const [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getItem**](#function-getitem) () const<br>_Gets the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _dropped by the player._ |


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




### function PlayerDropItemEvent 

```C++
inline explicit endstone::PlayerDropItemEvent::PlayerDropItemEvent (
    Player & player,
    const ItemStack & drop
) 
```




<hr>



### function getItem 

_Gets the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _dropped by the player._
```C++
inline const ItemStack & endstone::PlayerDropItemEvent::getItem () const
```





**Returns:**

ItemDrop dropped by the player 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_drop_item_event.h`

