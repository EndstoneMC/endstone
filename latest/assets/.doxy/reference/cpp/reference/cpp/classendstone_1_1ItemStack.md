

# Class endstone::ItemStack



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemStack**](classendstone_1_1ItemStack.md)



_Represents a stack of items._ 

* `#include <endstone/inventory/item_stack.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ItemStack**](#function-itemstack-12) () = default<br> |
|   | [**ItemStack**](#function-itemstack-22) (std::string type, int amount=1) <br> |
| virtual int | [**getAmount**](#function-getamount) () const<br>_Gets the amount of items in this stack._  |
| virtual std::string | [**getType**](#function-gettype) () const<br>_Gets the type of this item._  |
| virtual void | [**setAmount**](#function-setamount) (int amount) <br>_Sets the amount of items in this stack._  |
| virtual void | [**setType**](#function-settype) (std::string type) <br>_Sets the type of this item._  |
| virtual  | [**~ItemStack**](#function-itemstack) () = default<br> |




























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

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/item_stack.h`

