

# Class endstone::ItemStack



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemStack**](classendstone_1_1ItemStack.md)



_Represents a stack of items._ 

* `#include <endstone/inventory/item_stack.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ItemStack**](#function-itemstack-12) ([**const**](classendstone_1_1Vector.md) std::string & type="minecraft:air", [**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) amount=1, [**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) data=0) <br> |
|   | [**ItemStack**](#function-itemstack-22) ([**const**](classendstone_1_1Vector.md) [**ItemStack**](classendstone_1_1ItemStack.md) & stack) <br> |
| virtual std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**clone**](#function-clone) () const<br> |
| virtual [**int**](classendstone_1_1Vector.md) | [**getAmount**](#function-getamount) () const<br>_Gets the amount of items in this stack._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getData**](#function-getdata) () const<br>_Gets the data for this stack of items._  |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**getItemMeta**](#function-getitemmeta) () const<br>_Gets a copy of this_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _'s_[_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
| virtual [**int**](classendstone_1_1Vector.md) | [**getMaxStackSize**](#function-getmaxstacksize) () const<br>_Get the maximum stack size for this item._  |
| virtual std::string | [**getType**](#function-gettype) () const<br>_Gets the type of this item._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasItemMeta**](#function-hasitemmeta) () const<br>_Checks to see if any metadata has been defined._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isSimilar**](#function-issimilar) ([**const**](classendstone_1_1Vector.md) [**ItemStack**](classendstone_1_1ItemStack.md) & other) const<br>_Checks if the two stacks are equal, but does not consider stack size (amount)._  |
|  [**bool**](classendstone_1_1Vector.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Vector.md) [**ItemStack**](classendstone_1_1ItemStack.md) & other) const<br> |
|  [**bool**](classendstone_1_1Vector.md) | [**operator==**](#function-operator_1) ([**const**](classendstone_1_1Vector.md) [**ItemStack**](classendstone_1_1ItemStack.md) & other) const<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**setAmount**](#function-setamount) ([**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) amount) <br>_Sets the amount of items in this stack._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setData**](#function-setdata) ([**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) data) <br>_Sets the data for this stack of items._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**setItemMeta**](#function-setitemmeta) ([**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) <br>_Set the_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _of this_[_**ItemStack**_](classendstone_1_1ItemStack.md) _._ |
| virtual Result&lt; [**void**](classendstone_1_1Vector.md) &gt; | [**setType**](#function-settype) ([**const**](classendstone_1_1Vector.md) std::string & type) <br>_Sets the type of this item._  |
| virtual  | [**~ItemStack**](#function-itemstack) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Result&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**create**](#function-create) ([**const**](classendstone_1_1Vector.md) std::string & type, [**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) amount=1, [**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) data=0) <br> |






















## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isEndstoneItemStack**](#function-isendstoneitemstack) () const<br> |




## Public Functions Documentation




### function ItemStack [1/2]

```C++
inline explicit endstone::ItemStack::ItemStack (
    const std::string & type="minecraft:air",
    const  int amount=1,
    const  int data=0
) 
```




<hr>



### function ItemStack [2/2]

```C++
inline endstone::ItemStack::ItemStack (
    const  ItemStack & stack
) 
```




<hr>



### function clone 

```C++
inline virtual std::unique_ptr< ItemStack > endstone::ItemStack::clone () const
```




<hr>



### function getAmount 

_Gets the amount of items in this stack._ 
```C++
inline virtual int endstone::ItemStack::getAmount () const
```





**Returns:**

Amount of items in this stack 





        

<hr>



### function getData 

_Gets the data for this stack of items._ 
```C++
inline virtual int endstone::ItemStack::getData () const
```





**Returns:**

Data for this item 





        

<hr>



### function getItemMeta 

_Gets a copy of this_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _'s_[_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._
```C++
inline virtual std::unique_ptr< ItemMeta > endstone::ItemStack::getItemMeta () const
```





**Returns:**

a copy of the current [**ItemStack**](classendstone_1_1ItemStack.md)'s [**ItemMeta**](classendstone_1_1ItemMeta.md) 





        

<hr>



### function getMaxStackSize 

_Get the maximum stack size for this item._ 
```C++
inline virtual int endstone::ItemStack::getMaxStackSize () const
```





**Returns:**

The maximum you can stack this item to. 





        

<hr>



### function getType 

_Gets the type of this item._ 
```C++
inline virtual std::string endstone::ItemStack::getType () const
```





**Returns:**

Type of the items in this stack 





        

<hr>



### function hasItemMeta 

_Checks to see if any metadata has been defined._ 
```C++
inline virtual bool endstone::ItemStack::hasItemMeta () const
```





**Returns:**

Returns true if some metadata has been set for this item 





        

<hr>



### function isSimilar 

_Checks if the two stacks are equal, but does not consider stack size (amount)._ 
```C++
inline virtual bool endstone::ItemStack::isSimilar (
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
inline virtual void endstone::ItemStack::setAmount (
    const  int amount
) 
```





**Parameters:**


* `amount` New amount of items in this stack 




        

<hr>



### function setData 

_Sets the data for this stack of items._ 
```C++
inline virtual void endstone::ItemStack::setData (
    const  int data
) 
```





**Parameters:**


* `data` New data for this item 




        

<hr>



### function setItemMeta 

_Set the_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _of this_[_**ItemStack**_](classendstone_1_1ItemStack.md) _._
```C++
inline virtual bool endstone::ItemStack::setItemMeta (
    ItemMeta * meta
) 
```





**Parameters:**


* `meta` new [**ItemMeta**](classendstone_1_1ItemMeta.md), or null to indicate meta data be cleared. 



**Returns:**

True if successfully applied [**ItemMeta**](classendstone_1_1ItemMeta.md) 





        

<hr>



### function setType 

_Sets the type of this item._ 
```C++
inline virtual Result< void > endstone::ItemStack::setType (
    const std::string & type
) 
```





**Parameters:**


* `type` New type to set the items in this stack to 




        

<hr>



### function ~ItemStack 

```C++
virtual endstone::ItemStack::~ItemStack () = default
```




<hr>
## Public Static Functions Documentation




### function create 

```C++
static inline Result< ItemStack > endstone::ItemStack::create (
    const std::string & type,
    const  int amount=1,
    const  int data=0
) 
```




<hr>
## Protected Functions Documentation




### function isEndstoneItemStack 

```C++
inline virtual bool endstone::ItemStack::isEndstoneItemStack () const
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

