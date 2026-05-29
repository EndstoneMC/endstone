

# Class endstone::BanList

**template &lt;[**typename**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BanList**](classendstone_1_1BanList.md)



_Represents a ban list, containing bans._ [More...](#detailed-description)

* `#include <endstone/ban/ban_list.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**T**](classendstone_1_1Vector.md) & | [**addBan**](#function-addban-12) (std::string target, std::optional&lt; std::string &gt; reason, std::optional&lt; BanEntry::Date &gt; expires, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**T**](classendstone_1_1Vector.md) & | [**addBan**](#function-addban-22) (std::string target, std::optional&lt; std::string &gt; reason, std::chrono::seconds duration, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**const**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) \* | [**getBanEntry**](#function-getbanentry-12) (std::string target) const = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by target._ |
| virtual [**T**](classendstone_1_1Vector.md) \* | [**getBanEntry**](#function-getbanentry-22) (std::string target) = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by target._ |
| virtual std::vector&lt; [**const**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) \* &gt; | [**getEntries**](#function-getentries-12) () const = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual std::vector&lt; [**T**](classendstone_1_1Vector.md) \* &gt; | [**getEntries**](#function-getentries-22) () = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isBanned**](#function-isbanned) (std::string target) const = 0<br>_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._ |
| virtual [**void**](classendstone_1_1Vector.md) | [**removeBan**](#function-removeban) (std::string target) = 0<br>_Removes the specified target from this list, therefore indicating a "not banned" status._  |
| virtual  | [**~BanList**](#function-banlist) () = default<br> |




























## Detailed Description




**Template parameters:**


* `T` The ban target 




    
## Public Functions Documentation




### function addBan [1/2]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual T & endstone::BanList::addBan (
    std::string target,
    std::optional< std::string > reason,
    std::optional< BanEntry::Date > expires,
    std::optional< std::string > source
) = 0
```





**Parameters:**


* `target` The target of the ban. 
* `reason` The reason for the ban, std::nullopt indicates implementation default. 
* `expires` The date for the ban's expiration (unban), or std::nullopt to imply forever. 
* `source` The source of the ban, std::nullopt indicates implementation default. 



**Returns:**

T& The entry for the newly created ban, or the entry for the (updated) previous ban. 





        

<hr>



### function addBan [2/2]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual T & endstone::BanList::addBan (
    std::string target,
    std::optional< std::string > reason,
    std::chrono::seconds duration,
    std::optional< std::string > source
) = 0
```





**Parameters:**


* `target` The target of the ban. 
* `reason` The reason for the ban, std::nullopt indicates implementation default. 
* `duration` The duration of the ban 
* `source` The source of the ban, std::nullopt indicates implementation default. 



**Returns:**

T& The entry for the newly created ban, or the entry for the (updated) previous ban. 





        

<hr>



### function getBanEntry [1/2]

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by target._
```C++
virtual const  T * endstone::BanList::getBanEntry (
    std::string target
) const = 0
```





**Parameters:**


* `target` The entry parameter to search for. 



**Returns:**

T\* The corresponding entry, or nullptr if none found. 





        

<hr>



### function getBanEntry [2/2]

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by target._
```C++
virtual T * endstone::BanList::getBanEntry (
    std::string target
) = 0
```





**Parameters:**


* `target` The entry parameter to search for. 



**Returns:**

T\* The corresponding entry, or nullptr if none found. 





        

<hr>



### function getEntries [1/2]

_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._
```C++
virtual std::vector< const  T * > endstone::BanList::getEntries () const = 0
```





**Returns:**

A vector containing pointers to every entry tracked by this list. 





        

<hr>



### function getEntries [2/2]

_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._
```C++
virtual std::vector< T * > endstone::BanList::getEntries () = 0
```





**Returns:**

A vector containing pointers to every entry tracked by this list. 





        

<hr>



### function isBanned 

_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._
```C++
virtual bool endstone::BanList::isBanned (
    std::string target
) const = 0
```





**Parameters:**


* `target` The target to find. 



**Returns:**

true If a [**BanEntry**](classendstone_1_1BanEntry.md) exists for the target, indicating an active ban status. 




**Returns:**

false Otherwise. 





        

<hr>



### function removeBan 

_Removes the specified target from this list, therefore indicating a "not banned" status._ 
```C++
virtual void endstone::BanList::removeBan (
    std::string target
) = 0
```





**Parameters:**


* `target` The target to remove from this list. 




        

<hr>



### function ~BanList 

```C++
virtual endstone::BanList::~BanList () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/ban_list.h`

