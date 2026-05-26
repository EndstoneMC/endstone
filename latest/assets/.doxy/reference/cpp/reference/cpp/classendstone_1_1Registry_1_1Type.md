

# Class endstone::Registry::Type



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Registry**](classendstone_1_1Registry.md) **>** [**Type**](classendstone_1_1Registry_1_1Type.md)



_CRTP base for registry-backed types identified by an_ [_**Identifier**_](classendstone_1_1Identifier.md) _._[More...](#detailed-description)

* `#include <endstone/registry.h>`





Inherited by the following classes: [endstone::BlockType](classendstone_1_1BlockType.md),  [endstone::Enchantment](classendstone_1_1Enchantment.md),  [endstone::ItemType](classendstone_1_1ItemType.md)












## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**Identifier**](classendstone_1_1Identifier.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**Id**](#typedef-id)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Id**](classendstone_1_1Identifier.md) | [**getId**](#function-getid) () const = 0<br>_Return the identifier of this object._  |
| virtual std::string | [**getTranslationKey**](#function-gettranslationkey) () const = 0<br>_Get the translation key, suitable for use in a translation component._  |
|   | [**operator Id**](#function-operator-id) () const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Id**](classendstone_1_1Identifier.md) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_2) ([**const**](classendstone_1_1Identifier.md) [**Id**](classendstone_1_1Identifier.md) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_3) ([**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) & other) const<br> |
| virtual  | [**~Type**](#function-type) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**get**](#function-get) ([**Id**](classendstone_1_1Identifier.md) id) <br>_Look up a registry entry by identifier._  |


























## Detailed Description


Provides [**getId()**](classendstone_1_1Registry_1_1Type.md#function-getid), [**getTranslationKey()**](classendstone_1_1Registry_1_1Type.md#function-gettranslationkey) pure virtual declarations, comparison operators, implicit conversion to Id, and a static [**get()**](classendstone_1_1Registry_1_1Type.md#function-get) method. 


    
## Public Types Documentation




### typedef Id 

```C++
using endstone::Registry< T >::Type::Id =  Identifier<T>;
```




<hr>
## Public Functions Documentation




### function getId 

_Return the identifier of this object._ 
```C++
virtual Id endstone::Registry::Type::getId () const = 0
```





**Returns:**

this object's identifier 





        

<hr>



### function getTranslationKey 

_Get the translation key, suitable for use in a translation component._ 
```C++
virtual std::string endstone::Registry::Type::getTranslationKey () const = 0
```





**Returns:**

the translation key 





        

<hr>



### function operator Id 

```C++
inline endstone::Registry::Type::operator Id () const
```




<hr>



### function operator!= 

```C++
inline bool endstone::Registry::Type::operator!= (
    const  Id & other
) const
```




<hr>



### function operator!= 

```C++
inline bool endstone::Registry::Type::operator!= (
    const  T & other
) const
```




<hr>



### function operator== 

```C++
inline bool endstone::Registry::Type::operator== (
    const  Id & other
) const
```




<hr>



### function operator== 

```C++
inline bool endstone::Registry::Type::operator== (
    const  T & other
) const
```




<hr>



### function ~Type 

```C++
virtual endstone::Registry::Type::~Type () = default
```




<hr>
## Public Static Functions Documentation




### function get 

_Look up a registry entry by identifier._ 
```C++
static inline const  T * endstone::Registry::Type::get (
    Id id
) 
```





**Parameters:**


* `id` [**Identifier**](classendstone_1_1Identifier.md) to look up. 



**Returns:**

Pointer to the entry, or nullptr if not found. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/registry.h`

