

# Class endstone::Inventory



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Inventory**](classendstone_1_1Inventory.md)



_Interface to the various inventories._ 

* `#include <endstone/inventory/inventory.h>`





Inherited by the following classes: [endstone::PlayerInventory](classendstone_1_1PlayerInventory.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**addItem**](#function-additem) ([**ItemStack**](classendstone_1_1ItemStack.md) & item) = 0<br>_Stores the given ItemStacks in the inventory. This will try to fill existing stacks and empty slots as well as it can._  |
| virtual void | [**clear**](#function-clear) () = 0<br>_Clears out the whole_ [_**Inventory**_](classendstone_1_1Inventory.md) _._ |
| virtual int | [**first**](#function-first) ([**ItemStack**](classendstone_1_1ItemStack.md) & item) = 0<br>_Returns the first slot in the inventory containing an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with the given stack._ |
| virtual std::vector&lt; std::shared\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; &gt; | [**getContents**](#function-getcontents) () const = 0<br>_Returns all ItemStacks from the inventory._  |
| virtual std::shared\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getItem**](#function-getitem) (int index) const = 0<br>_Returns the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _found in the slot at the given index._ |
| virtual int | [**getMaxStackSize**](#function-getmaxstacksize) () const = 0<br>_Returns the maximum stack size for an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _in this inventory._ |
| virtual int | [**getSize**](#function-getsize) () const = 0<br>_Returns the size of the inventory._  |
| virtual bool | [**isEmpty**](#function-isempty) () const = 0<br>_Check whether this inventory is empty. An inventory is considered to be empty if there are no ItemStacks in any slot of this inventory._  |
| virtual void | [**setItem**](#function-setitem) (int index, std::shared\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; item) = 0<br>_Stores the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _at the given index of the inventory._ |
| virtual  | [**~Inventory**](#function-inventory) () = default<br> |




























## Public Functions Documentation




### function addItem 

_Stores the given ItemStacks in the inventory. This will try to fill existing stacks and empty slots as well as it can._ 
```C++
virtual void endstone::Inventory::addItem (
    ItemStack & item
) = 0
```





**Parameters:**


* `item` The [**ItemStack**](classendstone_1_1ItemStack.md) to add 




        

<hr>



### function clear 

_Clears out the whole_ [_**Inventory**_](classendstone_1_1Inventory.md) _._
```C++
virtual void endstone::Inventory::clear () = 0
```




<hr>



### function first 

_Returns the first slot in the inventory containing an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with the given stack._
```C++
virtual int endstone::Inventory::first (
    ItemStack & item
) = 0
```





**Parameters:**


* `item` The [**ItemStack**](classendstone_1_1ItemStack.md) to match against 



**Returns:**

The slot index of the given [**ItemStack**](classendstone_1_1ItemStack.md) or -1 if not found 





        

<hr>



### function getContents 

_Returns all ItemStacks from the inventory._ 
```C++
virtual std::vector< std::shared_ptr< ItemStack > > endstone::Inventory::getContents () const = 0
```





**Returns:**

An array of ItemStacks from the inventory. Individual items may be null. 





        

<hr>



### function getItem 

_Returns the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _found in the slot at the given index._
```C++
virtual std::shared_ptr< ItemStack > endstone::Inventory::getItem (
    int index
) const = 0
```





**Parameters:**


* `index` The index of the Slot's [**ItemStack**](classendstone_1_1ItemStack.md) to return 



**Returns:**

The [**ItemStack**](classendstone_1_1ItemStack.md) in the slot 





        

<hr>



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



### function isEmpty 

_Check whether this inventory is empty. An inventory is considered to be empty if there are no ItemStacks in any slot of this inventory._ 
```C++
virtual bool endstone::Inventory::isEmpty () const = 0
```





**Returns:**

true if empty, false otherwise 





        

<hr>



### function setItem 

_Stores the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _at the given index of the inventory._
```C++
virtual void endstone::Inventory::setItem (
    int index,
    std::shared_ptr< ItemStack > item
) = 0
```





**Parameters:**


* `index` The index where to put the [**ItemStack**](classendstone_1_1ItemStack.md) 
* `item` The [**ItemStack**](classendstone_1_1ItemStack.md) to set 




        

<hr>



### function ~Inventory 

```C++
virtual endstone::Inventory::~Inventory () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/inventory.h`

