

# Class endstone::InventoryEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**InventoryEvent**](classendstone_1_1InventoryEvent.md)



_Represents a player related inventory event._ 

* `#include <endstone/event/inventory/inventory_event.h>`



Inherits the following classes: [endstone::Event](classendstone_1_1Event.md)


Inherited by the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md),  [endstone::InventoryCloseEvent](classendstone_1_1InventoryCloseEvent.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**InventoryEvent**](#function-inventoryevent) ([**Inventory**](classendstone_1_1Inventory.md) & inventory) <br> |
|  [**Inventory**](classendstone_1_1Inventory.md) & | [**getInventory**](#function-getinventory) () const<br>_Gets the primary_ [_**Inventory**_](classendstone_1_1Inventory.md) _involved in this transaction._ |


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




### function InventoryEvent 

```C++
inline explicit endstone::InventoryEvent::InventoryEvent (
    Inventory & inventory
) 
```




<hr>



### function getInventory 

_Gets the primary_ [_**Inventory**_](classendstone_1_1Inventory.md) _involved in this transaction._
```C++
inline Inventory & endstone::InventoryEvent::getInventory () const
```





**Returns:**

The primary inventory 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/inventory/inventory_event.h`

