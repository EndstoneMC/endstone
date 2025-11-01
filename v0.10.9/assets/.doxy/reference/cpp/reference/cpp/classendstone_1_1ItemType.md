

# Class endstone::ItemType



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemType**](classendstone_1_1ItemType.md)



_Represents an item type._ 

* `#include <endstone/inventory/item_type.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::string\_view | [**getId**](#function-getid) () const = 0<br>_Return the identifier of this item type._  |
| virtual [**NamespacedKey**](classendstone_1_1NamespacedKey.md) | [**getKey**](#function-getkey) () const = 0<br>_Return the namespaced identifier of this item type._  |
| virtual int | [**getMaxDurability**](#function-getmaxdurability) () const = 0<br>_Gets the maximum durability of this item type._  |
| virtual int | [**getMaxStackSize**](#function-getmaxstacksize) () const = 0<br>_Gets the maximum amount of this item type that can be held in a stack._  |
| virtual std::string | [**getTranslationKey**](#function-gettranslationkey) () const = 0<br>_Get the translation key, suitable for use in a translation component._  |
|  bool | [**operator!=**](#function-operator) (const std::string\_view other) const<br> |
|  bool | [**operator!=**](#function-operator_1) (const [**ItemType**](classendstone_1_1ItemType.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_2) (const std::string\_view other) const<br> |
|  bool | [**operator==**](#function-operator_3) (const [**ItemType**](classendstone_1_1ItemType.md) & other) const<br> |
| virtual  | [**~ItemType**](#function-itemtype) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  const [**ItemType**](classendstone_1_1ItemType.md) \* | [**get**](#function-get) (std::string\_view name) <br>_Attempts to get the_ [_**ItemType**_](classendstone_1_1ItemType.md) _with the given name._ |


























## Public Functions Documentation




### function getId 

_Return the identifier of this item type._ 
```C++
virtual std::string_view endstone::ItemType::getId () const = 0
```





**Returns:**

this item's identifier 





        

<hr>



### function getKey 

_Return the namespaced identifier of this item type._ 
```C++
virtual NamespacedKey endstone::ItemType::getKey () const = 0
```





**Returns:**

this item's key 





        

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
    const ItemType & other
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
    const ItemType & other
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
static inline const ItemType * endstone::ItemType::get (
    std::string_view name
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

