

# Class endstone::ItemMeta



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemMeta**](classendstone_1_1ItemMeta.md)



_Represents the metadata of a generic item._ 

* `#include <endstone/inventory/meta/item_meta.h>`





Inherited by the following classes: [endstone::CrossbowMeta](classendstone_1_1CrossbowMeta.md),  [endstone::MapMeta](classendstone_1_1MapMeta.md),  [endstone::WritableBookMeta](classendstone_1_1WritableBookMeta.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**addEnchant**](#function-addenchant) ([**EnchantmentId**](classendstone_1_1Identifier.md) id, [**int**](classendstone_1_1Identifier.md) level, [**bool**](classendstone_1_1Identifier.md) force) = 0<br>_Adds the specified enchantment to this item meta._  |
|  [**T**](classendstone_1_1Identifier.md) \* | [**as**](#function-as-12) () <br> |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**as**](#function-as-22) () const<br> |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](#function-clone) () const = 0<br>_Creates a clone of the current metadata._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getDamage**](#function-getdamage) () const = 0<br>_Gets the damage._  |
| virtual std::string | [**getDisplayName**](#function-getdisplayname) () const = 0<br>_Gets the display name that is set._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getEnchantLevel**](#function-getenchantlevel) ([**EnchantmentId**](classendstone_1_1Identifier.md) id) const = 0<br>_Checks for the level of the specified enchantment._  |
| virtual std::unordered\_map&lt; [**const**](classendstone_1_1Identifier.md) [**Enchantment**](classendstone_1_1Enchantment.md) \*, [**int**](classendstone_1_1Identifier.md) &gt; | [**getEnchants**](#function-getenchants) () const = 0<br>_Returns a copy the enchantments in this_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
| virtual std::vector&lt; std::string &gt; | [**getLore**](#function-getlore) () const = 0<br>_Gets the lore that is set._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getRepairCost**](#function-getrepaircost) () const = 0<br>_Gets the repair penalty._  |
| virtual Type | [**getType**](#function-gettype) () const = 0<br>_Gets the type of this item meta._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasConflictingEnchant**](#function-hasconflictingenchant) ([**EnchantmentId**](classendstone_1_1Identifier.md) id) const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasDamage**](#function-hasdamage) () const = 0<br>_Checks to see if this item has damage._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasDisplayName**](#function-hasdisplayname) () const = 0<br>_Checks for existence of a display name._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasEnchant**](#function-hasenchant) ([**EnchantmentId**](classendstone_1_1Identifier.md) id) const = 0<br>_Checks for existence of the specified enchantment._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasEnchants**](#function-hasenchants) () const = 0<br>_Checks for the existence of any enchantments._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasLore**](#function-haslore) () const = 0<br>_Checks for existence of lore._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasRepairCost**](#function-hasrepaircost) () const = 0<br>_Checks to see if this has a repair penalty._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isUnbreakable**](#function-isunbreakable) () const = 0<br>_Return if the unbreakable tag is true. An unbreakable item will not lose durability._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**removeEnchant**](#function-removeenchant) ([**EnchantmentId**](classendstone_1_1Identifier.md) id) = 0<br>_Removes the specified enchantment from this item meta._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**removeEnchants**](#function-removeenchants) () = 0<br>_Removes all enchantments from this item meta._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setDamage**](#function-setdamage) ([**int**](classendstone_1_1Identifier.md) damage) = 0<br>_Sets the damage._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setDisplayName**](#function-setdisplayname) (std::optional&lt; std::string &gt; name) = 0<br>_Sets the display name._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setLore**](#function-setlore) (std::optional&lt; std::vector&lt; std::string &gt; &gt; lore) = 0<br>_Sets the lore for this item or removes lore when given_ `std::nullopt` _._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setRepairCost**](#function-setrepaircost) ([**int**](classendstone_1_1Identifier.md) cost) = 0<br>_Sets the repair penalty._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setUnbreakable**](#function-setunbreakable) ([**bool**](classendstone_1_1Identifier.md) unbreakable) = 0<br>_Sets the unbreakable tag. An unbreakable item will not lose durability._  |
| virtual [**CompoundTag**](classendstone_1_1CompoundTag.md) | [**toNbt**](#function-tonbt) () const = 0<br> |
| virtual  | [**~ItemMeta**](#function-itemmeta) () = default<br> |




























## Public Types Documentation




### enum Type 

```C++
enum endstone::ItemMeta::Type {
    Item,
    Book,
    CrossBow,
    Map,
    WritableBook
};
```




<hr>
## Public Functions Documentation




### function addEnchant 

_Adds the specified enchantment to this item meta._ 
```C++
virtual bool endstone::ItemMeta::addEnchant (
    EnchantmentId id,
    int level,
    bool force
) = 0
```





**Parameters:**


* `id` [**Enchantment**](classendstone_1_1Enchantment.md) id to add 
* `level` [**Level**](classendstone_1_1Level.md) for the enchantment 
* `force` this indicates the enchantment should be applied, ignoring the level limit 



**Returns:**

true if the item meta changed as a result of this call, false otherwise 





        

<hr>



### function as [1/2]

```C++
template<typename  T>
inline T * endstone::ItemMeta::as () 
```




<hr>



### function as [2/2]

```C++
template<typename  T>
inline const  T * endstone::ItemMeta::as () const
```




<hr>



### function clone 

_Creates a clone of the current metadata._ 
```C++
virtual std::unique_ptr< ItemMeta > endstone::ItemMeta::clone () const = 0
```





**Returns:**

A copy of the metadata containing the same state as the original. 





        

<hr>



### function getDamage 

_Gets the damage._ 
```C++
virtual int endstone::ItemMeta::getDamage () const = 0
```





**Returns:**

the damage 





        

<hr>



### function getDisplayName 

_Gets the display name that is set._ 
```C++
virtual std::string endstone::ItemMeta::getDisplayName () const = 0
```





**Returns:**

the display name that is set 





        

<hr>



### function getEnchantLevel 

_Checks for the level of the specified enchantment._ 
```C++
virtual int endstone::ItemMeta::getEnchantLevel (
    EnchantmentId id
) const = 0
```





**Parameters:**


* `id` enchantment id to check 



**Returns:**

The level that the specified enchantment has, or 0 if none 





        

<hr>



### function getEnchants 

_Returns a copy the enchantments in this_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._
```C++
virtual std::unordered_map< const  Enchantment *, int > endstone::ItemMeta::getEnchants () const = 0
```



Returns an empty map if none.




**Returns:**

An immutable copy of the enchantments 





        

<hr>



### function getLore 

_Gets the lore that is set._ 
```C++
virtual std::vector< std::string > endstone::ItemMeta::getLore () const = 0
```





**Returns:**

a list of lore that is set 





        

<hr>



### function getRepairCost 

_Gets the repair penalty._ 
```C++
virtual int endstone::ItemMeta::getRepairCost () const = 0
```





**Returns:**

the repair penalty 





        

<hr>



### function getType 

_Gets the type of this item meta._ 
```C++
virtual Type endstone::ItemMeta::getType () const = 0
```





**Returns:**

type of this item meta 





        

<hr>



### function hasConflictingEnchant 

```C++
virtual bool endstone::ItemMeta::hasConflictingEnchant (
    EnchantmentId id
) const = 0
```



Checks if the specified enchantment conflicts with any enchantments in this [**ItemMeta**](classendstone_1_1ItemMeta.md).




**Parameters:**


* `id` [**Enchantment**](classendstone_1_1Enchantment.md) id to test 



**Returns:**

true if the enchantment conflicts, false otherwise 





        

<hr>



### function hasDamage 

_Checks to see if this item has damage._ 
```C++
virtual bool endstone::ItemMeta::hasDamage () const = 0
```





**Returns:**

true if this has damage 





        

<hr>



### function hasDisplayName 

_Checks for existence of a display name._ 
```C++
virtual bool endstone::ItemMeta::hasDisplayName () const = 0
```





**Returns:**

true if this has a display name 





        

<hr>



### function hasEnchant 

_Checks for existence of the specified enchantment._ 
```C++
virtual bool endstone::ItemMeta::hasEnchant (
    EnchantmentId id
) const = 0
```





**Parameters:**


* `id` enchantment id to check 



**Returns:**

true if this enchantment exists for this meta 





        

<hr>



### function hasEnchants 

_Checks for the existence of any enchantments._ 
```C++
virtual bool endstone::ItemMeta::hasEnchants () const = 0
```





**Returns:**

true if an enchantment exists on this meta 





        

<hr>



### function hasLore 

_Checks for existence of lore._ 
```C++
virtual bool endstone::ItemMeta::hasLore () const = 0
```





**Returns:**

true if this has lore 





        

<hr>



### function hasRepairCost 

_Checks to see if this has a repair penalty._ 
```C++
virtual bool endstone::ItemMeta::hasRepairCost () const = 0
```





**Returns:**

true if this has a repair penalty 





        

<hr>



### function isUnbreakable 

_Return if the unbreakable tag is true. An unbreakable item will not lose durability._ 
```C++
virtual bool endstone::ItemMeta::isUnbreakable () const = 0
```





**Returns:**

true if the unbreakable tag is true 





        

<hr>



### function removeEnchant 

_Removes the specified enchantment from this item meta._ 
```C++
virtual bool endstone::ItemMeta::removeEnchant (
    EnchantmentId id
) = 0
```





**Parameters:**


* `id` [**Enchantment**](classendstone_1_1Enchantment.md) id to remove 



**Returns:**

true if the item meta changed as a result of this call, false otherwise 





        

<hr>



### function removeEnchants 

_Removes all enchantments from this item meta._ 
```C++
virtual void endstone::ItemMeta::removeEnchants () = 0
```




<hr>



### function setDamage 

_Sets the damage._ 
```C++
virtual void endstone::ItemMeta::setDamage (
    int damage
) = 0
```





**Parameters:**


* `damage` item damage 




        

<hr>



### function setDisplayName 

_Sets the display name._ 
```C++
virtual void endstone::ItemMeta::setDisplayName (
    std::optional< std::string > name
) = 0
```





**Parameters:**


* `name` the name to set 




        

<hr>



### function setLore 

_Sets the lore for this item or removes lore when given_ `std::nullopt` _._
```C++
virtual void endstone::ItemMeta::setLore (
    std::optional< std::vector< std::string > > lore
) = 0
```





**Parameters:**


* `lore` the lore that will be set 




        

<hr>



### function setRepairCost 

_Sets the repair penalty._ 
```C++
virtual void endstone::ItemMeta::setRepairCost (
    int cost
) = 0
```





**Parameters:**


* `cost` repair penalty 




        

<hr>



### function setUnbreakable 

_Sets the unbreakable tag. An unbreakable item will not lose durability._ 
```C++
virtual void endstone::ItemMeta::setUnbreakable (
    bool unbreakable
) = 0
```





**Parameters:**


* `unbreakable` true if set unbreakable 




        

<hr>



### function toNbt 

```C++
virtual CompoundTag endstone::ItemMeta::toNbt () const = 0
```




<hr>



### function ~ItemMeta 

```C++
virtual endstone::ItemMeta::~ItemMeta () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/item_meta.h`

