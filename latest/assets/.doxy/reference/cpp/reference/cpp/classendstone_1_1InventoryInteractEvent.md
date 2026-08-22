

# Class endstone::InventoryInteractEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**InventoryInteractEvent**](classendstone_1_1InventoryInteractEvent.md)



_An abstract base class for events that describe an interaction between a_ [_**Player**_](classendstone_1_1Player.md) _and the contents of an_[_**Inventory**_](classendstone_1_1Inventory.md) _._

* `#include <endstone/event/inventory/inventory_interact_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**InventoryInteractEvent**](#function-inventoryinteractevent) ([**Inventory**](classendstone_1_1Inventory.md) & inventory, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & who\_clicked) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & | [**getWhoClicked**](#function-getwhoclicked) () const<br>_Gets the player who performed the click._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br>_Cancels this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br>_Sets the cancellation state of this event._  |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Functions Documentation




### function InventoryInteractEvent 

```C++
inline endstone::InventoryInteractEvent::InventoryInteractEvent (
    Inventory & inventory,
    const  NotNull < Player > & who_clicked
) 
```




<hr>



### function getWhoClicked 

_Gets the player who performed the click._ 
```C++
inline const  NotNull < Player > & endstone::InventoryInteractEvent::getWhoClicked () const
```





**Returns:**

The clicking player 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/inventory/inventory_interact_event.h`

