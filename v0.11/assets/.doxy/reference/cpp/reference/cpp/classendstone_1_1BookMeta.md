

# Class endstone::BookMeta



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BookMeta**](classendstone_1_1BookMeta.md)



_Represents the meta for a written book that can have a title, an author, and pages._ 

* `#include <endstone/inventory/meta/book_meta.h>`



Inherits the following classes: [endstone::WritableBookMeta](classendstone_1_1WritableBookMeta.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Generation**](#enum-generation)  <br>_Represents the generation (or level of copying) of a written book._  |




## Public Types inherited from endstone::ItemMeta

See [endstone::ItemMeta](classendstone_1_1ItemMeta.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](classendstone_1_1ItemMeta.md#enum-type)  <br> |
























































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::string | [**getAuthor**](#function-getauthor) () const = 0<br>_Gets the author of the book._  |
| virtual std::optional&lt; [**Generation**](classendstone_1_1BookMeta.md#enum-generation) &gt; | [**getGeneration**](#function-getgeneration) () const = 0<br>_Gets the generation of the book._  |
| virtual std::string | [**getTitle**](#function-gettitle) () const = 0<br>_Gets the title of the book._  |
| virtual bool | [**hasAuthor**](#function-hasauthor) () const = 0<br>_Checks for the existence of an author in the book._  |
| virtual bool | [**hasGeneration**](#function-hasgeneration) () const = 0<br>_Checks for the existence of generation level in the book._  |
| virtual bool | [**hasTitle**](#function-hastitle) () const = 0<br>_Checks for the existence of a title in the book._  |
| virtual void | [**setAuthor**](#function-setauthor) (std::optional&lt; std::string &gt; author) = 0<br>_Sets the author of the book. Removes author when given std::nullopt._  |
| virtual void | [**setGeneration**](#function-setgeneration) (std::optional&lt; [**Generation**](classendstone_1_1BookMeta.md#enum-generation) &gt; generation) = 0<br>_Sets the generation of the book. Removes generation when given std::nullopt._  |
| virtual void | [**setTitle**](#function-settitle) (std::optional&lt; std::string &gt; title) = 0<br>_Sets the title of the book._  |


## Public Functions inherited from endstone::WritableBookMeta

See [endstone::WritableBookMeta](classendstone_1_1WritableBookMeta.md)

| Type | Name |
| ---: | :--- |
|  void | [**addPage**](classendstone_1_1WritableBookMeta.md#function-addpage-12) (Args &&... pages) <br>_Adds new pages to the end of the book._  |
| virtual void | [**addPage**](classendstone_1_1WritableBookMeta.md#function-addpage-22) (std::vector&lt; std::string &gt; pages) = 0<br>_Adds new pages to the end of the book._  |
| virtual std::string | [**getPage**](classendstone_1_1WritableBookMeta.md#function-getpage) (int page) const = 0<br>_Gets the specified page in the book. The given page must exist._  |
| virtual int | [**getPageCount**](classendstone_1_1WritableBookMeta.md#function-getpagecount) () const = 0<br>_Gets the number of pages in the book._  |
| virtual std::vector&lt; std::string &gt; | [**getPages**](classendstone_1_1WritableBookMeta.md#function-getpages) () const = 0<br>_Gets all the pages in the book._  |
| virtual bool | [**hasPages**](classendstone_1_1WritableBookMeta.md#function-haspages) () const = 0<br>_Checks for the existence of pages in the book._  |
| virtual void | [**setPage**](classendstone_1_1WritableBookMeta.md#function-setpage) (int page, std::string data) = 0<br>_Sets the specified page in the book. Pages of the book must be contiguous._  |
|  void | [**setPages**](classendstone_1_1WritableBookMeta.md#function-setpages-12) (Args &&... pages) <br>_Clears the existing book pages, and sets the book to use the provided pages._  |
| virtual void | [**setPages**](classendstone_1_1WritableBookMeta.md#function-setpages-22) (std::vector&lt; std::string &gt; pages) = 0<br>_Maximum 50 pages with 798 characters per page._  |


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
















































































## Public Types Documentation




### enum Generation 

_Represents the generation (or level of copying) of a written book._ 
```C++
enum endstone::BookMeta::Generation {
    Original,
    CopyOfOriginal,
    CopyOfCopy
};
```




<hr>
## Public Functions Documentation




### function getAuthor 

_Gets the author of the book._ 
```C++
virtual std::string endstone::BookMeta::getAuthor () const = 0
```





**Note:**

Plugins should check that [**hasAuthor()**](classendstone_1_1BookMeta.md#function-hasauthor) returns true before calling this method.




**Returns:**

the author of the book 





        

<hr>



### function getGeneration 

_Gets the generation of the book._ 
```C++
virtual std::optional< Generation > endstone::BookMeta::getGeneration () const = 0
```





**Note:**

Plugins should check that [**hasGeneration()**](classendstone_1_1BookMeta.md#function-hasgeneration) returns true before calling this method.




**Returns:**

the generation of the book 





        

<hr>



### function getTitle 

_Gets the title of the book._ 
```C++
virtual std::string endstone::BookMeta::getTitle () const = 0
```





**Note:**

Plugins should check that [**hasTitle()**](classendstone_1_1BookMeta.md#function-hastitle) returns true before calling this method.




**Returns:**

the title of the book 





        

<hr>



### function hasAuthor 

_Checks for the existence of an author in the book._ 
```C++
virtual bool endstone::BookMeta::hasAuthor () const = 0
```





**Returns:**

true if the book has an author 





        

<hr>



### function hasGeneration 

_Checks for the existence of generation level in the book._ 
```C++
virtual bool endstone::BookMeta::hasGeneration () const = 0
```





**Returns:**

true if the book has a generation level 





        

<hr>



### function hasTitle 

_Checks for the existence of a title in the book._ 
```C++
virtual bool endstone::BookMeta::hasTitle () const = 0
```





**Returns:**

true if the book has a title 





        

<hr>



### function setAuthor 

_Sets the author of the book. Removes author when given std::nullopt._ 
```C++
virtual void endstone::BookMeta::setAuthor (
    std::optional< std::string > author
) = 0
```





**Parameters:**


* `author` the author to set 




        

<hr>



### function setGeneration 

_Sets the generation of the book. Removes generation when given std::nullopt._ 
```C++
virtual void endstone::BookMeta::setGeneration (
    std::optional< Generation > generation
) = 0
```





**Parameters:**


* `generation` the generation to set 




        

<hr>



### function setTitle 

_Sets the title of the book._ 
```C++
virtual void endstone::BookMeta::setTitle (
    std::optional< std::string > title
) = 0
```





**Note:**

Limited to 32 characters. Removes title when given std::nullopt.




**Parameters:**


* `title` the title to set



**Returns:**

true if the title was successfully set 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/meta/book_meta.h`

