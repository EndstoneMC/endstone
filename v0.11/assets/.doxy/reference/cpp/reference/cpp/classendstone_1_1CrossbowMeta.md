

# Class endstone::CrossbowMeta



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CrossbowMeta**](classendstone_1_1CrossbowMeta.md)



_Represents the meta for a crossbow that can have charged projectiles._ 

* `#include <endstone/inventory/meta/crossbow_meta.h>`



Inherits the following classes: [endstone::ItemMeta](classendstone_1_1ItemMeta.md)
















## Public Types inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](classendstone_1_1ItemMeta.md#enum-type)  <br> |






































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**addChargedProjectile**](#function-addchargedprojectile) ([**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & item) = 0<br>_Adds a charged projectile to this item._  |
| virtual std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getChargedProjectiles**](#function-getchargedprojectiles) () const = 0<br>_Returns an immutable list of the projectiles charged on this item._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasChargedProjectiles**](#function-haschargedprojectiles) () const = 0<br>_Returns whether the item has any charged projectiles._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setChargedProjectiles**](#function-setchargedprojectiles) (std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; projectiles) = 0<br>_Sets the projectiles charged on this item._  |


## Public Functions inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**addEnchant**](classendstone_1_1ItemMeta.md#function-addenchant) ([**EnchantmentId**](classendstone_1_1Identifier.md) id, [**int**](classendstone_1_1Identifier.md) level, [**bool**](classendstone_1_1Identifier.md) force) = 0<br>_Adds the specified enchantment to this item meta._  |
|  [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1ItemMeta.md#function-as-12) () <br> |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1ItemMeta.md#function-as-22) () const<br> |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](classendstone_1_1ItemMeta.md#function-clone) () const = 0<br>_Creates a clone of the current metadata._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getDamage**](classendstone_1_1ItemMeta.md#function-getdamage) () const = 0<br>_Gets the damage._  |
| virtual std::string | [**getDisplayName**](classendstone_1_1ItemMeta.md#function-getdisplayname) () const = 0<br>_Gets the display name that is set._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getEnchantLevel**](classendstone_1_1ItemMeta.md#function-getenchantlevel) ([**EnchantmentId**](classendstone_1_1Identifier.md) id) const = 0<br>_Checks for the level of the specified enchantment._  |
| virtual std::unordered\_map&lt; [**const**](classendstone_1_1Identifier.md) [**Enchantment**](classendstone_1_1Enchantment.md) \*, [**int**](classendstone_1_1Identifier.md) &gt; | [**getEnchants**](classendstone_1_1ItemMeta.md#function-getenchants) () const = 0<br>_Returns a copy the enchantments in this_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
| virtual [**const**](classendstone_1_1Identifier.md) core::ItemMetaExtras & | [**getExtras**](classendstone_1_1ItemMeta.md#function-getextras) () const = 0<br> |
| virtual std::vector&lt; std::string &gt; | [**getLore**](classendstone_1_1ItemMeta.md#function-getlore) () const = 0<br>_Gets the lore that is set._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getRepairCost**](classendstone_1_1ItemMeta.md#function-getrepaircost) () const = 0<br>_Gets the repair penalty._  |
| virtual Type | [**getType**](classendstone_1_1ItemMeta.md#function-gettype) () const = 0<br>_Gets the type of this item meta._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasConflictingEnchant**](classendstone_1_1ItemMeta.md#function-hasconflictingenchant) ([**EnchantmentId**](classendstone_1_1Identifier.md) id) const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasDamage**](classendstone_1_1ItemMeta.md#function-hasdamage) () const = 0<br>_Checks to see if this item has damage._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasDisplayName**](classendstone_1_1ItemMeta.md#function-hasdisplayname) () const = 0<br>_Checks for existence of a display name._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasEnchant**](classendstone_1_1ItemMeta.md#function-hasenchant) ([**EnchantmentId**](classendstone_1_1Identifier.md) id) const = 0<br>_Checks for existence of the specified enchantment._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasEnchants**](classendstone_1_1ItemMeta.md#function-hasenchants) () const = 0<br>_Checks for the existence of any enchantments._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasLore**](classendstone_1_1ItemMeta.md#function-haslore) () const = 0<br>_Checks for existence of lore._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasRepairCost**](classendstone_1_1ItemMeta.md#function-hasrepaircost) () const = 0<br>_Checks to see if this has a repair penalty._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isUnbreakable**](classendstone_1_1ItemMeta.md#function-isunbreakable) () const = 0<br>_Return if the unbreakable tag is true. An unbreakable item will not lose durability._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**removeEnchant**](classendstone_1_1ItemMeta.md#function-removeenchant) ([**EnchantmentId**](classendstone_1_1Identifier.md) id) = 0<br>_Removes the specified enchantment from this item meta._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**removeEnchants**](classendstone_1_1ItemMeta.md#function-removeenchants) () = 0<br>_Removes all enchantments from this item meta._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setDamage**](classendstone_1_1ItemMeta.md#function-setdamage) ([**int**](classendstone_1_1Identifier.md) damage) = 0<br>_Sets the damage._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setDisplayName**](classendstone_1_1ItemMeta.md#function-setdisplayname) (std::optional&lt; std::string &gt; name) = 0<br>_Sets the display name._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setLore**](classendstone_1_1ItemMeta.md#function-setlore) (std::optional&lt; std::vector&lt; std::string &gt; &gt; lore) = 0<br>_Sets the lore for this item or removes lore when given_ `std::nullopt` _._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setRepairCost**](classendstone_1_1ItemMeta.md#function-setrepaircost) ([**int**](classendstone_1_1Identifier.md) cost) = 0<br>_Sets the repair penalty._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setUnbreakable**](classendstone_1_1ItemMeta.md#function-setunbreakable) ([**bool**](classendstone_1_1Identifier.md) unbreakable) = 0<br>_Sets the unbreakable tag. An unbreakable item will not lose durability._  |
| virtual  | [**~ItemMeta**](classendstone_1_1ItemMeta.md#function-itemmeta) () = default<br> |






















































## Public Functions Documentation




### function addChargedProjectile 

_Adds a charged projectile to this item._ 
```C++
virtual void endstone::CrossbowMeta::addChargedProjectile (
    const  ItemStack & item
) = 0
```





**Parameters:**


* `item` projectile 




        

<hr>



### function getChargedProjectiles 

_Returns an immutable list of the projectiles charged on this item._ 
```C++
virtual std::vector< ItemStack > endstone::CrossbowMeta::getChargedProjectiles () const = 0
```





**Returns:**

charged projectiles 





        

<hr>



### function hasChargedProjectiles 

_Returns whether the item has any charged projectiles._ 
```C++
virtual bool endstone::CrossbowMeta::hasChargedProjectiles () const = 0
```





**Returns:**

whether charged projectiles are present 





        

<hr>



### function setChargedProjectiles 

_Sets the projectiles charged on this item._ 
```C++
virtual void endstone::CrossbowMeta::setChargedProjectiles (
    std::vector< ItemStack > projectiles
) = 0
```





**Parameters:**


* `projectiles` the projectiles to set 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/crossbow_meta.h`

