

# Class endstone::InventoryCloseEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**InventoryCloseEvent**](classendstone_1_1InventoryCloseEvent.md)



_Called when a player closes an inventory._ 

* `#include <endstone/event/inventory/inventory_close_event.h>`



Inherits the following classes: [endstone::InventoryEvent](classendstone_1_1InventoryEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**InventoryCloseEvent**](classendstone_1_1InventoryCloseEvent.md)) <br> |
|   | [**InventoryCloseEvent**](#function-inventorycloseevent) ([**Inventory**](classendstone_1_1Inventory.md) & inventory, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & | [**getPlayer**](#function-getplayer) () const<br>_Returns the player involved in this event._  |


## Public Functions inherited from endstone::InventoryEvent

See [endstone::InventoryEvent](classendstone_1_1InventoryEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**InventoryEvent**](classendstone_1_1InventoryEvent.md#function-inventoryevent) ([**Inventory**](classendstone_1_1Inventory.md) & inventory) <br> |
|  [**Inventory**](classendstone_1_1Inventory.md) & | [**getInventory**](classendstone_1_1InventoryEvent.md#function-getinventory) () const<br>_Gets the primary_ [_**Inventory**_](classendstone_1_1Inventory.md) _involved in this transaction._ |


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
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::InventoryCloseEvent::ENDSTONE_EVENT (
    InventoryCloseEvent
) 
```




<hr>



### function InventoryCloseEvent 

```C++
inline endstone::InventoryCloseEvent::InventoryCloseEvent (
    Inventory & inventory,
    const  NotNull < Player > & player
) 
```




<hr>



### function getPlayer 

_Returns the player involved in this event._ 
```C++
inline const  NotNull < Player > & endstone::InventoryCloseEvent::getPlayer () const
```





**Returns:**

[**Player**](classendstone_1_1Player.md) who is closing the inventory 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/inventory/inventory_close_event.h`

