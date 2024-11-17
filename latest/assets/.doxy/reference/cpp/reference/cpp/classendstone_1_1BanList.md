

# Class endstone::BanList

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BanList**](classendstone_1_1BanList.md)



_A class that represents a ban list, containing bans of some target type._ [More...](#detailed-description)

* `#include <endstone/ban/ban_list.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**BanEntry**](classendstone_1_1BanEntry.md)&lt; T &gt; & | [**addBan**](#function-addban-12) (T target, std::optional&lt; std::string &gt; reason, std::optional&lt; std::chrono::system\_clock::time\_point &gt; expires, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**BanEntry**](classendstone_1_1BanEntry.md)&lt; T &gt; & | [**addBan**](#function-addban-22) (T target, std::optional&lt; std::string &gt; reason, std::optional&lt; std::chrono::seconds &gt; duration, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**BanEntry**](classendstone_1_1BanEntry.md)&lt; T &gt; \* | [**getBanEntry**](#function-getbanentry) (T target) const = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by the target._ |
| virtual std::vector&lt; [**BanEntry**](classendstone_1_1BanEntry.md)&lt; T &gt; \* &gt; | [**getEntries**](#function-getentries) () const = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual bool | [**isBanned**](#function-isbanned) (T target) const = 0<br>_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._ |
| virtual void | [**removeBan**](#function-removeban) (T target) = 0<br>_Removes the specified target from this list, therefore indicating a "not banned" status._  |
| virtual  | [**~BanList**](#function-banlist) () = default<br> |




























# Detailed Description




**Template parameters:**


* `T` The ban target type 




    
## Public Functions Documentation




### function addBan [1/2]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual BanEntry < T > & endstone::BanList::addBan (
    T target,
    std::optional< std::string > reason,
    std::optional< std::chrono::system_clock::time_point > expires,
    std::optional< std::string > source
) = 0
```





**Parameters:**


* `target` The target of the ban 
* `reason` The reason for the ban, std::nullopt indicates implementation default 
* `expires` The date for the ban's expiration (unban), or std::nullopt to imply forever 
* `source` The source of the ban, std::nullopt indicates implementation default 



**Returns:**

BanEntry&lt;T&gt;& The entry for the newly created ban, or the entry for the (updated) previous ban 





        

<hr>



### function addBan [2/2]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual BanEntry < T > & endstone::BanList::addBan (
    T target,
    std::optional< std::string > reason,
    std::optional< std::chrono::seconds > duration,
    std::optional< std::string > source
) = 0
```





**Parameters:**


* `target` The target of the ban 
* `reason` The reason for the ban, std::nullopt indicates implementation default 
* `duration` The duration of the ban, or std::nullopt to imply forever 
* `source` The source of the ban, std::nullopt indicates implementation default 



**Returns:**

BanEntry&lt;T&gt;& The entry for the newly created ban, or the entry for the (updated) previous ban 





        

<hr>



### function getBanEntry 

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by the target._
```C++
virtual BanEntry < T > * endstone::BanList::getBanEntry (
    T target
) const = 0
```





**Parameters:**


* `target` The entry parameter to search for 



**Returns:**

BanEntry&lt;T&gt;\* The corresponding entry, or nullptr if none found 





        

<hr>



### function getEntries 

_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._
```C++
virtual std::vector< BanEntry < T > * > endstone::BanList::getEntries () const = 0
```





**Returns:**

std::vector&lt;BanEntry&lt;T&gt;\*&gt; A vector containing pointers to every entry tracked by this list 





        

<hr>



### function isBanned 

_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._
```C++
virtual bool endstone::BanList::isBanned (
    T target
) const = 0
```





**Parameters:**


* `target` The target to find 



**Returns:**

true If a [**BanEntry**](classendstone_1_1BanEntry.md) exists for the target, indicating an active ban status 




**Returns:**

false Otherwise 





        

<hr>



### function removeBan 

_Removes the specified target from this list, therefore indicating a "not banned" status._ 
```C++
virtual void endstone::BanList::removeBan (
    T target
) = 0
```





**Parameters:**


* `target` The target to remove from this list 




        

<hr>



### function ~BanList 

```C++
virtual endstone::BanList::~BanList () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/ban_list.h`

