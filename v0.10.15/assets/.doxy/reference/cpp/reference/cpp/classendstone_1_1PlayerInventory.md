

# Class endstone::PlayerInventory



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerInventory**](classendstone_1_1PlayerInventory.md)



_Interface to the inventory of a_ [_**Player**_](classendstone_1_1Player.md) _, including the four armor slots and any extra slots._

* `#include <endstone/inventory/player_inventory.h>`



Inherits the following classes: [endstone::Inventory](classendstone_1_1Inventory.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getBoots**](#function-getboots) () const = 0<br>_Return the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _from the boots slot._ |
| virtual std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getChestplate**](#function-getchestplate) () const = 0<br>_Return the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _from the chestplate slot._ |
| virtual int | [**getHeldItemSlot**](#function-gethelditemslot) () const = 0<br>_Get the slot number of the currently held item._  |
| virtual std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getHelmet**](#function-gethelmet) () const = 0<br>_Return the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _from the helmet slot._ |
| virtual std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getItemInMainHand**](#function-getiteminmainhand) () const = 0<br>_Gets the item the player is currently holding in their main hand._  |
| virtual std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getItemInOffHand**](#function-getiteminoffhand) () const = 0<br>_Gets the item the player is currently holding in their off hand._  |
| virtual std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getLeggings**](#function-getleggings) () const = 0<br>_Return the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _from the leg slot._ |
| virtual void | [**setBoots**](#function-setboots) (const [**ItemStack**](classendstone_1_1ItemStack.md) \* boots) = 0<br>_Put the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _into the boots slot._ |
| virtual void | [**setChestplate**](#function-setchestplate) (const [**ItemStack**](classendstone_1_1ItemStack.md) \* chestplate) = 0<br>_Put the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _into the chestplate slot._ |
| virtual Result&lt; void &gt; | [**setHeldItemSlot**](#function-sethelditemslot) (int slot) = 0<br>_Set the slot number of the currently held item._  |
| virtual void | [**setHelmet**](#function-sethelmet) (const [**ItemStack**](classendstone_1_1ItemStack.md) \* helmet) = 0<br>_Put the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _into the helmet slot._ |
| virtual void | [**setItemInMainHand**](#function-setiteminmainhand) (const [**ItemStack**](classendstone_1_1ItemStack.md) \* item) = 0<br>_Sets the item the player is holding in their main hand._  |
| virtual void | [**setItemInOffHand**](#function-setiteminoffhand) (const [**ItemStack**](classendstone_1_1ItemStack.md) \* item) = 0<br>_Sets the item the player is holding in their off hand._  |
| virtual void | [**setLeggings**](#function-setleggings) (const [**ItemStack**](classendstone_1_1ItemStack.md) \* leggings) = 0<br>_Put the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _into the leggings slot._ |


## Public Functions inherited from endstone::Inventory

See [endstone::Inventory](classendstone_1_1Inventory.md)

| Type | Name |
| ---: | :--- |
| virtual std::unordered\_map&lt; int, [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; | [**addItem**](classendstone_1_1Inventory.md#function-additem-12) (std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; items) = 0<br>_Stores the given ItemStacks in the inventory._  |
|  std::unordered\_map&lt; int, [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; | [**addItem**](classendstone_1_1Inventory.md#function-additem-22) (Args &&... items) <br>_Stores the given ItemStacks in the inventory._  |
| virtual Result&lt; std::unordered\_map&lt; int, std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; &gt; &gt; | [**all**](classendstone_1_1Inventory.md#function-all-12) (const std::string & type) const = 0<br>_Finds all slots in the inventory containing any ItemStacks with the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _._ |
| virtual std::unordered\_map&lt; int, std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; &gt; | [**all**](classendstone_1_1Inventory.md#function-all-22) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br>_Finds all slots in the inventory containing any ItemStacks with the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _._ |
| virtual void | [**clear**](classendstone_1_1Inventory.md#function-clear-12) (int index) = 0<br>_Clears out a particular slot in the index._  |
| virtual void | [**clear**](classendstone_1_1Inventory.md#function-clear-22) () = 0<br>_Clears out the whole_ [_**Inventory**_](classendstone_1_1Inventory.md) _._ |
| virtual Result&lt; bool &gt; | [**contains**](classendstone_1_1Inventory.md#function-contains-13) (const std::string & type) const = 0<br>_Checks if the inventory contains any ItemStacks with the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _._ |
| virtual bool | [**contains**](classendstone_1_1Inventory.md#function-contains-23) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br>_Checks if the inventory contains any ItemStacks with the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _._ |
| virtual bool | [**contains**](classendstone_1_1Inventory.md#function-contains-33) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item, int amount) const = 0<br>_Checks if the inventory contains at least the minimum amount specified of exactly matching ItemStacks._  |
| virtual Result&lt; bool &gt; | [**containsAtLeast**](classendstone_1_1Inventory.md#function-containsatleast-12) (const std::string & type, int amount) const = 0<br>_Checks if the inventory contains any ItemStacks with the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _, adding to at least the minimum amount specified._ |
| virtual bool | [**containsAtLeast**](classendstone_1_1Inventory.md#function-containsatleast-22) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item, int amount) const = 0<br>_Checks if the inventory contains ItemStacks matching the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _whose amounts sum to at least the minimum amount specified._ |
| virtual Result&lt; int &gt; | [**first**](classendstone_1_1Inventory.md#function-first-12) (const std::string & type) const = 0<br>_Finds the first slot in the inventory containing an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with the given_[_**ItemType**_](classendstone_1_1ItemType.md) _._ |
| virtual int | [**first**](classendstone_1_1Inventory.md#function-first-22) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br>_Returns the first slot in the inventory containing an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with the given stack._ |
| virtual int | [**firstEmpty**](classendstone_1_1Inventory.md#function-firstempty) () const = 0<br>_Returns the first empty Slot._  |
| virtual std::vector&lt; std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; &gt; | [**getContents**](classendstone_1_1Inventory.md#function-getcontents) () const = 0<br>_Returns all ItemStacks from the inventory._  |
| virtual std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getItem**](classendstone_1_1Inventory.md#function-getitem) (int index) const = 0<br>_Returns the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _found in the slot at the given index._ |
| virtual int | [**getMaxStackSize**](classendstone_1_1Inventory.md#function-getmaxstacksize) () const = 0<br>_Returns the maximum stack size for an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _in this inventory._ |
| virtual int | [**getSize**](classendstone_1_1Inventory.md#function-getsize) () const = 0<br>_Returns the size of the inventory._  |
| virtual bool | [**isEmpty**](classendstone_1_1Inventory.md#function-isempty) () const = 0<br>_Check whether this inventory is empty. An inventory is considered to be empty if there are no ItemStacks in any slot of this inventory._  |
| virtual Result&lt; void &gt; | [**remove**](classendstone_1_1Inventory.md#function-remove-12) (const std::string & type) = 0<br>_Removes all stacks in the inventory matching the given_ [_**ItemType**_](classendstone_1_1ItemType.md) _._ |
| virtual void | [**remove**](classendstone_1_1Inventory.md#function-remove-22) (const [**ItemStack**](classendstone_1_1ItemStack.md) & item) = 0<br>_Removes all stacks in the inventory matching the given stack._  |
| virtual std::unordered\_map&lt; int, [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; | [**removeItem**](classendstone_1_1Inventory.md#function-removeitem-12) (std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; items) = 0<br>_Removes the given ItemStacks from the inventory._  |
|  std::unordered\_map&lt; int, [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; | [**removeItem**](classendstone_1_1Inventory.md#function-removeitem-22) (Args &&... items) <br>_Removes the given ItemStacks from the inventory._  |
| virtual Result&lt; void &gt; | [**setContents**](classendstone_1_1Inventory.md#function-setcontents) (std::vector&lt; const [**ItemStack**](classendstone_1_1ItemStack.md) \* &gt; items) = 0<br>_Completely replaces the inventory's contents. Removes all existing contents and replaces it with the ItemStacks given in the array._  |
| virtual void | [**setItem**](classendstone_1_1Inventory.md#function-setitem) (int index, const [**ItemStack**](classendstone_1_1ItemStack.md) \* item) = 0<br>_Stores the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _at the given index of the inventory._ |
| virtual  | [**~Inventory**](classendstone_1_1Inventory.md#function-inventory) () = default<br> |






















































## Public Functions Documentation




### function getBoots 

_Return the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _from the boots slot._
```C++
virtual std::unique_ptr< ItemStack > endstone::PlayerInventory::getBoots () const = 0
```





**Returns:**

The [**ItemStack**](classendstone_1_1ItemStack.md) in the boots slot 





        

<hr>



### function getChestplate 

_Return the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _from the chestplate slot._
```C++
virtual std::unique_ptr< ItemStack > endstone::PlayerInventory::getChestplate () const = 0
```





**Returns:**

The [**ItemStack**](classendstone_1_1ItemStack.md) in the chestplate slot 





        

<hr>



### function getHeldItemSlot 

_Get the slot number of the currently held item._ 
```C++
virtual int endstone::PlayerInventory::getHeldItemSlot () const = 0
```





**Returns:**

Held item slot number 





        

<hr>



### function getHelmet 

_Return the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _from the helmet slot._
```C++
virtual std::unique_ptr< ItemStack > endstone::PlayerInventory::getHelmet () const = 0
```





**Returns:**

The [**ItemStack**](classendstone_1_1ItemStack.md) in the helmet slot 





        

<hr>



### function getItemInMainHand 

_Gets the item the player is currently holding in their main hand._ 
```C++
virtual std::unique_ptr< ItemStack > endstone::PlayerInventory::getItemInMainHand () const = 0
```





**Returns:**

the currently held item 





        

<hr>



### function getItemInOffHand 

_Gets the item the player is currently holding in their off hand._ 
```C++
virtual std::unique_ptr< ItemStack > endstone::PlayerInventory::getItemInOffHand () const = 0
```





**Returns:**

the currently held item 





        

<hr>



### function getLeggings 

_Return the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _from the leg slot._
```C++
virtual std::unique_ptr< ItemStack > endstone::PlayerInventory::getLeggings () const = 0
```





**Returns:**

The [**ItemStack**](classendstone_1_1ItemStack.md) in the leg slot 





        

<hr>



### function setBoots 

_Put the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _into the boots slot._
```C++
virtual void endstone::PlayerInventory::setBoots (
    const ItemStack * boots
) = 0
```



This does not check if the [**ItemStack**](classendstone_1_1ItemStack.md) is a pair of boots.




**Parameters:**


* `boots` The [**ItemStack**](classendstone_1_1ItemStack.md) to use as boots 




        

<hr>



### function setChestplate 

_Put the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _into the chestplate slot._
```C++
virtual void endstone::PlayerInventory::setChestplate (
    const ItemStack * chestplate
) = 0
```



This does not check if the [**ItemStack**](classendstone_1_1ItemStack.md) is a chestplate.




**Parameters:**


* `chestplate` The [**ItemStack**](classendstone_1_1ItemStack.md) to use as chestplate 




        

<hr>



### function setHeldItemSlot 

_Set the slot number of the currently held item._ 
```C++
virtual Result< void > endstone::PlayerInventory::setHeldItemSlot (
    int slot
) = 0
```



This validates whether the slot is between 0 and 8 inclusive.




**Parameters:**


* `slot` The new slot number 




        

<hr>



### function setHelmet 

_Put the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _into the helmet slot._
```C++
virtual void endstone::PlayerInventory::setHelmet (
    const ItemStack * helmet
) = 0
```



This does not check if the [**ItemStack**](classendstone_1_1ItemStack.md) is a helmet.




**Parameters:**


* `helmet` The [**ItemStack**](classendstone_1_1ItemStack.md) to use as helmet 




        

<hr>



### function setItemInMainHand 

_Sets the item the player is holding in their main hand._ 
```C++
virtual void endstone::PlayerInventory::setItemInMainHand (
    const ItemStack * item
) = 0
```





**Parameters:**


* `item` The item to put into the player's hand 




        

<hr>



### function setItemInOffHand 

_Sets the item the player is holding in their off hand._ 
```C++
virtual void endstone::PlayerInventory::setItemInOffHand (
    const ItemStack * item
) = 0
```





**Parameters:**


* `item` The item to put into the player's hand 




        

<hr>



### function setLeggings 

_Put the given_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _into the leggings slot._
```C++
virtual void endstone::PlayerInventory::setLeggings (
    const ItemStack * leggings
) = 0
```



This does not check if the [**ItemStack**](classendstone_1_1ItemStack.md) is a pair of leggings.




**Parameters:**


* `leggings` The [**ItemStack**](classendstone_1_1ItemStack.md) to use as leggings 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/player_inventory.h`

