

# Class endstone::ItemMeta



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemMeta**](classendstone_1_1ItemMeta.md)



_Represents the metadata of a generic item._ 

* `#include <endstone/inventory/meta/item_meta.h>`





Inherited by the following classes: [endstone::MapMeta](classendstone_1_1MapMeta.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
| virtual bool | [**addEnchant**](#function-addenchant) (const std::string & id, int level, bool force) = 0<br>_Adds the specified enchantment to this item meta._  |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](#function-clone) () const = 0<br>_Creates a clone of the current metadata._  |
| virtual int | [**getDamage**](#function-getdamage) () const = 0<br>_Gets the damage._  |
| virtual std::optional&lt; std::string &gt; | [**getDisplayName**](#function-getdisplayname) () const = 0<br>_Gets the display name that is set._  |
| virtual int | [**getEnchantLevel**](#function-getenchantlevel) (const std::string & id) const = 0<br>_Checks for the level of the specified enchantment._  |
| virtual std::unordered\_map&lt; std::string, int &gt; | [**getEnchants**](#function-getenchants) () const = 0<br>_Returns a copy the enchantments in this_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
| virtual std::optional&lt; std::vector&lt; std::string &gt; &gt; | [**getLore**](#function-getlore) () const = 0<br>_Gets the lore that is set._  |
| virtual int | [**getRepairCost**](#function-getrepaircost) () const = 0<br>_Gets the repair penalty._  |
| virtual Type | [**getType**](#function-gettype) () const = 0<br>_Gets the type of this item meta._  |
| virtual bool | [**hasDamage**](#function-hasdamage) () const = 0<br>_Checks to see if this item has damage._  |
| virtual bool | [**hasDisplayName**](#function-hasdisplayname) () const = 0<br>_Checks for existence of a display name._  |
| virtual bool | [**hasEnchant**](#function-hasenchant) (const std::string & id) const = 0<br>_Checks for existence of the specified enchantment._  |
| virtual bool | [**hasEnchants**](#function-hasenchants) () const = 0<br>_Checks for the existence of any enchantments._  |
| virtual bool | [**hasLore**](#function-haslore) () const = 0<br>_Checks for existence of lore._  |
| virtual bool | [**hasRepairCost**](#function-hasrepaircost) () const = 0<br>_Checks to see if this has a repair penalty._  |
| virtual bool | [**isEmpty**](#function-isempty) () const = 0<br>_Checks if the item metadata is empty._  |
| virtual bool | [**isUnbreakable**](#function-isunbreakable) () const = 0<br>_Return if the unbreakable tag is true. An unbreakable item will not lose durability._  |
| virtual bool | [**removeEnchant**](#function-removeenchant) (const std::string & id) = 0<br>_Removes the specified enchantment from this item meta._  |
| virtual void | [**removeEnchants**](#function-removeenchants) () = 0<br>_Removes all enchantments from this item meta._  |
| virtual void | [**setDamage**](#function-setdamage) (int damage) = 0<br>_Sets the damage._  |
| virtual void | [**setDisplayName**](#function-setdisplayname) (std::optional&lt; std::string &gt; name) = 0<br>_Sets the display name._  |
| virtual void | [**setLore**](#function-setlore) (std::optional&lt; std::vector&lt; std::string &gt; &gt; lore) = 0<br>_Sets the lore for this item or removes lore when given std::nullopt._  |
| virtual void | [**setRepairCost**](#function-setrepaircost) (int cost) = 0<br>_Sets the repair penalty._  |
| virtual void | [**setUnbreakable**](#function-setunbreakable) (bool unbreakable) = 0<br>_Sets the unbreakable tag. An unbreakable item will not lose durability._  |
| virtual  | [**~ItemMeta**](#function-itemmeta) () = default<br> |




























## Public Types Documentation




### enum Type 

```C++
enum endstone::ItemMeta::Type {
    Item = 0,
    Map = 1,
    Count,
    None = Item
};
```




<hr>
## Public Functions Documentation




### function addEnchant 

_Adds the specified enchantment to this item meta._ 
```C++
virtual bool endstone::ItemMeta::addEnchant (
    const std::string & id,
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
virtual std::optional< std::string > endstone::ItemMeta::getDisplayName () const = 0
```





**Returns:**

the display name that is set 





        

<hr>



### function getEnchantLevel 

_Checks for the level of the specified enchantment._ 
```C++
virtual int endstone::ItemMeta::getEnchantLevel (
    const std::string & id
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
virtual std::unordered_map< std::string, int > endstone::ItemMeta::getEnchants () const = 0
```



Returns an empty map if none.




**Returns:**

An immutable copy of the enchantments 





        

<hr>



### function getLore 

_Gets the lore that is set._ 
```C++
virtual std::optional< std::vector< std::string > > endstone::ItemMeta::getLore () const = 0
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
    const std::string & id
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



### function isEmpty 

_Checks if the item metadata is empty._ 
```C++
virtual bool endstone::ItemMeta::isEmpty () const = 0
```





**Returns:**

true if the metadata is empty, false otherwise. 





        

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
    const std::string & id
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

_Sets the lore for this item or removes lore when given std::nullopt._ 
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



### function ~ItemMeta 

```C++
virtual endstone::ItemMeta::~ItemMeta () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/item_meta.h`

