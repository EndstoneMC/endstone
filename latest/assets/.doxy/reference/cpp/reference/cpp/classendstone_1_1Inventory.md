

# Class endstone::Inventory



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Inventory**](classendstone_1_1Inventory.md)



_Interface to the various inventories._ 

* `#include <endstone/inventory/inventory.h>`





Inherited by the following classes: [endstone::PlayerInventory](classendstone_1_1PlayerInventory.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::unordered\_map&lt; int, [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; | [**addItem**](#function-additem-12) (std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; items) = 0<br>_Stores the given ItemStacks in the inventory._  |
|  std::unordered\_map&lt; int, [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; | [**addItem**](#function-additem-22) (Args &&... items) <br>_Stores the given ItemStacks in the inventory._  |
| virtual Result&lt; std::unordered\_map&lt; int, std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; &gt; &gt; | [**all**](#function-all-12) (const std::string & type) const = 0<br>_Finds all slots in the inventory containing any ItemStacks with the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _._ |
| virtual std::unordered\_map&lt; int, std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; &gt; | [**all**](#function-all-22) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br>_Finds all slots in the inventory containing any ItemStacks with the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _._ |
| virtual void | [**clear**](#function-clear-12) (int index) = 0<br>_Clears out a particular slot in the index._  |
| virtual void | [**clear**](#function-clear-22) () = 0<br>_Clears out the whole_ [_**Inventory**_](classendstone_1_1Inventory.md) _._ |
| virtual Result&lt; bool &gt; | [**contains**](#function-contains-13) (const std::string & type) const = 0<br>_Checks if the inventory contains any ItemStacks with the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _._ |
| virtual bool | [**contains**](#function-contains-23) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br>_Checks if the inventory contains any ItemStacks with the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _._ |
| virtual bool | [**contains**](#function-contains-33) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item, int amount) const = 0<br>_Checks if the inventory contains at least the minimum amount specified of exactly matching ItemStacks._  |
| virtual Result&lt; bool &gt; | [**containsAtLeast**](#function-containsatleast-12) (const std::string & type, int amount) const = 0<br>_Checks if the inventory contains any ItemStacks with the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _, adding to at least the minimum amount specified._ |
| virtual bool | [**containsAtLeast**](#function-containsatleast-22) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item, int amount) const = 0<br>_Checks if the inventory contains ItemStacks matching the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _whose amounts sum to at least the minimum amount specified._ |
| virtual Result&lt; int &gt; | [**first**](#function-first-12) (const std::string & type) const = 0<br>_Finds the first slot in the inventory containing an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with the given_[_**ItemType**_](classendstone_1_1ItemType.md) _._ |
| virtual int | [**first**](#function-first-22) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br>_Returns the first slot in the inventory containing an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with the given stack._ |
| virtual int | [**firstEmpty**](#function-firstempty) () const = 0<br>_Returns the first empty Slot._  |
| virtual std::vector&lt; std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; &gt; | [**getContents**](#function-getcontents) () const = 0<br>_Returns all ItemStacks from the inventory._  |
| virtual std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getItem**](#function-getitem) (int index) const = 0<br>_Returns the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _found in the slot at the given index._ |
| virtual int | [**getMaxStackSize**](#function-getmaxstacksize) () const = 0<br>_Returns the maximum stack size for an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _in this inventory._ |
| virtual int | [**getSize**](#function-getsize) () const = 0<br>_Returns the size of the inventory._  |
| virtual bool | [**isEmpty**](#function-isempty) () const = 0<br>_Check whether this inventory is empty. An inventory is considered to be empty if there are no ItemStacks in any slot of this inventory._  |
| virtual Result&lt; void &gt; | [**remove**](#function-remove-12) (const std::string & type) = 0<br>_Removes all stacks in the inventory matching the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _._ |
| virtual void | [**remove**](#function-remove-22) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item) = 0<br>_Removes all stacks in the inventory matching the given stack._  |
| virtual std::unordered\_map&lt; int, [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; | [**removeItem**](#function-removeitem-12) (std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; items) = 0<br>_Removes the given ItemStacks from the inventory._  |
|  std::unordered\_map&lt; int, [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; | [**removeItem**](#function-removeitem-22) (Args &&... items) <br>_Removes the given ItemStacks from the inventory._  |
| virtual Result&lt; void &gt; | [**setContents**](#function-setcontents) (std::vector&lt; const [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; items) = 0<br>_Completely replaces the inventory's contents. Removes all existing contents and replaces it with the ItemStacks given in the array._  |
| virtual void | [**setItem**](#function-setitem) (int index, const [**ItemStack**](classendstone_1_1ItemStack.md) \* item) = 0<br>_Stores the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _at the given index of the inventory._ |
| virtual  | [**~Inventory**](#function-inventory) () = default<br> |




























## Public Functions Documentation




### function addItem [1/2]

_Stores the given ItemStacks in the inventory._ 
```C++
virtual std::unordered_map< int, ItemStack * > endstone::Inventory::addItem (
    std::vector< ItemStack * > items
) = 0
```





**Note:**

This will try to fill existing stacks and empty slots as well as it can.




**Note:**

The returned map contains what it couldn't store, where the key is the index, and the value is the [**ItemStack**](classendstone_1_1ItemStack.md). If all items are stored, it will return an empty map.




**Parameters:**


* `items` The ItemStacks to add



**Returns:**

A map containing items that couldn't be added. 





        

<hr>



### function addItem [2/2]

_Stores the given ItemStacks in the inventory._ 
```C++
template<typename... Args, typename>
inline std::unordered_map< int, ItemStack * > endstone::Inventory::addItem (
    Args &&... items
) 
```





**Note:**

This will try to fill existing stacks and empty slots as well as it can.




**Note:**

The returned map contains what it couldn't store, where the key is the index, and the value is the [**ItemStack**](classendstone_1_1ItemStack.md). If all items are stored, it will return an empty map.




**Parameters:**


* `items` The ItemStacks to add



**Returns:**

A map containing items that couldn't be added. 





        

<hr>



### function all [1/2]

_Finds all slots in the inventory containing any ItemStacks with the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _._
```C++
virtual Result< std::unordered_map< int, std::unique_ptr< ItemStack > > > endstone::Inventory::all (
    const std::string & type
) const = 0
```



The returned map contains entries where, the key is the slot index, and the value is the [**ItemStack**](classendstone_1_1ItemStack.md) in that slot. If no matching [**ItemStack**](classendstone_1_1ItemStack.md) is found, an empty map is returned.




**Parameters:**


* `type` The [**ItemType**](classendstone_1_1ItemType.md) to match against



**Returns:**

A map from slot indexes to item at index 





        

<hr>



### function all [2/2]

_Finds all slots in the inventory containing any ItemStacks with the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _._
```C++
virtual std::unordered_map< int, std::unique_ptr< ItemStack > > endstone::Inventory::all (
    const ItemStack & item
) const = 0
```





**Note:**

This will only match slots if both the type and the amount of the stack match


The returned map contains entries where, the key is the slot index, and the value is the [**ItemStack**](classendstone_1_1ItemStack.md) in that slot. If no matching [**ItemStack**](classendstone_1_1ItemStack.md) is found, an empty map is returned.




**Parameters:**


* `item` The [**ItemStack**](classendstone_1_1ItemStack.md) to match against



**Returns:**

A map from slot indexes to item at index 





        

<hr>



### function clear [1/2]

_Clears out a particular slot in the index._ 
```C++
virtual void endstone::Inventory::clear (
    int index
) = 0
```





**Parameters:**


* `index` The index to empty. 




        

<hr>



### function clear [2/2]

_Clears out the whole_ [_**Inventory**_](classendstone_1_1Inventory.md) _._
```C++
virtual void endstone::Inventory::clear () = 0
```




<hr>



### function contains [1/3]

_Checks if the inventory contains any ItemStacks with the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _._
```C++
virtual Result< bool > endstone::Inventory::contains (
    const std::string & type
) const = 0
```





**Parameters:**


* `type` The item type to check for



**Returns:**

true if an [**ItemStack**](classendstone_1_1ItemStack.md) is found with the given [**ItemType**](classendstone_1_1ItemType.md) 





        

<hr>



### function contains [2/3]

_Checks if the inventory contains any ItemStacks with the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _._
```C++
virtual bool endstone::Inventory::contains (
    const ItemStack & item
) const = 0
```





**Note:**

This will only return true if both the type and the amount of the stack match.




**Parameters:**


* `item` The [**ItemStack**](classendstone_1_1ItemStack.md) to match against 



**Returns:**

true if any exactly matching ItemStacks were found, false otherwise 





        

<hr>



### function contains [3/3]

_Checks if the inventory contains at least the minimum amount specified of exactly matching ItemStacks._ 
```C++
virtual bool endstone::Inventory::contains (
    const ItemStack & item,
    int amount
) const = 0
```





**Note:**

An [**ItemStack**](classendstone_1_1ItemStack.md) only counts if both the type and the amount of the stack match.




**Parameters:**


* `item` the [**ItemStack**](classendstone_1_1ItemStack.md) to match against 
* `amount` how many identical stacks to check for



**Returns:**

true if amount less than 1 or if amount of exactly matching ItemStacks were found, false otherwise 





        

<hr>



### function containsAtLeast [1/2]

_Checks if the inventory contains any ItemStacks with the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _, adding to at least the minimum amount specified._
```C++
virtual Result< bool > endstone::Inventory::containsAtLeast (
    const std::string & type,
    int amount
) const = 0
```





**Parameters:**


* `type` The [**ItemType**](classendstone_1_1ItemType.md) to check for 
* `amount` The minimum amount



**Returns:**

true if amount is less than 1, true if enough ItemStacks were found to add to the given amount 





        

<hr>



### function containsAtLeast [2/2]

_Checks if the inventory contains ItemStacks matching the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _whose amounts sum to at least the minimum amount specified._
```C++
virtual bool endstone::Inventory::containsAtLeast (
    const ItemStack & item,
    int amount
) const = 0
```





**Parameters:**


* `item` the [**ItemStack**](classendstone_1_1ItemStack.md) to match against 
* `amount` the minimum amount



**Returns:**

true if amount less than 1 or enough ItemStacks were found to add to the given amount, false otherwise 





        

<hr>



### function first [1/2]

_Finds the first slot in the inventory containing an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with the given_[_**ItemType**_](classendstone_1_1ItemType.md) _._
```C++
virtual Result< int > endstone::Inventory::first (
    const std::string & type
) const = 0
```





**Parameters:**


* `type` The [**ItemType**](classendstone_1_1ItemType.md) to look for



**Returns:**

The slot index of the given [**ItemType**](classendstone_1_1ItemType.md) or -1 if not found 





        

<hr>



### function first [2/2]

_Returns the first slot in the inventory containing an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with the given stack._
```C++
virtual int endstone::Inventory::first (
    const ItemStack & item
) const = 0
```





**Parameters:**


* `item` The [**ItemStack**](classendstone_1_1ItemStack.md) to match against



**Returns:**

The slot index of the given [**ItemStack**](classendstone_1_1ItemStack.md) or -1 if not found 





        

<hr>



### function firstEmpty 

_Returns the first empty Slot._ 
```C++
virtual int endstone::Inventory::firstEmpty () const = 0
```





**Returns:**

The first empty Slot found, or -1 if no empty slots. 





        

<hr>



### function getContents 

_Returns all ItemStacks from the inventory._ 
```C++
virtual std::vector< std::unique_ptr< ItemStack > > endstone::Inventory::getContents () const = 0
```





**Returns:**

An array of ItemStacks from the inventory. Individual items may be null. 





        

<hr>



### function getItem 

_Returns the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _found in the slot at the given index._
```C++
virtual std::unique_ptr< ItemStack > endstone::Inventory::getItem (
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



### function remove [1/2]

_Removes all stacks in the inventory matching the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _._
```C++
virtual Result< void > endstone::Inventory::remove (
    const std::string & type
) = 0
```





**Parameters:**


* `type` The [**ItemType**](classendstone_1_1ItemType.md) to remove 




        

<hr>



### function remove [2/2]

_Removes all stacks in the inventory matching the given stack._ 
```C++
virtual void endstone::Inventory::remove (
    const ItemStack & item
) = 0
```





**Note:**

This will only match a slot if both the type and the amount of the stack match




**Parameters:**


* `item` The [**ItemStack**](classendstone_1_1ItemStack.md) to match against 




        

<hr>



### function removeItem [1/2]

_Removes the given ItemStacks from the inventory._ 
```C++
virtual std::unordered_map< int, ItemStack * > endstone::Inventory::removeItem (
    std::vector< ItemStack * > items
) = 0
```





**Note:**

It will try to remove 'as much as possible' from the types and amounts you give as arguments.




**Note:**

The returned HashMap contains what it couldn't remove, where the key is the index, and the value is the [**ItemStack**](classendstone_1_1ItemStack.md). If all the given ItemStacks are removed, it will return an empty map.




**Parameters:**


* `items` The ItemStacks to remove



**Returns:**

A map containing items that couldn't be removed. 





        

<hr>



### function removeItem [2/2]

_Removes the given ItemStacks from the inventory._ 
```C++
template<typename... Args, typename>
inline std::unordered_map< int, ItemStack * > endstone::Inventory::removeItem (
    Args &&... items
) 
```





**Note:**

It will try to remove 'as much as possible' from the types and amounts you give as arguments.




**Note:**

The returned HashMap contains what it couldn't remove, where the key is the index, and the value is the [**ItemStack**](classendstone_1_1ItemStack.md). If all the given ItemStacks are removed, it will return an empty map.




**Parameters:**


* `items` The ItemStacks to remove 



**Returns:**

A map containing items that couldn't be removed. 





        

<hr>



### function setContents 

_Completely replaces the inventory's contents. Removes all existing contents and replaces it with the ItemStacks given in the array._ 
```C++
virtual Result< void > endstone::Inventory::setContents (
    std::vector< const ItemStack * > items
) = 0
```





**Parameters:**


* `items` A complete replacement for the contents; the length must be less than or equal to [**getSize()**](classendstone_1_1Inventory.md#function-getsize). 




        

<hr>



### function setItem 

_Stores the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _at the given index of the inventory._
```C++
virtual void endstone::Inventory::setItem (
    int index,
    const ItemStack * item
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

