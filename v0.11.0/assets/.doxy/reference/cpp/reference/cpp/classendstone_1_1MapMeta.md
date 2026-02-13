

# Class endstone::MapMeta



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**MapMeta**](classendstone_1_1MapMeta.md)



_Represents the metadata for a map item._ 

* `#include <endstone/inventory/meta/map_meta.h>`



Inherits the following classes: [endstone::ItemMeta](classendstone_1_1ItemMeta.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::int64\_t | [**MapId**](#typedef-mapid)  <br> |


## Public Types inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](classendstone_1_1ItemMeta.md#enum-type)  <br> |






































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual MapId | [**getMapId**](#function-getmapid) () const = 0<br>_Gets the map ID that is set. This is used to determine what map is displayed._  |
| virtual [**MapView**](classendstone_1_1MapView.md) \* | [**getMapView**](#function-getmapview) () const = 0<br>_Gets the map view associated with this map item._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasMapId**](#function-hasmapid) () const = 0<br>_Checks for existence of a map ID number._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasMapView**](#function-hasmapview) () const = 0<br>_Checks for existence of an associated map._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setMapId**](#function-setmapid) (MapId id) = 0<br>_Sets the map ID. This is used to determine what map is displayed._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setMapView**](#function-setmapview) ([**const**](classendstone_1_1Identifier.md) [**MapView**](classendstone_1_1MapView.md) \* map) = 0<br>_Sets the associated map. This is used to determine what map is displayed._  |


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
| virtual [**CompoundTag**](classendstone_1_1CompoundTag.md) | [**toNbt**](classendstone_1_1ItemMeta.md#function-tonbt) () const = 0<br> |
| virtual  | [**~ItemMeta**](classendstone_1_1ItemMeta.md#function-itemmeta) () = default<br> |






















































## Public Types Documentation




### typedef MapId 

```C++
using endstone::MapMeta::MapId =  std::int64_t;
```




<hr>
## Public Functions Documentation




### function getMapId 

_Gets the map ID that is set. This is used to determine what map is displayed._ 
```C++
virtual MapId endstone::MapMeta::getMapId () const = 0
```





**Returns:**

the map ID that is set 





        

<hr>



### function getMapView 

_Gets the map view associated with this map item._ 
```C++
virtual MapView * endstone::MapMeta::getMapView () const = 0
```





**Note:**

Plugins should check that [**hasMapView()**](classendstone_1_1MapMeta.md#function-hasmapview) returns true before calling this method.




**Returns:**

the map view, or nullptr if the item [**hasMapView()**](classendstone_1_1MapMeta.md#function-hasmapview), but this map does not exist on the server 





        

<hr>



### function hasMapId 

_Checks for existence of a map ID number._ 
```C++
virtual bool endstone::MapMeta::hasMapId () const = 0
```





**Returns:**

true if this has a map ID number. 





        

<hr>



### function hasMapView 

_Checks for existence of an associated map._ 
```C++
virtual bool endstone::MapMeta::hasMapView () const = 0
```





**Returns:**

true if this item has an associated map 





        

<hr>



### function setMapId 

_Sets the map ID. This is used to determine what map is displayed._ 
```C++
virtual void endstone::MapMeta::setMapId (
    MapId id
) = 0
```





**Parameters:**


* `id` the map id to set 




        

<hr>



### function setMapView 

_Sets the associated map. This is used to determine what map is displayed._ 
```C++
virtual void endstone::MapMeta::setMapView (
    const  MapView * map
) = 0
```





**Parameters:**


* `map` the map to set 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/map_meta.h`

