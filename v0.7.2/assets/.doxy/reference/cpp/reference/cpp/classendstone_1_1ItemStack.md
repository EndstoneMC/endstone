

# Class endstone::ItemStack



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemStack**](classendstone_1_1ItemStack.md)



_Represents a stack of items._ 

* `#include <endstone/inventory/item_stack.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ItemStack**](#function-itemstack-12) () = default<br> |
|   | [**ItemStack**](#function-itemstack-22) (std::string type, [**int**](classendstone_1_1Vector.md) amount=1) <br> |
| virtual [**int**](classendstone_1_1Vector.md) | [**getAmount**](#function-getamount) () const<br>_Gets the amount of items in this stack._  |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**getItemMeta**](#function-getitemmeta) () const<br>_Gets a copy of this_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _'s_[_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
| virtual std::string | [**getType**](#function-gettype) () const<br>_Gets the type of this item._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasItemMeta**](#function-hasitemmeta) () const<br>_Checks to see if any metadata has been defined._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setAmount**](#function-setamount) ([**int**](classendstone_1_1Vector.md) amount) <br>_Sets the amount of items in this stack._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**setItemMeta**](#function-setitemmeta) ([**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) <br>_Set the_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _of this_[_**ItemStack**_](classendstone_1_1ItemStack.md) _._ |
| virtual [**void**](classendstone_1_1Vector.md) | [**setType**](#function-settype) (std::string type) <br>_Sets the type of this item._  |
| virtual  | [**~ItemStack**](#function-itemstack) () = default<br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isEndstoneItemStack**](#function-isendstoneitemstack) () const<br> |




## Public Functions Documentation




### function ItemStack [1/2]

```C++
endstone::ItemStack::ItemStack () = default
```




<hr>



### function ItemStack [2/2]

```C++
inline explicit endstone::ItemStack::ItemStack (
    std::string type,
    int amount=1
) 
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



### function getItemMeta 

_Gets a copy of this_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _'s_[_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._
```C++
inline virtual std::unique_ptr< ItemMeta > endstone::ItemStack::getItemMeta () const
```





**Returns:**

a copy of the current [**ItemStack**](classendstone_1_1ItemStack.md)'s [**ItemMeta**](classendstone_1_1ItemMeta.md) 





        

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



### function setAmount 

_Sets the amount of items in this stack._ 
```C++
inline virtual void endstone::ItemStack::setAmount (
    int amount
) 
```





**Parameters:**


* `amount` New amount of items in this stack 




        

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
inline virtual void endstone::ItemStack::setType (
    std::string type
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

