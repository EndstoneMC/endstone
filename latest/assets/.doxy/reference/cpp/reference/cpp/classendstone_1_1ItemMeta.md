

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
|   | [**ItemMeta**](#function-itemmeta) (const [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) <br> |
|  bool | [**addEnchant**](#function-addenchant) (const std::string & id, int level, bool force=false) <br>_Adds the specified enchantment to this item meta._  |
|  T \* | [**as**](#function-as-12) () <br> |
|  const T \* | [**as**](#function-as-22) () const<br> |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](#function-clone) () const<br>_Creates a clone of the current metadata._  |
|  int | [**getDamage**](#function-getdamage) () const<br>_Gets the damage._  |
|  std::string | [**getDisplayName**](#function-getdisplayname) () const<br>_Gets the display name that is set._  |
|  int | [**getEnchantLevel**](#function-getenchantlevel) (const std::string & id) const<br>_Checks for the level of the specified enchantment._  |
|  std::unordered\_map&lt; std::string, int &gt; | [**getEnchants**](#function-getenchants) () const<br>_Returns a copy the enchantments in this_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
|  std::vector&lt; std::string &gt; | [**getLore**](#function-getlore) () const<br>_Gets the lore that is set._  |
|  int | [**getRepairCost**](#function-getrepaircost) () const<br>_Gets the repair penalty._  |
| virtual Type | [**getType**](#function-gettype) () const<br>_Gets the type of this item meta._  |
|  bool | [**hasDamage**](#function-hasdamage) () const<br>_Checks to see if this item has damage._  |
|  bool | [**hasDisplayName**](#function-hasdisplayname) () const<br>_Checks for existence of a display name._  |
|  bool | [**hasEnchant**](#function-hasenchant) (const std::string & id) const<br>_Checks for existence of the specified enchantment._  |
|  bool | [**hasEnchants**](#function-hasenchants) () const<br>_Checks for the existence of any enchantments._  |
|  bool | [**hasLore**](#function-haslore) () const<br>_Checks for existence of lore._  |
|  bool | [**hasRepairCost**](#function-hasrepaircost) () const<br>_Checks to see if this has a repair penalty._  |
| virtual bool | [**isEmpty**](#function-isempty) () const<br>_Checks if the item metadata is empty._  |
|  bool | [**isUnbreakable**](#function-isunbreakable) () const<br>_Return if the unbreakable tag is true. An unbreakable item will not lose durability._  |
|  bool | [**removeEnchant**](#function-removeenchant) (const std::string & id) <br>_Removes the specified enchantment from this item meta._  |
|  void | [**removeEnchants**](#function-removeenchants) () <br>_Removes all enchantments from this item meta._  |
|  void | [**setDamage**](#function-setdamage) (int damage) <br>_Sets the damage._  |
|  void | [**setDisplayName**](#function-setdisplayname) (std::optional&lt; std::string &gt; name) <br>_Sets the display name._  |
|  void | [**setLore**](#function-setlore) (std::optional&lt; std::vector&lt; std::string &gt; &gt; lore) <br>_Sets the lore for this item or removes lore when given std::nullopt._  |
|  void | [**setRepairCost**](#function-setrepaircost) (int cost) <br>_Sets the repair penalty._  |
|  void | [**setUnbreakable**](#function-setunbreakable) (bool unbreakable) <br>_Sets the unbreakable tag. An unbreakable item will not lose durability._  |
| virtual  | [**~ItemMeta**](#function-itemmeta) () = default<br> |




























## Public Types Documentation




### enum Type 

```C++
enum endstone::ItemMeta::Type {
    Item,
    Map
};
```




<hr>
## Public Functions Documentation




### function ItemMeta 

```C++
inline explicit endstone::ItemMeta::ItemMeta (
    const ItemMeta * meta
) 
```




<hr>



### function addEnchant 

_Adds the specified enchantment to this item meta._ 
```C++
inline bool endstone::ItemMeta::addEnchant (
    const std::string & id,
    int level,
    bool force=false
) 
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
template<typename T>
inline T * endstone::ItemMeta::as () 
```




<hr>



### function as [2/2]

```C++
template<typename T>
inline const T * endstone::ItemMeta::as () const
```




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
inline int endstone::ItemMeta::getDamage () const
```





**Returns:**

the damage 





        

<hr>



### function getDisplayName 

_Gets the display name that is set._ 
```C++
inline std::string endstone::ItemMeta::getDisplayName () const
```





**Returns:**

the display name that is set 





        

<hr>



### function getEnchantLevel 

_Checks for the level of the specified enchantment._ 
```C++
inline int endstone::ItemMeta::getEnchantLevel (
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
inline std::unordered_map< std::string, int > endstone::ItemMeta::getEnchants () const
```



Returns an empty map if none.




**Returns:**

An immutable copy of the enchantments 





        

<hr>



### function getLore 

_Gets the lore that is set._ 
```C++
inline std::vector< std::string > endstone::ItemMeta::getLore () const
```





**Returns:**

a list of lore that is set 





        

<hr>



### function getRepairCost 

_Gets the repair penalty._ 
```C++
inline int endstone::ItemMeta::getRepairCost () const
```





**Returns:**

the repair penalty 





        

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
inline bool endstone::ItemMeta::hasDamage () const
```





**Returns:**

true if this has damage 





        

<hr>



### function hasDisplayName 

_Checks for existence of a display name._ 
```C++
inline bool endstone::ItemMeta::hasDisplayName () const
```





**Returns:**

true if this has a display name 





        

<hr>



### function hasEnchant 

_Checks for existence of the specified enchantment._ 
```C++
inline bool endstone::ItemMeta::hasEnchant (
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
inline bool endstone::ItemMeta::hasEnchants () const
```





**Returns:**

true if an enchantment exists on this meta 





        

<hr>



### function hasLore 

_Checks for existence of lore._ 
```C++
inline bool endstone::ItemMeta::hasLore () const
```





**Returns:**

true if this has lore 





        

<hr>



### function hasRepairCost 

_Checks to see if this has a repair penalty._ 
```C++
inline bool endstone::ItemMeta::hasRepairCost () const
```





**Returns:**

true if this has a repair penalty 





        

<hr>



### function isEmpty 

_Checks if the item metadata is empty._ 
```C++
inline virtual bool endstone::ItemMeta::isEmpty () const
```





**Returns:**

true if the metadata is empty, false otherwise. 





        

<hr>



### function isUnbreakable 

_Return if the unbreakable tag is true. An unbreakable item will not lose durability._ 
```C++
inline bool endstone::ItemMeta::isUnbreakable () const
```





**Returns:**

true if the unbreakable tag is true 





        

<hr>



### function removeEnchant 

_Removes the specified enchantment from this item meta._ 
```C++
inline bool endstone::ItemMeta::removeEnchant (
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
inline void endstone::ItemMeta::removeEnchants () 
```




<hr>



### function setDamage 

_Sets the damage._ 
```C++
inline void endstone::ItemMeta::setDamage (
    int damage
) 
```





**Parameters:**


* `damage` item damage 




        

<hr>



### function setDisplayName 

_Sets the display name._ 
```C++
inline void endstone::ItemMeta::setDisplayName (
    std::optional< std::string > name
) 
```





**Parameters:**


* `name` the name to set 




        

<hr>



### function setLore 

_Sets the lore for this item or removes lore when given std::nullopt._ 
```C++
inline void endstone::ItemMeta::setLore (
    std::optional< std::vector< std::string > > lore
) 
```





**Parameters:**


* `lore` the lore that will be set 




        

<hr>



### function setRepairCost 

_Sets the repair penalty._ 
```C++
inline void endstone::ItemMeta::setRepairCost (
    int cost
) 
```





**Parameters:**


* `cost` repair penalty 




        

<hr>



### function setUnbreakable 

_Sets the unbreakable tag. An unbreakable item will not lose durability._ 
```C++
inline void endstone::ItemMeta::setUnbreakable (
    bool unbreakable
) 
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

