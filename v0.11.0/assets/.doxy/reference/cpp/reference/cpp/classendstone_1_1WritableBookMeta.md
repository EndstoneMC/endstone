

# Class endstone::WritableBookMeta



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**WritableBookMeta**](classendstone_1_1WritableBookMeta.md)



_Represents the meta for a writable book that can have pages._ 

* `#include <endstone/inventory/meta/writable_book_meta.h>`



Inherits the following classes: [endstone::ItemMeta](classendstone_1_1ItemMeta.md)


Inherited by the following classes: [endstone::BookMeta](classendstone_1_1BookMeta.md)














## Public Types inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](classendstone_1_1ItemMeta.md#enum-type)  <br> |






































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**void**](classendstone_1_1Identifier.md) | [**addPage**](#function-addpage-12) ([**Args**](classendstone_1_1Identifier.md) &&... pages) <br>_Adds new pages to the end of the book._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**addPage**](#function-addpage-22) (std::vector&lt; std::string &gt; pages) = 0<br>_Adds new pages to the end of the book._  |
| virtual std::string | [**getPage**](#function-getpage) ([**int**](classendstone_1_1Identifier.md) page) const = 0<br>_Gets the specified page in the book. The given page must exist._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getPageCount**](#function-getpagecount) () const = 0<br>_Gets the number of pages in the book._  |
| virtual std::vector&lt; std::string &gt; | [**getPages**](#function-getpages) () const = 0<br>_Gets all the pages in the book._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasPages**](#function-haspages) () const = 0<br>_Checks for the existence of pages in the book._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setPage**](#function-setpage) ([**int**](classendstone_1_1Identifier.md) page, std::string data) = 0<br>_Sets the specified page in the book. Pages of the book must be contiguous._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setPages**](#function-setpages-12) ([**Args**](classendstone_1_1Identifier.md) &&... pages) <br>_Clears the existing book pages, and sets the book to use the provided pages._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setPages**](#function-setpages-22) (std::vector&lt; std::string &gt; pages) = 0<br>_Maximum 50 pages with 798 characters per page._  |


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






















































## Public Functions Documentation




### function addPage [1/2]

_Adds new pages to the end of the book._ 
```C++
template<std::convertible_to< std::string >... Args>
inline void endstone::WritableBookMeta::addPage (
    Args &&... pages
) 
```





**Note:**

Up to a maximum of 50 pages with 798 characters per page.




**Parameters:**


* `pages` A list of strings, each being a page 




        

<hr>



### function addPage [2/2]

_Adds new pages to the end of the book._ 
```C++
virtual void endstone::WritableBookMeta::addPage (
    std::vector< std::string > pages
) = 0
```





**Note:**

Up to a maximum of 50 pages with 798 characters per page.




**Parameters:**


* `pages` A list of strings, each being a page 




        

<hr>



### function getPage 

_Gets the specified page in the book. The given page must exist._ 
```C++
virtual std::string endstone::WritableBookMeta::getPage (
    int page
) const = 0
```





**Note:**

Pages are 1-indexed.




**Parameters:**


* `page` the page number to get, in range [1, [**getPageCount()**](classendstone_1_1WritableBookMeta.md#function-getpagecount)] 



**Returns:**

the page from the book 





        

<hr>



### function getPageCount 

_Gets the number of pages in the book._ 
```C++
virtual int endstone::WritableBookMeta::getPageCount () const = 0
```





**Returns:**

the number of pages in the book 





        

<hr>



### function getPages 

_Gets all the pages in the book._ 
```C++
virtual std::vector< std::string > endstone::WritableBookMeta::getPages () const = 0
```





**Returns:**

list of all the pages in the book 





        

<hr>



### function hasPages 

_Checks for the existence of pages in the book._ 
```C++
virtual bool endstone::WritableBookMeta::hasPages () const = 0
```





**Returns:**

true if the book has pages 





        

<hr>



### function setPage 

_Sets the specified page in the book. Pages of the book must be contiguous._ 
```C++
virtual void endstone::WritableBookMeta::setPage (
    int page,
    std::string data
) = 0
```





**Note:**

The data can be up to 1024 characters in length, additional characters are truncated.


Pages are 1-indexed.




**Parameters:**


* `page` the page number to set, in range [1, [**getPageCount()**](classendstone_1_1WritableBookMeta.md#function-getpagecount)] 
* `data` the data to set for that page 




        

<hr>



### function setPages [1/2]

_Clears the existing book pages, and sets the book to use the provided pages._ 
```C++
template<std::convertible_to< std::string >... Args>
inline void endstone::WritableBookMeta::setPages (
    Args &&... pages
) 
```





**Note:**

Maximum 50 pages with 798 characters per page.




**Parameters:**


* `pages` A list of pages to set the book to use 




        

<hr>



### function setPages [2/2]

_Maximum 50 pages with 798 characters per page._ 
```C++
virtual void endstone::WritableBookMeta::setPages (
    std::vector< std::string > pages
) = 0
```





**Note:**

Clears the existing book pages, and sets the book to use the provided pages.







**Parameters:**


* `pages` A list of strings, each being a page 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/writable_book_meta.h`

