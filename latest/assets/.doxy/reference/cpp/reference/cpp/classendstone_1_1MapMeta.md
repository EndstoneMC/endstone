

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










## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  constexpr auto | [**TYPE**](#variable-type)   = `Type::Map`<br> |




























## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MapMeta**](#function-mapmeta) (const [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) <br> |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](#function-clone) () override const<br>_Creates a clone of the current metadata._  |
|  std::int64\_t | [**getMapId**](#function-getmapid) () const<br>_Gets the map ID that is set. This is used to determine what map is displayed._  |
|  [**MapView**](classendstone_1_1MapView.md) \* | [**getMapView**](#function-getmapview) () const<br>_Gets the map view associated with this map item._  |
| virtual Type | [**getType**](#function-gettype) () override const<br>_Gets the type of this item meta._  |
|  bool | [**hasMapId**](#function-hasmapid) () const<br>_Checks for existence of a map ID number._  |
|  bool | [**hasMapView**](#function-hasmapview) () const<br>_Checks for existence of an associated map._  |
| virtual bool | [**isEmpty**](#function-isempty) () override const<br>_Checks if the item metadata is empty._  |
|  void | [**setMapId**](#function-setmapid) (std::int64\_t id) <br>_Sets the map ID. This is used to determine what map is displayed._  |
|  void | [**setMapView**](#function-setmapview) (const [**MapView**](classendstone_1_1MapView.md) \* map) <br>_Sets the associated map. This is used to determine what map is displayed._  |


## Public Functions inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
|   | [**ItemMeta**](classendstone_1_1ItemMeta.md#function-itemmeta) (const [**ItemMeta**](classendstone_1_1ItemMeta.md) \* meta) <br> |
|  bool | [**addEnchant**](classendstone_1_1ItemMeta.md#function-addenchant) (const std::string & id, int level, bool force=false) <br>_Adds the specified enchantment to this item meta._  |
|  T \* | [**as**](classendstone_1_1ItemMeta.md#function-as-12) () <br> |
|  const T \* | [**as**](classendstone_1_1ItemMeta.md#function-as-22) () const<br> |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](classendstone_1_1ItemMeta.md#function-clone) () const<br>_Creates a clone of the current metadata._  |
|  int | [**getDamage**](classendstone_1_1ItemMeta.md#function-getdamage) () const<br>_Gets the damage._  |
|  std::string | [**getDisplayName**](classendstone_1_1ItemMeta.md#function-getdisplayname) () const<br>_Gets the display name that is set._  |
|  int | [**getEnchantLevel**](classendstone_1_1ItemMeta.md#function-getenchantlevel) (const std::string & id) const<br>_Checks for the level of the specified enchantment._  |
|  std::unordered\_map&lt; std::string, int &gt; | [**getEnchants**](classendstone_1_1ItemMeta.md#function-getenchants) () const<br>_Returns a copy the enchantments in this_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
|  std::vector&lt; std::string &gt; | [**getLore**](classendstone_1_1ItemMeta.md#function-getlore) () const<br>_Gets the lore that is set._  |
|  int | [**getRepairCost**](classendstone_1_1ItemMeta.md#function-getrepaircost) () const<br>_Gets the repair penalty._  |
| virtual Type | [**getType**](classendstone_1_1ItemMeta.md#function-gettype) () const<br>_Gets the type of this item meta._  |
|  bool | [**hasDamage**](classendstone_1_1ItemMeta.md#function-hasdamage) () const<br>_Checks to see if this item has damage._  |
|  bool | [**hasDisplayName**](classendstone_1_1ItemMeta.md#function-hasdisplayname) () const<br>_Checks for existence of a display name._  |
|  bool | [**hasEnchant**](classendstone_1_1ItemMeta.md#function-hasenchant) (const std::string & id) const<br>_Checks for existence of the specified enchantment._  |
|  bool | [**hasEnchants**](classendstone_1_1ItemMeta.md#function-hasenchants) () const<br>_Checks for the existence of any enchantments._  |
|  bool | [**hasLore**](classendstone_1_1ItemMeta.md#function-haslore) () const<br>_Checks for existence of lore._  |
|  bool | [**hasRepairCost**](classendstone_1_1ItemMeta.md#function-hasrepaircost) () const<br>_Checks to see if this has a repair penalty._  |
| virtual bool | [**isEmpty**](classendstone_1_1ItemMeta.md#function-isempty) () const<br>_Checks if the item metadata is empty._  |
|  bool | [**isUnbreakable**](classendstone_1_1ItemMeta.md#function-isunbreakable) () const<br>_Return if the unbreakable tag is true. An unbreakable item will not lose durability._  |
|  bool | [**removeEnchant**](classendstone_1_1ItemMeta.md#function-removeenchant) (const std::string & id) <br>_Removes the specified enchantment from this item meta._  |
|  void | [**removeEnchants**](classendstone_1_1ItemMeta.md#function-removeenchants) () <br>_Removes all enchantments from this item meta._  |
|  void | [**setDamage**](classendstone_1_1ItemMeta.md#function-setdamage) (int damage) <br>_Sets the damage._  |
|  void | [**setDisplayName**](classendstone_1_1ItemMeta.md#function-setdisplayname) (std::optional&lt; std::string &gt; name) <br>_Sets the display name._  |
|  void | [**setLore**](classendstone_1_1ItemMeta.md#function-setlore) (std::optional&lt; std::vector&lt; std::string &gt; &gt; lore) <br>_Sets the lore for this item or removes lore when given std::nullopt._  |
|  void | [**setRepairCost**](classendstone_1_1ItemMeta.md#function-setrepaircost) (int cost) <br>_Sets the repair penalty._  |
|  void | [**setUnbreakable**](classendstone_1_1ItemMeta.md#function-setunbreakable) (bool unbreakable) <br>_Sets the unbreakable tag. An unbreakable item will not lose durability._  |
| virtual  | [**~ItemMeta**](classendstone_1_1ItemMeta.md#function-itemmeta) () = default<br> |














































## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**isMapEmpty**](#function-ismapempty) () const<br> |








## Public Static Attributes Documentation




### variable TYPE 

```C++
constexpr auto endstone::MapMeta::TYPE;
```




<hr>
## Public Functions Documentation




### function MapMeta 

```C++
inline explicit endstone::MapMeta::MapMeta (
    const ItemMeta * meta
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



### function getMapId 

_Gets the map ID that is set. This is used to determine what map is displayed._ 
```C++
inline std::int64_t endstone::MapMeta::getMapId () const
```





**Returns:**

the map ID that is set 





        

<hr>



### function getMapView 

_Gets the map view associated with this map item._ 
```C++
inline MapView * endstone::MapMeta::getMapView () const
```





**Note:**

Plugins should check that [**hasMapView()**](classendstone_1_1MapMeta.md#function-hasmapview) returns true before calling this method.




**Returns:**

the map view, or nullptr if the item [**hasMapView()**](classendstone_1_1MapMeta.md#function-hasmapview), but this map does not exist on the server 





        

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



### function hasMapId 

_Checks for existence of a map ID number._ 
```C++
inline bool endstone::MapMeta::hasMapId () const
```





**Returns:**

true if this has a map ID number. 





        

<hr>



### function hasMapView 

_Checks for existence of an associated map._ 
```C++
inline bool endstone::MapMeta::hasMapView () const
```





**Returns:**

true if this item has an associated map 





        

<hr>



### function isEmpty 

_Checks if the item metadata is empty._ 
```C++
inline virtual bool endstone::MapMeta::isEmpty () override const
```





**Returns:**

true if the metadata is empty, false otherwise. 





        
Implements [*endstone::ItemMeta::isEmpty*](classendstone_1_1ItemMeta.md#function-isempty)


<hr>



### function setMapId 

_Sets the map ID. This is used to determine what map is displayed._ 
```C++
inline void endstone::MapMeta::setMapId (
    std::int64_t id
) 
```





**Parameters:**


* `id` the map id to set 




        

<hr>



### function setMapView 

_Sets the associated map. This is used to determine what map is displayed._ 
```C++
inline void endstone::MapMeta::setMapView (
    const MapView * map
) 
```





**Parameters:**


* `map` the map to set 




        

<hr>
## Protected Functions Documentation




### function isMapEmpty 

```C++
inline bool endstone::MapMeta::isMapEmpty () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/map_meta.h`

