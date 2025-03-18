

# Class endstone::ItemStack



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemStack**](classendstone_1_1ItemStack.md)



_Represents a stack of items._ 

* `#include <endstone/inventory/item_stack.h>`



Inherits the following classes: std::enable_shared_from_this< ItemStack >


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ItemStack**](#function-itemstack-12) () = default<br> |
|   | [**ItemStack**](#function-itemstack-22) (std::string type, [**int**](classendstone_1_1Vector.md) amount=1) <br> |
| virtual [**int**](classendstone_1_1Vector.md) | [**getAmount**](#function-getamount) () const<br>_Gets the amount of items in this stack._  |
| virtual std::shared\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**getItemMeta**](#function-getitemmeta) () const<br>_Gets a copy of this_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _'s_[_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
| virtual std::string | [**getType**](#function-gettype) () const<br>_Gets the type of this item._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setAmount**](#function-setamount) ([**int**](classendstone_1_1Vector.md) amount) <br>_Sets the amount of items in this stack._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setType**](#function-settype) (std::string type) <br>_Sets the type of this item._  |
| virtual  | [**~ItemStack**](#function-itemstack) () = default<br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual [**const**](classendstone_1_1Vector.md) core::EndstoneItemStack \* | [**asEndstoneItemStack**](#function-asendstoneitemstack-12) () const<br> |
| virtual core::EndstoneItemStack \* | [**asEndstoneItemStack**](#function-asendstoneitemstack-22) () <br> |




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
inline virtual std::shared_ptr< ItemMeta > endstone::ItemStack::getItemMeta () const
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




### function asEndstoneItemStack [1/2]

```C++
inline virtual const core::EndstoneItemStack * endstone::ItemStack::asEndstoneItemStack () const
```




<hr>



### function asEndstoneItemStack [2/2]

```C++
inline virtual core::EndstoneItemStack * endstone::ItemStack::asEndstoneItemStack () 
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

