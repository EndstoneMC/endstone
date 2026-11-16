

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
|  void | [**addPage**](#function-addpage-12) (Args &&... pages) <br>_Adds new pages to the end of the book._  |
| virtual void | [**addPage**](#function-addpage-22) (std::vector&lt; std::string &gt; pages) = 0<br>_Adds new pages to the end of the book._  |
| virtual std::string | [**getPage**](#function-getpage) (int page) const = 0<br>_Gets the specified page in the book. The given page must exist._  |
| virtual int | [**getPageCount**](#function-getpagecount) () const = 0<br>_Gets the number of pages in the book._  |
| virtual std::vector&lt; std::string &gt; | [**getPages**](#function-getpages) () const = 0<br>_Gets all the pages in the book._  |
| virtual bool | [**hasPages**](#function-haspages) () const = 0<br>_Checks for the existence of pages in the book._  |
| virtual void | [**setPage**](#function-setpage) (int page, std::string data) = 0<br>_Sets the specified page in the book. Pages of the book must be contiguous._  |
|  void | [**setPages**](#function-setpages-12) (Args &&... pages) <br>_Clears the existing book pages, and sets the book to use the provided pages._  |
| virtual void | [**setPages**](#function-setpages-22) (std::vector&lt; std::string &gt; pages) = 0<br>_Maximum 50 pages with 798 characters per page._  |


## Public Functions inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
| virtual bool | [**addEnchant**](classendstone_1_1ItemMeta.md#function-addenchant) (EnchantmentId id, int level, bool force) = 0<br>_Adds the specified enchantment to this item meta._  |
|  T \* | [**as**](classendstone_1_1ItemMeta.md#function-as-12) () <br> |
|  const T \* | [**as**](classendstone_1_1ItemMeta.md#function-as-22) () const<br> |
| virtual std::unique\_ptr&lt; [**ItemMeta**](classendstone_1_1ItemMeta.md) &gt; | [**clone**](classendstone_1_1ItemMeta.md#function-clone) () const = 0<br>_Creates a clone of the current metadata._  |
| virtual int | [**getDamage**](classendstone_1_1ItemMeta.md#function-getdamage) () const = 0<br>_Gets the damage._  |
| virtual std::string | [**getDisplayName**](classendstone_1_1ItemMeta.md#function-getdisplayname) () const = 0<br>_Gets the display name that is set._  |
| virtual int | [**getEnchantLevel**](classendstone_1_1ItemMeta.md#function-getenchantlevel) (EnchantmentId id) const = 0<br>_Checks for the level of the specified enchantment._  |
| virtual std::unordered\_map&lt; const [**Enchantment**](classendstone_1_1Enchantment.md) \*, int &gt; | [**getEnchants**](classendstone_1_1ItemMeta.md#function-getenchants) () const = 0<br>_Returns a copy the enchantments in this_ [_**ItemMeta**_](classendstone_1_1ItemMeta.md) _._ |
| virtual const core::ItemMetaExtras & | [**getExtras**](classendstone_1_1ItemMeta.md#function-getextras) () const = 0<br> |
| virtual std::vector&lt; std::string &gt; | [**getLore**](classendstone_1_1ItemMeta.md#function-getlore) () const = 0<br>_Gets the lore that is set._  |
| virtual int | [**getRepairCost**](classendstone_1_1ItemMeta.md#function-getrepaircost) () const = 0<br>_Gets the repair penalty._  |
| virtual Type | [**getType**](classendstone_1_1ItemMeta.md#function-gettype) () const = 0<br>_Gets the type of this item meta._  |
| virtual bool | [**hasConflictingEnchant**](classendstone_1_1ItemMeta.md#function-hasconflictingenchant) (EnchantmentId id) const = 0<br> |
| virtual bool | [**hasDamage**](classendstone_1_1ItemMeta.md#function-hasdamage) () const = 0<br>_Checks to see if this item has damage._  |
| virtual bool | [**hasDisplayName**](classendstone_1_1ItemMeta.md#function-hasdisplayname) () const = 0<br>_Checks for existence of a display name._  |
| virtual bool | [**hasEnchant**](classendstone_1_1ItemMeta.md#function-hasenchant) (EnchantmentId id) const = 0<br>_Checks for existence of the specified enchantment._  |
| virtual bool | [**hasEnchants**](classendstone_1_1ItemMeta.md#function-hasenchants) () const = 0<br>_Checks for the existence of any enchantments._  |
| virtual bool | [**hasLore**](classendstone_1_1ItemMeta.md#function-haslore) () const = 0<br>_Checks for existence of lore._  |
| virtual bool | [**hasRepairCost**](classendstone_1_1ItemMeta.md#function-hasrepaircost) () const = 0<br>_Checks to see if this has a repair penalty._  |
| virtual bool | [**isUnbreakable**](classendstone_1_1ItemMeta.md#function-isunbreakable) () const = 0<br>_Return if the unbreakable tag is true. An unbreakable item will not lose durability._  |
| virtual bool | [**removeEnchant**](classendstone_1_1ItemMeta.md#function-removeenchant) (EnchantmentId id) = 0<br>_Removes the specified enchantment from this item meta._  |
| virtual void | [**removeEnchants**](classendstone_1_1ItemMeta.md#function-removeenchants) () = 0<br>_Removes all enchantments from this item meta._  |
| virtual void | [**setDamage**](classendstone_1_1ItemMeta.md#function-setdamage) (int damage) = 0<br>_Sets the damage._  |
| virtual void | [**setDisplayName**](classendstone_1_1ItemMeta.md#function-setdisplayname) (std::optional&lt; std::string &gt; name) = 0<br>_Sets the display name._  |
| virtual void | [**setLore**](classendstone_1_1ItemMeta.md#function-setlore) (std::optional&lt; std::vector&lt; std::string &gt; &gt; lore) = 0<br>_Sets the lore for this item or removes lore when given_ `std::nullopt` _._ |
| virtual void | [**setRepairCost**](classendstone_1_1ItemMeta.md#function-setrepaircost) (int cost) = 0<br>_Sets the repair penalty._  |
| virtual void | [**setUnbreakable**](classendstone_1_1ItemMeta.md#function-setunbreakable) (bool unbreakable) = 0<br>_Sets the unbreakable tag. An unbreakable item will not lose durability._  |
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

