

# Class endstone::ItemType



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemType**](classendstone_1_1ItemType.md)



_Represents an item type._ 

* `#include <endstone/inventory/item_type.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::string\_view | [**getId**](#function-getid) () const = 0<br>_Return the identifier of this item type._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getMaxDurability**](#function-getmaxdurability) () const = 0<br>_Gets the maximum durability of this item type._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getMaxStackSize**](#function-getmaxstacksize) () const = 0<br>_Gets the maximum amount of this item type that can be held in a stack._  |
| virtual std::string | [**getTranslationKey**](#function-gettranslationkey) () const = 0<br>_Get the translation key, suitable for use in a translation component._  |
|  [**bool**](classendstone_1_1Vector.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Vector.md) std::string\_view other) const<br> |
|  [**bool**](classendstone_1_1Vector.md) | [**operator!=**](#function-operator_1) ([**const**](classendstone_1_1Vector.md) [**ItemType**](classendstone_1_1ItemType.md) & other) const<br> |
|  [**bool**](classendstone_1_1Vector.md) | [**operator==**](#function-operator_2) ([**const**](classendstone_1_1Vector.md) std::string\_view other) const<br> |
|  [**bool**](classendstone_1_1Vector.md) | [**operator==**](#function-operator_3) ([**const**](classendstone_1_1Vector.md) [**ItemType**](classendstone_1_1ItemType.md) & other) const<br> |
| virtual  | [**~ItemType**](#function-itemtype) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) [**ItemType**](classendstone_1_1ItemType.md) \* | [**get**](#function-get) ([**const**](classendstone_1_1Vector.md) std::string & name) <br>_Attempts to get the_ [_**ItemType**_](classendstone_1_1ItemType.md) _with the given name._ |


























## Public Functions Documentation




### function getId 

_Return the identifier of this item type._ 
```C++
virtual std::string_view endstone::ItemType::getId () const = 0
```





**Returns:**

this item's identifier 





        

<hr>



### function getMaxDurability 

_Gets the maximum durability of this item type._ 
```C++
virtual int endstone::ItemType::getMaxDurability () const = 0
```





**Returns:**

Maximum durability for this item type 





        

<hr>



### function getMaxStackSize 

_Gets the maximum amount of this item type that can be held in a stack._ 
```C++
virtual int endstone::ItemType::getMaxStackSize () const = 0
```





**Returns:**

Maximum stack size for this item type 





        

<hr>



### function getTranslationKey 

_Get the translation key, suitable for use in a translation component._ 
```C++
virtual std::string endstone::ItemType::getTranslationKey () const = 0
```





**Returns:**

the translation key 





        

<hr>



### function operator!= 

```C++
inline bool endstone::ItemType::operator!= (
    const std::string_view other
) const
```




<hr>



### function operator!= 

```C++
inline bool endstone::ItemType::operator!= (
    const  ItemType & other
) const
```




<hr>



### function operator== 

```C++
inline bool endstone::ItemType::operator== (
    const std::string_view other
) const
```




<hr>



### function operator== 

```C++
inline bool endstone::ItemType::operator== (
    const  ItemType & other
) const
```




<hr>



### function ~ItemType 

```C++
virtual endstone::ItemType::~ItemType () = default
```




<hr>
## Public Static Functions Documentation




### function get 

_Attempts to get the_ [_**ItemType**_](classendstone_1_1ItemType.md) _with the given name._
```C++
static inline const  ItemType * endstone::ItemType::get (
    const std::string & name
) 
```





**Note:**

This is a normal lookup, names must be the precise name.




**Parameters:**


* `name` Name of the item\_type to get



**Returns:**

[**ItemType**](classendstone_1_1ItemType.md) if found, or nullptr 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/item_type.h`

