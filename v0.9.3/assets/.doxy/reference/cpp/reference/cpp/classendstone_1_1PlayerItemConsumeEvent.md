

# Class endstone::PlayerItemConsumeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerItemConsumeEvent**](classendstone_1_1PlayerItemConsumeEvent.md)



_Called when a player is finishing consuming an item (food, potion, milk bucket)._ [More...](#detailed-description)

* `#include <endstone/event/player/player_item_consume_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerItemConsumeEvent**](#function-playeritemconsumeevent) ([**Player**](classendstone_1_1Player.md) & player, const [**ItemStack**](classendstone_1_1ItemStack.md) & item, [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) hand) <br> |
|  [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) | [**getHand**](#function-gethand) () const<br>_Get the hand used to consume the item._  |
|  std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getItem**](#function-getitem) () const<br>_Gets the item that is being consumed._  |


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
















































































## Detailed Description




**Note:**

If the [**ItemStack**](classendstone_1_1ItemStack.md) is modified the server will use the effects of the new item and not remove the original one from the player's inventory.




**Note:**

If the event is cancelled the effect will not be applied and the item will not be removed from the player's inventory. 





    
## Public Functions Documentation




### function PlayerItemConsumeEvent 

```C++
inline explicit endstone::PlayerItemConsumeEvent::PlayerItemConsumeEvent (
    Player & player,
    const ItemStack & item,
    EquipmentSlot hand
) 
```




<hr>



### function getHand 

_Get the hand used to consume the item._ 
```C++
inline EquipmentSlot endstone::PlayerItemConsumeEvent::getHand () const
```





**Returns:**

the hand 





        

<hr>



### function getItem 

_Gets the item that is being consumed._ 
```C++
inline std::unique_ptr< ItemStack > endstone::PlayerItemConsumeEvent::getItem () const
```





**Note:**

Modifying the returned item will have no effect, you must use setItem(ItemStack) instead.




**Returns:**

an [**ItemStack**](classendstone_1_1ItemStack.md) for the item being consumed 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_item_consume_event.h`

