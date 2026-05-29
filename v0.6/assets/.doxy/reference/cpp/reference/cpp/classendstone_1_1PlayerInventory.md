

# Class endstone::PlayerInventory



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerInventory**](classendstone_1_1PlayerInventory.md)



_Interface to the inventory of a_ [_**Player**_](classendstone_1_1Player.md) _, including the four armor slots and any extra slots._

* `#include <endstone/inventory/player_inventory.h>`



Inherits the following classes: [endstone::Inventory](classendstone_1_1Inventory.md)
























































## Public Functions inherited from endstone::Inventory

See [endstone::Inventory](classendstone_1_1Inventory.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Vector.md) | [**addItem**](classendstone_1_1Inventory.md#function-additem) ([**ItemStack**](classendstone_1_1ItemStack.md) & item) = 0<br>_Stores the given ItemStacks in the inventory. This will try to fill existing stacks and empty slots as well as it can._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**clear**](classendstone_1_1Inventory.md#function-clear) () = 0<br>_Clears out the whole_ [_**Inventory**_](classendstone_1_1Inventory.md) _._ |
| virtual [**int**](classendstone_1_1Vector.md) | [**first**](classendstone_1_1Inventory.md#function-first) ([**ItemStack**](classendstone_1_1ItemStack.md) & item) = 0<br>_Returns the first slot in the inventory containing an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _with the given stack._ |
| virtual std::vector&lt; std::shared\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; &gt; | [**getContents**](classendstone_1_1Inventory.md#function-getcontents) () const = 0<br>_Returns all ItemStacks from the inventory._  |
| virtual std::shared\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getItem**](classendstone_1_1Inventory.md#function-getitem) ([**int**](classendstone_1_1Vector.md) index) const = 0<br>_Returns the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _found in the slot at the given index._ |
| virtual [**int**](classendstone_1_1Vector.md) | [**getMaxStackSize**](classendstone_1_1Inventory.md#function-getmaxstacksize) () const = 0<br>_Returns the maximum stack size for an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _in this inventory._ |
| virtual [**int**](classendstone_1_1Vector.md) | [**getSize**](classendstone_1_1Inventory.md#function-getsize) () const = 0<br>_Returns the size of the inventory._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isEmpty**](classendstone_1_1Inventory.md#function-isempty) () const = 0<br>_Check whether this inventory is empty. An inventory is considered to be empty if there are no ItemStacks in any slot of this inventory._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setItem**](classendstone_1_1Inventory.md#function-setitem) ([**int**](classendstone_1_1Vector.md) index, std::shared\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; item) = 0<br>_Stores the_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _at the given index of the inventory._ |
| virtual  | [**~Inventory**](classendstone_1_1Inventory.md#function-inventory) () = default<br> |























































------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/player_inventory.h`

