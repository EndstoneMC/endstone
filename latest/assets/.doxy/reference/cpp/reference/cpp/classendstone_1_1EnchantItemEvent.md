

# Class endstone::EnchantItemEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**EnchantItemEvent**](classendstone_1_1EnchantItemEvent.md)



_Called when a player enchants an item at an enchanting table._ [More...](#detailed-description)

* `#include <endstone/event/enchantment/enchant_item_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::unordered\_map&lt; [**const**](classendstone_1_1Identifier.md) [**Enchantment**](classendstone_1_1Enchantment.md) \*, [**int**](classendstone_1_1Identifier.md) &gt; | [**Enchantments**](#typedef-enchantments)  <br> |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**EnchantItemEvent**](classendstone_1_1EnchantItemEvent.md)) <br> |
|   | [**EnchantItemEvent**](#function-enchantitemevent) ([**Inventory**](classendstone_1_1Inventory.md) & inventory, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & enchanter, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & enchant\_block, [**ItemStack**](classendstone_1_1ItemStack.md) item, [**int**](classendstone_1_1Identifier.md) exp\_level\_cost, Enchantments enchants\_to\_add, [**int**](classendstone_1_1Identifier.md) which\_button) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & | [**getEnchantBlock**](#function-getenchantblock) () const<br>_Gets the enchanting table involved in this event._  |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & | [**getEnchanter**](#function-getenchanter) () const<br>_Gets the player enchanting the item._  |
|  [**const**](classendstone_1_1Identifier.md) Enchantments & | [**getEnchantsToAdd**](#function-getenchantstoadd-12) () const<br>_Gets the enchantments that will be applied to the item._  |
|  Enchantments & | [**getEnchantsToAdd**](#function-getenchantstoadd-22) () <br>_Gets the enchantments that will be applied to the item._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getExpLevelCost**](#function-getexplevelcost) () const<br>_Gets the minimum player level required by the selected option._  |
|  [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getItem**](#function-getitem) () const<br>_Gets the item being enchanted._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setExpLevelCost**](#function-setexplevelcost) ([**int**](classendstone_1_1Identifier.md) exp\_level\_cost) <br>_Sets the minimum player level required by the selected option._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setItem**](#function-setitem) ([**ItemStack**](classendstone_1_1ItemStack.md) item) <br>_Sets the item to enchant._  |
|  [**int**](classendstone_1_1Identifier.md) | [**whichButton**](#function-whichbutton) () const<br>_Gets the selected enchanting button._  |


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




**Note:**

Cancelling the event leaves the item, the player's experience levels and the lapis lazuli untouched. 




**Note:**

Bedrock does not reveal a single hinted enchantment for an offer, so every enchantment the offer applies is listed in [**getEnchantsToAdd()**](classendstone_1_1EnchantItemEvent.md#function-getenchantstoadd-22). 





    
## Public Types Documentation




### typedef Enchantments 

```C++
using endstone::EnchantItemEvent::Enchantments =  std::unordered_map<const Enchantment *, int>;
```




<hr>
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::EnchantItemEvent::ENDSTONE_EVENT (
    EnchantItemEvent
) 
```




<hr>



### function EnchantItemEvent 

```C++
inline endstone::EnchantItemEvent::EnchantItemEvent (
    Inventory & inventory,
    const  NotNull < Player > & enchanter,
    const  NotNull < Block > & enchant_block,
    ItemStack item,
    int exp_level_cost,
    Enchantments enchants_to_add,
    int which_button
) 
```




<hr>



### function getEnchantBlock 

_Gets the enchanting table involved in this event._ 
```C++
inline const  NotNull < Block > & endstone::EnchantItemEvent::getEnchantBlock () const
```





**Returns:**

the enchanting table 





        

<hr>



### function getEnchanter 

_Gets the player enchanting the item._ 
```C++
inline const  NotNull < Player > & endstone::EnchantItemEvent::getEnchanter () const
```





**Returns:**

the player enchanting the item 





        

<hr>



### function getEnchantsToAdd [1/2]

_Gets the enchantments that will be applied to the item._ 
```C++
inline const Enchantments & endstone::EnchantItemEvent::getEnchantsToAdd () const
```





**Returns:**

the enchantments and their levels 





        

<hr>



### function getEnchantsToAdd [2/2]

_Gets the enchantments that will be applied to the item._ 
```C++
inline Enchantments & endstone::EnchantItemEvent::getEnchantsToAdd () 
```





**Returns:**

the enchantments and their levels 





        

<hr>



### function getExpLevelCost 

_Gets the minimum player level required by the selected option._ 
```C++
inline int endstone::EnchantItemEvent::getExpLevelCost () const
```



The number of levels consumed remains determined by the selected button.




**Returns:**

the minimum required player level 





        

<hr>



### function getItem 

_Gets the item being enchanted._ 
```C++
inline const  ItemStack & endstone::EnchantItemEvent::getItem () const
```





**Returns:**

the item being enchanted 





        

<hr>



### function setExpLevelCost 

_Sets the minimum player level required by the selected option._ 
```C++
inline void endstone::EnchantItemEvent::setExpLevelCost (
    int exp_level_cost
) 
```





**Parameters:**


* `exp_level_cost` the minimum required player level 




        

<hr>



### function setItem 

_Sets the item to enchant._ 
```C++
inline void endstone::EnchantItemEvent::setItem (
    ItemStack item
) 
```





**Parameters:**


* `item` the item to enchant 




        

<hr>



### function whichButton 

_Gets the selected enchanting button._ 
```C++
inline int endstone::EnchantItemEvent::whichButton () const
```





**Returns:**

the selected button, from 0 to 2 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/enchantment/enchant_item_event.h`

