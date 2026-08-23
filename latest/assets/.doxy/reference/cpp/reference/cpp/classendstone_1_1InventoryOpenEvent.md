

# Class endstone::InventoryOpenEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**InventoryOpenEvent**](classendstone_1_1InventoryOpenEvent.md)



_Called when a player opens an inventory._ [More...](#detailed-description)

* `#include <endstone/event/inventory/inventory_open_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**InventoryOpenEvent**](classendstone_1_1InventoryOpenEvent.md)) <br> |
|   | [**InventoryOpenEvent**](#function-inventoryopenevent) ([**Inventory**](classendstone_1_1Inventory.md) & inventory, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & | [**getPlayer**](#function-getplayer) () const<br>_Returns the player involved in this event._  |


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
















































































## Detailed Description


If this event is cancelled, the inventory will not be opened and the player will not see the container screen. 


    
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::InventoryOpenEvent::ENDSTONE_EVENT (
    InventoryOpenEvent
) 
```




<hr>



### function InventoryOpenEvent 

```C++
inline endstone::InventoryOpenEvent::InventoryOpenEvent (
    Inventory & inventory,
    const  NotNull < Player > & player
) 
```




<hr>



### function getPlayer 

_Returns the player involved in this event._ 
```C++
inline const  NotNull < Player > & endstone::InventoryOpenEvent::getPlayer () const
```





**Returns:**

[**Player**](classendstone_1_1Player.md) who is opening the inventory 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/inventory/inventory_open_event.h`

