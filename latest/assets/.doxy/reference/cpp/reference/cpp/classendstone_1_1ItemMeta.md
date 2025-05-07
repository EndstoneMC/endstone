

# Class endstone::ItemMeta



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemMeta**](classendstone_1_1ItemMeta.md)



_Represents the metadata of a generic item._ 

* `#include <endstone/inventory/meta/item_meta.h>`





Inherited by the following classes: [endstone::MapMeta](classendstone_1_1MapMeta.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br> |






## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**ItemMeta**](classendstone_1_1ItemMeta.md) | [**EMPTY**](#variable-empty)  <br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ItemMeta**](#function-itemmeta-12) () = default<br> |
|   | [**ItemMeta**](#function-itemmeta-22) ([**const**](classendstone_1_1Vector.md) [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) <br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**addEnchant**](#function-addenchant) ([**const**](classendstone_1_1Vector.md) std::string & id, [**int**](classendstone_1_1Vector.md) level, [**bool**](classendstone_1_1Vector.md) force) <br>_Adds the specified enchantment to this item meta._  |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](#function-clone) () const<br>_Creates a clone of the current metadata._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getDamage**](#function-getdamage) () const<br>_Gets the damage._  |
| virtual std::optional&lt; std::string &gt; | [**getDisplayName**](#function-getdisplayname) () const<br>_Gets the display name that is set._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getEnchantLevel**](#function-getenchantlevel) ([**const**](classendstone_1_1Vector.md) std::string & id) const<br>_Checks for the level of the specified enchantment._  |
| virtual std::unordered\_map&lt; std::string, [**int**](classendstone_1_1Vector.md) &gt; | [**getEnchants**](#function-getenchants) () const<br>_Returns a copy the enchantments in this_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
| virtual std::optional&lt; std::vector&lt; std::string &gt; &gt; | [**getLore**](#function-getlore) () const<br>_Gets the lore that is set._  |
| virtual Type | [**getType**](#function-gettype) () const<br>_Gets the type of this item meta._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasDamage**](#function-hasdamage) () const<br>_Checks to see if this item has damage._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasDisplayName**](#function-hasdisplayname) () const<br>_Checks for existence of a display name._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasEnchant**](#function-hasenchant) ([**const**](classendstone_1_1Vector.md) std::string & id) const<br>_Checks for existence of the specified enchantment._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasEnchants**](#function-hasenchants) () const<br>_Checks for the existence of any enchantments._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasLore**](#function-haslore) () const<br>_Checks for existence of lore._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isEmpty**](#function-isempty) () const<br>_Checks if the item metadata is empty._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**removeEnchant**](#function-removeenchant) ([**const**](classendstone_1_1Vector.md) std::string & id) <br>_Removes the specified enchantment from this item meta._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**removeEnchants**](#function-removeenchants) () <br>_Removes all enchantments from this item meta._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setDamage**](#function-setdamage) ([**int**](classendstone_1_1Vector.md) damage) <br>_Sets the damage._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setDisplayName**](#function-setdisplayname) (std::optional&lt; std::string &gt; name) <br>_Sets the display name._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setLore**](#function-setlore) (std::optional&lt; std::vector&lt; std::string &gt; &gt; lore) <br>_Sets the lore for this item or removes lore when given std::nullopt._  |
| virtual  | [**~ItemMeta**](#function-itemmeta) () = default<br> |




























## Public Types Documentation




### enum Type 

```C++
enum endstone::ItemMeta::Type {
    Item = 0,
    Map = 1,
    Count = 2,
    None = Item
};
```




<hr>
## Public Static Attributes Documentation




### variable EMPTY 

```C++
ItemMeta endstone::ItemMeta::EMPTY;
```




<hr>
## Public Functions Documentation




### function ItemMeta [1/2]

```C++
endstone::ItemMeta::ItemMeta () = default
```




<hr>



### function ItemMeta [2/2]

```C++
inline explicit endstone::ItemMeta::ItemMeta (
    const  ItemMeta * meta
) 
```




<hr>



### function addEnchant 

_Adds the specified enchantment to this item meta._ 
```C++
inline virtual bool endstone::ItemMeta::addEnchant (
    const std::string & id,
    int level,
    bool force
) 
```





**Parameters:**


* `id` [**Enchantment**](classendstone_1_1Enchantment.md) id to add 
* `level` [**Level**](classendstone_1_1Level.md) for the enchantment 
* `force` this indicates the enchantment should be applied, ignoring the level limit 



**Returns:**

true if the item meta changed as a result of this call, false otherwise 





        

<hr>



### function clone 

_Creates a clone of the current metadata._ 
```C++
inline virtual std::unique_ptr< ItemMeta > endstone::ItemMeta::clone () const
```





**Returns:**

A copy of the metadata containing the same state as the original. 





        

<hr>



### function getDamage 

_Gets the damage._ 
```C++
inline virtual int endstone::ItemMeta::getDamage () const
```





**Returns:**

the damage 





        

<hr>



### function getDisplayName 

_Gets the display name that is set._ 
```C++
inline virtual std::optional< std::string > endstone::ItemMeta::getDisplayName () const
```





**Returns:**

the display name that is set 





        

<hr>



### function getEnchantLevel 

_Checks for the level of the specified enchantment._ 
```C++
inline virtual int endstone::ItemMeta::getEnchantLevel (
    const std::string & id
) const
```





**Parameters:**


* `id` enchantment id to check 



**Returns:**

The level that the specified enchantment has, or 0 if none 





        

<hr>



### function getEnchants 

_Returns a copy the enchantments in this_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._
```C++
inline virtual std::unordered_map< std::string, int > endstone::ItemMeta::getEnchants () const
```



Returns an empty map if none.




**Returns:**

An immutable copy of the enchantments 





        

<hr>



### function getLore 

_Gets the lore that is set._ 
```C++
inline virtual std::optional< std::vector< std::string > > endstone::ItemMeta::getLore () const
```





**Returns:**

a list of lore that is set 





        

<hr>



### function getType 

_Gets the type of this item meta._ 
```C++
inline virtual Type endstone::ItemMeta::getType () const
```





**Returns:**

type of this item meta 





        

<hr>



### function hasDamage 

_Checks to see if this item has damage._ 
```C++
inline virtual bool endstone::ItemMeta::hasDamage () const
```





**Returns:**

true if this has damage 





        

<hr>



### function hasDisplayName 

_Checks for existence of a display name._ 
```C++
inline virtual bool endstone::ItemMeta::hasDisplayName () const
```





**Returns:**

true if this has a display name 





        

<hr>



### function hasEnchant 

_Checks for existence of the specified enchantment._ 
```C++
inline virtual bool endstone::ItemMeta::hasEnchant (
    const std::string & id
) const
```





**Parameters:**


* `id` enchantment id to check 



**Returns:**

true if this enchantment exists for this meta 





        

<hr>



### function hasEnchants 

_Checks for the existence of any enchantments._ 
```C++
inline virtual bool endstone::ItemMeta::hasEnchants () const
```





**Returns:**

true if an enchantment exists on this meta 





        

<hr>



### function hasLore 

_Checks for existence of lore._ 
```C++
inline virtual bool endstone::ItemMeta::hasLore () const
```





**Returns:**

true if this has lore 





        

<hr>



### function isEmpty 

_Checks if the item metadata is empty._ 
```C++
inline virtual bool endstone::ItemMeta::isEmpty () const
```





**Returns:**

true if the metadata is empty, false otherwise. 





        

<hr>



### function removeEnchant 

_Removes the specified enchantment from this item meta._ 
```C++
inline virtual bool endstone::ItemMeta::removeEnchant (
    const std::string & id
) 
```





**Parameters:**


* `id` [**Enchantment**](classendstone_1_1Enchantment.md) id to remove 



**Returns:**

true if the item meta changed as a result of this call, false otherwise 





        

<hr>



### function removeEnchants 

_Removes all enchantments from this item meta._ 
```C++
inline virtual void endstone::ItemMeta::removeEnchants () 
```




<hr>



### function setDamage 

_Sets the damage._ 
```C++
inline virtual void endstone::ItemMeta::setDamage (
    int damage
) 
```





**Parameters:**


* `damage` item damage 




        

<hr>



### function setDisplayName 

_Sets the display name._ 
```C++
inline virtual void endstone::ItemMeta::setDisplayName (
    std::optional< std::string > name
) 
```





**Parameters:**


* `name` the name to set 




        

<hr>



### function setLore 

_Sets the lore for this item or removes lore when given std::nullopt._ 
```C++
inline virtual void endstone::ItemMeta::setLore (
    std::optional< std::vector< std::string > > lore
) 
```





**Parameters:**


* `lore` the lore that will be set 




        

<hr>



### function ~ItemMeta 

```C++
virtual endstone::ItemMeta::~ItemMeta () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/item_meta.h`

