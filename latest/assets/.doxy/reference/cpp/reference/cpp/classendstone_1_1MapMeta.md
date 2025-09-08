

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








































## Public Functions inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
| virtual bool | [**addEnchant**](classendstone_1_1ItemMeta.md#function-addenchant) (const std::string & id, int level, bool force) = 0<br>_Adds the specified enchantment to this item meta._  |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](classendstone_1_1ItemMeta.md#function-clone) () const = 0<br>_Creates a clone of the current metadata._  |
| virtual int | [**getDamage**](classendstone_1_1ItemMeta.md#function-getdamage) () const = 0<br>_Gets the damage._  |
| virtual std::optional&lt; std::string &gt; | [**getDisplayName**](classendstone_1_1ItemMeta.md#function-getdisplayname) () const = 0<br>_Gets the display name that is set._  |
| virtual int | [**getEnchantLevel**](classendstone_1_1ItemMeta.md#function-getenchantlevel) (const std::string & id) const = 0<br>_Checks for the level of the specified enchantment._  |
| virtual std::unordered\_map&lt; std::string, int &gt; | [**getEnchants**](classendstone_1_1ItemMeta.md#function-getenchants) () const = 0<br>_Returns a copy the enchantments in this_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
| virtual std::optional&lt; std::vector&lt; std::string &gt; &gt; | [**getLore**](classendstone_1_1ItemMeta.md#function-getlore) () const = 0<br>_Gets the lore that is set._  |
| virtual int | [**getRepairCost**](classendstone_1_1ItemMeta.md#function-getrepaircost) () const = 0<br>_Gets the repair penalty._  |
| virtual Type | [**getType**](classendstone_1_1ItemMeta.md#function-gettype) () const = 0<br>_Gets the type of this item meta._  |
| virtual bool | [**hasDamage**](classendstone_1_1ItemMeta.md#function-hasdamage) () const = 0<br>_Checks to see if this item has damage._  |
| virtual bool | [**hasDisplayName**](classendstone_1_1ItemMeta.md#function-hasdisplayname) () const = 0<br>_Checks for existence of a display name._  |
| virtual bool | [**hasEnchant**](classendstone_1_1ItemMeta.md#function-hasenchant) (const std::string & id) const = 0<br>_Checks for existence of the specified enchantment._  |
| virtual bool | [**hasEnchants**](classendstone_1_1ItemMeta.md#function-hasenchants) () const = 0<br>_Checks for the existence of any enchantments._  |
| virtual bool | [**hasLore**](classendstone_1_1ItemMeta.md#function-haslore) () const = 0<br>_Checks for existence of lore._  |
| virtual bool | [**hasRepairCost**](classendstone_1_1ItemMeta.md#function-hasrepaircost) () const = 0<br>_Checks to see if this has a repair penalty._  |
| virtual bool | [**isEmpty**](classendstone_1_1ItemMeta.md#function-isempty) () const = 0<br>_Checks if the item metadata is empty._  |
| virtual bool | [**isUnbreakable**](classendstone_1_1ItemMeta.md#function-isunbreakable) () const = 0<br>_Return if the unbreakable tag is true. An unbreakable item will not lose durability._  |
| virtual bool | [**removeEnchant**](classendstone_1_1ItemMeta.md#function-removeenchant) (const std::string & id) = 0<br>_Removes the specified enchantment from this item meta._  |
| virtual void | [**removeEnchants**](classendstone_1_1ItemMeta.md#function-removeenchants) () = 0<br>_Removes all enchantments from this item meta._  |
| virtual void | [**setDamage**](classendstone_1_1ItemMeta.md#function-setdamage) (int damage) = 0<br>_Sets the damage._  |
| virtual void | [**setDisplayName**](classendstone_1_1ItemMeta.md#function-setdisplayname) (std::optional&lt; std::string &gt; name) = 0<br>_Sets the display name._  |
| virtual void | [**setLore**](classendstone_1_1ItemMeta.md#function-setlore) (std::optional&lt; std::vector&lt; std::string &gt; &gt; lore) = 0<br>_Sets the lore for this item or removes lore when given std::nullopt._  |
| virtual void | [**setRepairCost**](classendstone_1_1ItemMeta.md#function-setrepaircost) (int cost) = 0<br>_Sets the repair penalty._  |
| virtual void | [**setUnbreakable**](classendstone_1_1ItemMeta.md#function-setunbreakable) (bool unbreakable) = 0<br>_Sets the unbreakable tag. An unbreakable item will not lose durability._  |
| virtual  | [**~ItemMeta**](classendstone_1_1ItemMeta.md#function-itemmeta) () = default<br> |























































------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/map_meta.h`

