

# Class endstone::ItemMeta



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemMeta**](classendstone_1_1ItemMeta.md)



_Represents the storage mechanism for auxiliary item data._ 

* `#include <endstone/inventory/meta/item_meta.h>`



Inherits the following classes: std::enable_shared_from_this< ItemMeta >














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::optional&lt; std::vector&lt; std::string &gt; &gt; | [**getLore**](#function-getlore) () const = 0<br>_Gets the lore that is set._  |
| virtual Type | [**getType**](#function-gettype) () const = 0<br>_Gets the type of this item meta._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasLore**](#function-haslore) () const = 0<br>_Checks for existence of lore._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setLore**](#function-setlore) (std::optional&lt; std::vector&lt; std::string &gt; &gt; lore) = 0<br>_Sets the lore for this item or removes lore when given std::nullopt._  |
| virtual  | [**~ItemMeta**](#function-itemmeta) () = default<br> |




























## Public Types Documentation




### enum Type 

```C++
enum endstone::ItemMeta::Type {
    Base = 0,
    Map = 1,
    Count = 2,
    None = Base
};
```




<hr>
## Public Functions Documentation




### function getLore 

_Gets the lore that is set._ 
```C++
virtual std::optional< std::vector< std::string > > endstone::ItemMeta::getLore () const = 0
```





**Returns:**

a list of lore that is set 





        

<hr>



### function getType 

_Gets the type of this item meta._ 
```C++
virtual Type endstone::ItemMeta::getType () const = 0
```





**Returns:**

type of this item meta 





        

<hr>



### function hasLore 

_Checks for existence of lore._ 
```C++
virtual bool endstone::ItemMeta::hasLore () const = 0
```





**Returns:**

true if this has lore 





        

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



### function ~ItemMeta 

```C++
virtual endstone::ItemMeta::~ItemMeta () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/item_meta.h`

