

# Class endstone::MapMeta



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**MapMeta**](classendstone_1_1MapMeta.md)



_Represents the metadata for a map item._ 

* `#include <endstone/inventory/meta/map_meta.h>`



Inherits the following classes: [endstone::ItemMeta](classendstone_1_1ItemMeta.md)
















## Public Types inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](classendstone_1_1ItemMeta.md#enum-type)  <br> |












## Public Static Attributes inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
|  [**ItemMeta**](classendstone_1_1ItemMeta.md) | [**EMPTY**](classendstone_1_1ItemMeta.md#variable-empty)  <br> |


























## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ItemMeta**](#function-itemmeta-12) () = default<br> |
|   | [**ItemMeta**](#function-itemmeta-22) ([**const**](classendstone_1_1Vector.md) [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) <br> |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](#function-clone) () override const<br>_Creates a clone of the current metadata._  |
| virtual Type | [**getType**](#function-gettype) () override const<br>_Gets the type of this item meta._  |


## Public Functions inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
|   | [**ItemMeta**](classendstone_1_1ItemMeta.md#function-itemmeta-12) () = default<br> |
|   | [**ItemMeta**](classendstone_1_1ItemMeta.md#function-itemmeta-22) ([**const**](classendstone_1_1Vector.md) [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) <br> |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](classendstone_1_1ItemMeta.md#function-clone) () const<br>_Creates a clone of the current metadata._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getDamage**](classendstone_1_1ItemMeta.md#function-getdamage) () const<br>_Gets the damage._  |
| virtual std::optional&lt; std::string &gt; | [**getDisplayName**](classendstone_1_1ItemMeta.md#function-getdisplayname) () const<br>_Gets the display name that is set._  |
| virtual std::optional&lt; std::vector&lt; std::string &gt; &gt; | [**getLore**](classendstone_1_1ItemMeta.md#function-getlore) () const<br>_Gets the lore that is set._  |
| virtual Type | [**getType**](classendstone_1_1ItemMeta.md#function-gettype) () const<br>_Gets the type of this item meta._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasDamage**](classendstone_1_1ItemMeta.md#function-hasdamage) () const<br>_Checks to see if this item has damage._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasDisplayName**](classendstone_1_1ItemMeta.md#function-hasdisplayname) () const<br>_Checks for existence of a display name._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasLore**](classendstone_1_1ItemMeta.md#function-haslore) () const<br>_Checks for existence of lore._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isEmpty**](classendstone_1_1ItemMeta.md#function-isempty) () const<br>_Checks if the item metadata is empty._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setDamage**](classendstone_1_1ItemMeta.md#function-setdamage) ([**int**](classendstone_1_1Vector.md) damage) <br>_Sets the damage._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setDisplayName**](classendstone_1_1ItemMeta.md#function-setdisplayname) (std::optional&lt; std::string &gt; name) <br>_Sets the display name._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setLore**](classendstone_1_1ItemMeta.md#function-setlore) (std::optional&lt; std::vector&lt; std::string &gt; &gt; lore) <br>_Sets the lore for this item or removes lore when given std::nullopt._  |
| virtual  | [**~ItemMeta**](classendstone_1_1ItemMeta.md#function-itemmeta) () = default<br> |






















































## Public Functions Documentation




### function ItemMeta [1/2]

```C++
endstone::MapMeta::ItemMeta () = default
```




<hr>



### function ItemMeta [2/2]

```C++
inline explicit endstone::MapMeta::ItemMeta (
    const  ItemMeta * meta
) 
```




<hr>



### function clone 

_Creates a clone of the current metadata._ 
```C++
inline virtual std::unique_ptr< ItemMeta > endstone::MapMeta::clone () override const
```





**Returns:**

A copy of the metadata containing the same state as the original. 





        
Implements [*endstone::ItemMeta::clone*](classendstone_1_1ItemMeta.md#function-clone)


<hr>



### function getType 

_Gets the type of this item meta._ 
```C++
inline virtual Type endstone::MapMeta::getType () override const
```





**Returns:**

type of this item meta 





        
Implements [*endstone::ItemMeta::getType*](classendstone_1_1ItemMeta.md#function-gettype)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/map_meta.h`

