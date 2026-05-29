

# Class endstone::PlayerBanList



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerBanList**](classendstone_1_1PlayerBanList.md)



_Represents a ban list, containing banned players._ 

* `#include <endstone/ban/player_ban_list.h>`



Inherits the following classes: [endstone::BanList](classendstone_1_1BanList.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) & | [**addBan**](#function-addban-14) (std::string name, std::optional&lt; std::string &gt; reason, std::optional&lt; BanEntry::Date &gt; expires, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) & | [**addBan**](#function-addban-24) (std::string name, std::optional&lt; [**UUID**](classendstone_1_1UUID.md) &gt; uuid, std::optional&lt; std::string &gt; xuid, std::optional&lt; std::string &gt; reason, std::optional&lt; BanEntry::Date &gt; expires, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) & | [**addBan**](#function-addban-34) (std::string name, std::optional&lt; std::string &gt; reason, std::chrono::seconds duration, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) & | [**addBan**](#function-addban-44) (std::string name, std::optional&lt; [**UUID**](classendstone_1_1UUID.md) &gt; uuid, std::optional&lt; std::string &gt; xuid, std::optional&lt; std::string &gt; reason, std::chrono::seconds duration, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual const [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) \* | [**getBanEntry**](#function-getbanentry-14) (std::string name) const = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by player name._ |
| virtual [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) \* | [**getBanEntry**](#function-getbanentry-24) (std::string name) = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by player name._ |
| virtual const [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) \* | [**getBanEntry**](#function-getbanentry-34) (std::string name, std::optional&lt; [**UUID**](classendstone_1_1UUID.md) &gt; uuid, std::optional&lt; std::string &gt; xuid) const = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by player name,_[_**UUID**_](classendstone_1_1UUID.md) _, or XUID._ |
| virtual [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) \* | [**getBanEntry**](#function-getbanentry-44) (std::string name, std::optional&lt; [**UUID**](classendstone_1_1UUID.md) &gt; uuid, std::optional&lt; std::string &gt; xuid) = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by player name,_[_**UUID**_](classendstone_1_1UUID.md) _, or XUID._ |
| virtual std::vector&lt; const [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) \* &gt; | [**getEntries**](#function-getentries-12) () const = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual std::vector&lt; [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) \* &gt; | [**getEntries**](#function-getentries-22) () = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual bool | [**isBanned**](#function-isbanned-12) (std::string name) const = 0<br>_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._ |
| virtual bool | [**isBanned**](#function-isbanned-22) (std::string name, std::optional&lt; [**UUID**](classendstone_1_1UUID.md) &gt; uuid, std::optional&lt; std::string &gt; xuid) const = 0<br>_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._ |
| virtual void | [**removeBan**](#function-removeban-12) (std::string name) = 0<br>_Removes the specified player from this list, therefore indicating a "not banned" status._  |
| virtual void | [**removeBan**](#function-removeban-22) (std::string name, std::optional&lt; [**UUID**](classendstone_1_1UUID.md) &gt; uuid, std::optional&lt; std::string &gt; xuid) = 0<br>_Removes the specified player from this list, therefore indicating a "not banned" status._  |
| virtual  | [**~PlayerBanList**](#function-playerbanlist) () = default<br> |


## Public Functions inherited from endstone::BanList

See [endstone::BanList](classendstone_1_1BanList.md)

| Type | Name |
| ---: | :--- |
| virtual T & | [**addBan**](classendstone_1_1BanList.md#function-addban-12) (std::string target, std::optional&lt; std::string &gt; reason, std::optional&lt; BanEntry::Date &gt; expires, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual T & | [**addBan**](classendstone_1_1BanList.md#function-addban-22) (std::string target, std::optional&lt; std::string &gt; reason, std::chrono::seconds duration, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual const T \* | [**getBanEntry**](classendstone_1_1BanList.md#function-getbanentry-12) (std::string target) const = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by target._ |
| virtual T \* | [**getBanEntry**](classendstone_1_1BanList.md#function-getbanentry-22) (std::string target) = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by target._ |
| virtual std::vector&lt; const T \* &gt; | [**getEntries**](classendstone_1_1BanList.md#function-getentries-12) () const = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual std::vector&lt; T \* &gt; | [**getEntries**](classendstone_1_1BanList.md#function-getentries-22) () = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual bool | [**isBanned**](classendstone_1_1BanList.md#function-isbanned) (std::string target) const = 0<br>_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._ |
| virtual void | [**removeBan**](classendstone_1_1BanList.md#function-removeban) (std::string target) = 0<br>_Removes the specified target from this list, therefore indicating a "not banned" status._  |
| virtual  | [**~BanList**](classendstone_1_1BanList.md#function-banlist) () = default<br> |






















































## Public Functions Documentation




### function addBan [1/4]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual PlayerBanEntry & endstone::PlayerBanList::addBan (
    std::string name,
    std::optional< std::string > reason,
    std::optional< BanEntry::Date > expires,
    std::optional< std::string > source
) = 0
```





**Parameters:**


* `name` The name of the target. 
* `reason` The reason for the ban, std::nullopt indicates implementation default. 
* `expires` The date for the ban's expiration (unban), or std::nullopt to imply forever. 
* `source` The source of the ban, std::nullopt indicates implementation default. 



**Returns:**

[**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md)& The entry for the newly created ban, or the entry for the (updated) previous ban. 





        
Implements [*endstone::BanList::addBan*](classendstone_1_1BanList.md#function-addban-12)


<hr>



### function addBan [2/4]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual PlayerBanEntry & endstone::PlayerBanList::addBan (
    std::string name,
    std::optional< UUID > uuid,
    std::optional< std::string > xuid,
    std::optional< std::string > reason,
    std::optional< BanEntry::Date > expires,
    std::optional< std::string > source
) = 0
```





**Parameters:**


* `name` The name of the target. 
* `uuid` The [**UUID**](classendstone_1_1UUID.md) of the target, std::nullopt if not used. 
* `xuid` The XUID of the target, std::nullopt if not used. 
* `reason` The reason for the ban, std::nullopt indicates implementation default. 
* `expires` The date for the ban's expiration (unban), or std::nullopt to imply forever. 
* `source` The source of the ban, std::nullopt indicates implementation default. 



**Returns:**

[**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md)& The entry for the newly created ban, or the entry for the (updated) previous ban. 





        

<hr>



### function addBan [3/4]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual PlayerBanEntry & endstone::PlayerBanList::addBan (
    std::string name,
    std::optional< std::string > reason,
    std::chrono::seconds duration,
    std::optional< std::string > source
) = 0
```





**Parameters:**


* `name` The name of the target. 
* `reason` The reason for the ban, std::nullopt indicates implementation default. 
* `duration` The duration of the ban 
* `source` The source of the ban, std::nullopt indicates implementation default. 



**Returns:**

[**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md)& The entry for the newly created ban, or the entry for the (updated) previous ban. 





        
Implements [*endstone::BanList::addBan*](classendstone_1_1BanList.md#function-addban-22)


<hr>



### function addBan [4/4]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual PlayerBanEntry & endstone::PlayerBanList::addBan (
    std::string name,
    std::optional< UUID > uuid,
    std::optional< std::string > xuid,
    std::optional< std::string > reason,
    std::chrono::seconds duration,
    std::optional< std::string > source
) = 0
```





**Parameters:**


* `name` The name of the target. 
* `uuid` The [**UUID**](classendstone_1_1UUID.md) of the target, std::nullopt if not used. 
* `xuid` The XUID of the target, std::nullopt if not used. 
* `reason` The reason for the ban, std::nullopt indicates implementation default. 
* `duration` The duration of the ban 
* `source` The source of the ban, std::nullopt indicates implementation default. 



**Returns:**

[**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md)& The entry for the newly created ban, or the entry for the (updated) previous ban. 





        

<hr>



### function getBanEntry [1/4]

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by player name._
```C++
virtual const PlayerBanEntry * endstone::PlayerBanList::getBanEntry (
    std::string name
) const = 0
```





**Parameters:**


* `name` The player name to search for. 



**Returns:**

PlayerBanEntry\* The corresponding entry, or nullptr if none found. 





        
Implements [*endstone::BanList::getBanEntry*](classendstone_1_1BanList.md#function-getbanentry-12)


<hr>



### function getBanEntry [2/4]

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by player name._
```C++
virtual PlayerBanEntry * endstone::PlayerBanList::getBanEntry (
    std::string name
) = 0
```





**Parameters:**


* `name` The player name to search for. 



**Returns:**

PlayerBanEntry\* The corresponding entry, or nullptr if none found. 





        
Implements [*endstone::BanList::getBanEntry*](classendstone_1_1BanList.md#function-getbanentry-22)


<hr>



### function getBanEntry [3/4]

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by player name,_[_**UUID**_](classendstone_1_1UUID.md) _, or XUID._
```C++
virtual const PlayerBanEntry * endstone::PlayerBanList::getBanEntry (
    std::string name,
    std::optional< UUID > uuid,
    std::optional< std::string > xuid
) const = 0
```





**Parameters:**


* `name` The player name to search for. 
* `uuid` The [**UUID**](classendstone_1_1UUID.md) of the player to search for, std::nullopt if not used. 
* `xuid` The XUID of the player to search for, std::nullopt if not used. 



**Returns:**

PlayerBanEntry\* The corresponding entry, or nullptr if none found. 





        

<hr>



### function getBanEntry [4/4]

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by player name,_[_**UUID**_](classendstone_1_1UUID.md) _, or XUID._
```C++
virtual PlayerBanEntry * endstone::PlayerBanList::getBanEntry (
    std::string name,
    std::optional< UUID > uuid,
    std::optional< std::string > xuid
) = 0
```





**Parameters:**


* `name` The player name to search for. 
* `uuid` The [**UUID**](classendstone_1_1UUID.md) of the player to search for, std::nullopt if not used. 
* `xuid` The XUID of the player to search for, std::nullopt if not used. 



**Returns:**

PlayerBanEntry\* The corresponding entry, or nullptr if none found. 





        

<hr>



### function getEntries [1/2]

_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._
```C++
virtual std::vector< const PlayerBanEntry * > endstone::PlayerBanList::getEntries () const = 0
```





**Returns:**

A vector containing pointers to every entry tracked by this list. 





        
Implements [*endstone::BanList::getEntries*](classendstone_1_1BanList.md#function-getentries-12)


<hr>



### function getEntries [2/2]

_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._
```C++
virtual std::vector< PlayerBanEntry * > endstone::PlayerBanList::getEntries () = 0
```





**Returns:**

A vector containing pointers to every entry tracked by this list. 





        
Implements [*endstone::BanList::getEntries*](classendstone_1_1BanList.md#function-getentries-22)


<hr>



### function isBanned [1/2]

_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._
```C++
virtual bool endstone::PlayerBanList::isBanned (
    std::string name
) const = 0
```





**Parameters:**


* `name` The player name to find. 



**Returns:**

true If a [**BanEntry**](classendstone_1_1BanEntry.md) exists for the target, indicating an active ban status. 




**Returns:**

false Otherwise. 





        
Implements [*endstone::BanList::isBanned*](classendstone_1_1BanList.md#function-isbanned)


<hr>



### function isBanned [2/2]

_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._
```C++
virtual bool endstone::PlayerBanList::isBanned (
    std::string name,
    std::optional< UUID > uuid,
    std::optional< std::string > xuid
) const = 0
```





**Parameters:**


* `name` The player name to find. 
* `uuid` The [**UUID**](classendstone_1_1UUID.md) of the target, std::nullopt if not used. 
* `xuid` The XUID of the target, std::nullopt if not used. 



**Returns:**

true If a [**BanEntry**](classendstone_1_1BanEntry.md) exists for the target, indicating an active ban status. 




**Returns:**

false Otherwise. 





        

<hr>



### function removeBan [1/2]

_Removes the specified player from this list, therefore indicating a "not banned" status._ 
```C++
virtual void endstone::PlayerBanList::removeBan (
    std::string name
) = 0
```





**Parameters:**


* `name` The player name to remove from this list. 




        
Implements [*endstone::BanList::removeBan*](classendstone_1_1BanList.md#function-removeban)


<hr>



### function removeBan [2/2]

_Removes the specified player from this list, therefore indicating a "not banned" status._ 
```C++
virtual void endstone::PlayerBanList::removeBan (
    std::string name,
    std::optional< UUID > uuid,
    std::optional< std::string > xuid
) = 0
```





**Parameters:**


* `name` The player name to remove from this list. 
* `uuid` The unique id of the player, std::nullopt if not used. 
* `xuid` The xbox user id (xuid) of the player, std::nullopt if not used. 




        

<hr>



### function ~PlayerBanList 

```C++
virtual endstone::PlayerBanList::~PlayerBanList () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/player_ban_list.h`

