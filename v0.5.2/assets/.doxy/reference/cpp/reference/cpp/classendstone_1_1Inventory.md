

# Class endstone::Inventory



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Inventory**](classendstone_1_1Inventory.md)



_Interface to the various inventories._ 

* `#include <endstone/inventory/inventory.h>`





Inherited by the following classes: [endstone::PlayerInventory](classendstone_1_1PlayerInventory.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual int | [**getMaxStackSize**](#function-getmaxstacksize) () const = 0<br>_Returns the maximum stack size for an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _in this inventory._ |
| virtual int | [**getSize**](#function-getsize) () const = 0<br>_Returns the size of the inventory._  |
| virtual  | [**~Inventory**](#function-inventory) () = default<br> |




























## Public Functions Documentation




### function getMaxStackSize 

_Returns the maximum stack size for an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _in this inventory._
```C++
virtual int endstone::Inventory::getMaxStackSize () const = 0
```





**Returns:**

The maximum size for an [**ItemStack**](classendstone_1_1ItemStack.md) in this inventory. 





        

<hr>



### function getSize 

_Returns the size of the inventory._ 
```C++
virtual int endstone::Inventory::getSize () const = 0
```





**Returns:**

The size of the inventory 





        

<hr>



### function ~Inventory 

```C++
virtual endstone::Inventory::~Inventory () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/inventory.h`

