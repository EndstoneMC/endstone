

# Class endstone::ItemStack



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemStack**](classendstone_1_1ItemStack.md)



_Represents a stack of items._ 

* `#include <endstone/inventory/item_stack.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ItemStack**](#function-itemstack-14) ([**ItemTypeId**](namespaceendstone.md#typedef-itemtypeid) type, [**int**](classendstone_1_1Identifier.md) amount=1, [**int**](classendstone_1_1Identifier.md) data=0) <br>_An item stack._  |
|   | [**ItemStack**](#function-itemstack-24) ([**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & other) <br> |
|   | [**ItemStack**](#function-itemstack-34) ([**ItemStack**](classendstone_1_1ItemStack.md) && other) noexcept<br> |
|  [**int**](classendstone_1_1Identifier.md) | [**getAmount**](#function-getamount) () const<br>_Gets the amount of items in this stack._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getData**](#function-getdata) () const<br>_Gets the data for this stack of items._  |
|  std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**getItemMeta**](#function-getitemmeta) () const<br>_Gets a copy of this_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _'s_[_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
|  [**int**](classendstone_1_1Identifier.md) | [**getMaxStackSize**](#function-getmaxstacksize) () const<br>_Get the maximum stack size for this item._  |
|  [**CompoundTag**](classendstone_1_1CompoundTag.md) | [**getNbt**](#function-getnbt) () const<br>_Gets the NBT compound tag of this item stack._  |
|  std::string | [**getTranslationKey**](#function-gettranslationkey) () const<br>_Get the translation key, suitable for use in a translation component._  |
|  [**const**](classendstone_1_1Identifier.md) [**ItemType**](classendstone_1_1ItemType.md) & | [**getType**](#function-gettype) () const<br>_Gets the type of this item._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**hasItemMeta**](#function-hasitemmeta) () const<br>_Checks to see if any metadata has been defined._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isSimilar**](#function-issimilar) ([**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & other) const<br>_Checks if the two stacks are equal, but does not consider stack size (amount)._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & other) const<br> |
|  [**ItemStack**](classendstone_1_1ItemStack.md) & | [**operator=**](#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & other) <br> |
|  [**ItemStack**](classendstone_1_1ItemStack.md) & | [**operator=**](#function-operator_2) ([**ItemStack**](classendstone_1_1ItemStack.md) && other) noexcept<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_3) ([**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & other) const<br> |
|  [**void**](classendstone_1_1Identifier.md) | [**setAmount**](#function-setamount) ([**int**](classendstone_1_1Identifier.md) amount) <br>_Sets the amount of items in this stack._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setData**](#function-setdata) ([**int**](classendstone_1_1Identifier.md) data) <br>_Sets the data for this stack of items._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**setItemMeta**](#function-setitemmeta) ([**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) <br>_Set the_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _of this_[_**ItemStack**_](classendstone_1_1ItemStack.md) _._ |
|  [**void**](classendstone_1_1Identifier.md) | [**setNbt**](#function-setnbt) ([**const**](classendstone_1_1Identifier.md) [**CompoundTag**](classendstone_1_1CompoundTag.md) & nbt) <br>_Sets the NBT compound tag of this item stack._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setType**](#function-settype) ([**ItemTypeId**](namespaceendstone.md#typedef-itemtypeid) type) <br>_Sets the type of this item._  |
|   | [**~ItemStack**](#function-itemstack) () = default<br> |




























## Public Functions Documentation




### function ItemStack [1/4]

_An item stack._ 
```C++
inline explicit endstone::ItemStack::ItemStack (
    ItemTypeId type,
    int amount=1,
    int data=0
) 
```





**Parameters:**


* `type` the type 
* `amount` the amount in the stack 
* `data` the data value 




        

<hr>



### function ItemStack [2/4]

```C++
inline endstone::ItemStack::ItemStack (
    const  ItemStack & other
) 
```




<hr>



### function ItemStack [3/4]

```C++
endstone::ItemStack::ItemStack (
    ItemStack && other
) noexcept
```




<hr>



### function getAmount 

_Gets the amount of items in this stack._ 
```C++
inline int endstone::ItemStack::getAmount () const
```





**Returns:**

Amount of items in this stack 





        

<hr>



### function getData 

_Gets the data for this stack of items._ 
```C++
inline int endstone::ItemStack::getData () const
```





**Returns:**

Data for this item 





        

<hr>



### function getItemMeta 

_Gets a copy of this_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _'s_[_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._
```C++
inline std::unique_ptr< ItemMeta > endstone::ItemStack::getItemMeta () const
```





**Returns:**

a copy of the current [**ItemStack**](classendstone_1_1ItemStack.md)'s [**ItemMeta**](classendstone_1_1ItemMeta.md) 





        

<hr>



### function getMaxStackSize 

_Get the maximum stack size for this item._ 
```C++
inline int endstone::ItemStack::getMaxStackSize () const
```





**Returns:**

The maximum you can stack this item to. 





        

<hr>



### function getNbt 

_Gets the NBT compound tag of this item stack._ 
```C++
inline CompoundTag endstone::ItemStack::getNbt () const
```





**Returns:**

the NBT compound tag 





        

<hr>



### function getTranslationKey 

_Get the translation key, suitable for use in a translation component._ 
```C++
inline std::string endstone::ItemStack::getTranslationKey () const
```





**Returns:**

the translation key 





        

<hr>



### function getType 

_Gets the type of this item._ 
```C++
inline const  ItemType & endstone::ItemStack::getType () const
```





**Returns:**

Type of the items in this stack 





        

<hr>



### function hasItemMeta 

_Checks to see if any metadata has been defined._ 
```C++
inline bool endstone::ItemStack::hasItemMeta () const
```





**Returns:**

Returns true if some metadata has been set for this item 





        

<hr>



### function isSimilar 

_Checks if the two stacks are equal, but does not consider stack size (amount)._ 
```C++
inline bool endstone::ItemStack::isSimilar (
    const  ItemStack & other
) const
```





**Parameters:**


* `other` the item stack to compare to 



**Returns:**

true if the two stacks are equal, ignoring the amount 





        

<hr>



### function operator!= 

```C++
inline bool endstone::ItemStack::operator!= (
    const  ItemStack & other
) const
```




<hr>



### function operator= 

```C++
inline ItemStack & endstone::ItemStack::operator= (
    const  ItemStack & other
) 
```




<hr>



### function operator= 

```C++
ItemStack & endstone::ItemStack::operator= (
    ItemStack && other
) noexcept
```




<hr>



### function operator== 

```C++
inline bool endstone::ItemStack::operator== (
    const  ItemStack & other
) const
```




<hr>



### function setAmount 

_Sets the amount of items in this stack._ 
```C++
inline void endstone::ItemStack::setAmount (
    int amount
) 
```





**Parameters:**


* `amount` New amount of items in this stack 




        

<hr>



### function setData 

_Sets the data for this stack of items._ 
```C++
inline void endstone::ItemStack::setData (
    int data
) 
```





**Parameters:**


* `data` New data for this item 




        

<hr>



### function setItemMeta 

_Set the_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _of this_[_**ItemStack**_](classendstone_1_1ItemStack.md) _._
```C++
inline bool endstone::ItemStack::setItemMeta (
    ItemMeta * meta
) 
```





**Parameters:**


* `meta` new [**ItemMeta**](classendstone_1_1ItemMeta.md), or null to indicate meta data be cleared. 



**Returns:**

True if successfully applied [**ItemMeta**](classendstone_1_1ItemMeta.md) 





        

<hr>



### function setNbt 

_Sets the NBT compound tag of this item stack._ 
```C++
inline void endstone::ItemStack::setNbt (
    const  CompoundTag & nbt
) 
```





**Parameters:**


* `nbt` the NBT compound tag to set 




        

<hr>



### function setType 

_Sets the type of this item._ 
```C++
inline void endstone::ItemStack::setType (
    ItemTypeId type
) 
```





**Parameters:**


* `type` New type to set the items in this stack to 




        

<hr>



### function ~ItemStack 

```C++
endstone::ItemStack::~ItemStack () = default
```




<hr>## Friends Documentation





### friend EndstoneItemStack 

```C++
class endstone::ItemStack::EndstoneItemStack (
    core::EndstoneItemStack
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/item_stack.h`

