

# Class endstone::BanList

**template &lt;[**typename**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BanList**](classendstone_1_1BanList.md)



_Represents a ban list, containing bans._ [More...](#detailed-description)

* `#include <endstone/ban/ban_list.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**T**](classendstone_1_1Identifier.md) | [**EntryType**](#typedef-entrytype)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**addBan**](#function-addban-12) (std::string target, std::optional&lt; std::string &gt; reason, std::optional&lt; BanEntry::Date &gt; expires, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**addBan**](#function-addban-22) (std::string target, std::optional&lt; std::string &gt; reason, std::chrono::seconds duration, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**Nullable**](classendstone_1_1Nullable.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**getBanEntry**](#function-getbanentry) (std::string target) const = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by target._ |
| virtual std::vector&lt; [**NotNull**](classendstone_1_1NotNull.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; &gt; | [**getEntries**](#function-getentries) () const = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isBanned**](#function-isbanned) (std::string target) const = 0<br>_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**removeBan**](#function-removeban) (std::string target) = 0<br>_Removes the specified target from this list, therefore indicating a "not banned" status._  |
| virtual  | [**~BanList**](#function-banlist) () = default<br> |




























## Detailed Description




**Template parameters:**


* `T` The ban target 




    
## Public Types Documentation




### typedef EntryType 

```C++
using endstone::BanList< T >::EntryType =  T;
```




<hr>
## Public Functions Documentation




### function addBan [1/2]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual NotNull < T > endstone::BanList::addBan (
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
virtual NotNull < T > endstone::BanList::addBan (
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



### function getBanEntry 

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by target._
```C++
virtual Nullable < T > endstone::BanList::getBanEntry (
    std::string target
) const = 0
```





**Parameters:**


* `target` The entry parameter to search for. 



**Returns:**

T\* The corresponding entry, or nullptr if none found. 





        

<hr>



### function getEntries 

_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._
```C++
virtual std::vector< NotNull < T > > endstone::BanList::getEntries () const = 0
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

