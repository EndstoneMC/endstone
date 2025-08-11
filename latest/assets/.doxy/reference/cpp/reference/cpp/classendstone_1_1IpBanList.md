

# Class endstone::IpBanList



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**IpBanList**](classendstone_1_1IpBanList.md)



_Represents a ban list, containing banned IP addresses._ 

* `#include <endstone/ban/ip_ban_list.h>`



Inherits the following classes: [endstone::BanList](classendstone_1_1BanList.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**IpBanEntry**](classendstone_1_1IpBanEntry.md) & | [**addBan**](#function-addban-12) (std::string address, std::optional&lt; std::string &gt; reason, std::optional&lt; BanEntry::Date &gt; expires, std::optional&lt; std::string &gt; source) override = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**IpBanEntry**](classendstone_1_1IpBanEntry.md) & | [**addBan**](#function-addban-22) (std::string address, std::optional&lt; std::string &gt; reason, std::chrono::seconds duration, std::optional&lt; std::string &gt; source) override = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**const**](classendstone_1_1Vector.md) [**IpBanEntry**](classendstone_1_1IpBanEntry.md) \* | [**getBanEntry**](#function-getbanentry-12) (std::string address) override const = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by IP address._ |
| virtual [**IpBanEntry**](classendstone_1_1IpBanEntry.md) \* | [**getBanEntry**](#function-getbanentry-22) (std::string address) override = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by IP address._ |
| virtual std::vector&lt; [**const**](classendstone_1_1Vector.md) [**IpBanEntry**](classendstone_1_1IpBanEntry.md) \* &gt; | [**getEntries**](#function-getentries-12) () override const = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual std::vector&lt; [**IpBanEntry**](classendstone_1_1IpBanEntry.md) \* &gt; | [**getEntries**](#function-getentries-22) () override = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isBanned**](#function-isbanned) (std::string address) override const = 0<br>_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._ |
| virtual [**void**](classendstone_1_1Vector.md) | [**removeBan**](#function-removeban) (std::string address) override = 0<br>_Removes the specified IP address from this list, therefore indicating a "not banned" status._  |
|   | [**~IpBanList**](#function-ipbanlist) () override<br> |


## Public Functions inherited from endstone::BanList

See [endstone::BanList](classendstone_1_1BanList.md)

| Type | Name |
| ---: | :--- |
| virtual [**T**](classendstone_1_1Vector.md) & | [**addBan**](classendstone_1_1BanList.md#function-addban-12) (std::string target, std::optional&lt; std::string &gt; reason, std::optional&lt; BanEntry::Date &gt; expires, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**T**](classendstone_1_1Vector.md) & | [**addBan**](classendstone_1_1BanList.md#function-addban-22) (std::string target, std::optional&lt; std::string &gt; reason, std::chrono::seconds duration, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**const**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) \* | [**getBanEntry**](classendstone_1_1BanList.md#function-getbanentry-12) (std::string target) const = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by target._ |
| virtual [**T**](classendstone_1_1Vector.md) \* | [**getBanEntry**](classendstone_1_1BanList.md#function-getbanentry-22) (std::string target) = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by target._ |
| virtual std::vector&lt; [**const**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) \* &gt; | [**getEntries**](classendstone_1_1BanList.md#function-getentries-12) () const = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual std::vector&lt; [**T**](classendstone_1_1Vector.md) \* &gt; | [**getEntries**](classendstone_1_1BanList.md#function-getentries-22) () = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isBanned**](classendstone_1_1BanList.md#function-isbanned) (std::string target) const = 0<br>_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._ |
| virtual [**void**](classendstone_1_1Vector.md) | [**removeBan**](classendstone_1_1BanList.md#function-removeban) (std::string target) = 0<br>_Removes the specified target from this list, therefore indicating a "not banned" status._  |
| virtual  | [**~BanList**](classendstone_1_1BanList.md#function-banlist) () = default<br> |






















































## Public Functions Documentation




### function addBan [1/2]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual IpBanEntry & endstone::IpBanList::addBan (
    std::string address,
    std::optional< std::string > reason,
    std::optional< BanEntry::Date > expires,
    std::optional< std::string > source
) override = 0
```





**Parameters:**


* `address` The IP address of the target. 
* `reason` The reason for the ban, std::nullopt indicates implementation default. 
* `expires` The date for the ban's expiration (unban), or std::nullopt to imply forever. 
* `source` The source of the ban, std::nullopt indicates implementation default. 



**Returns:**

[**IpBanEntry**](classendstone_1_1IpBanEntry.md)& The entry for the newly created ban, or the entry for the (updated) previous ban. 





        
Implements [*endstone::BanList::addBan*](classendstone_1_1BanList.md#function-addban-12)


<hr>



### function addBan [2/2]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual IpBanEntry & endstone::IpBanList::addBan (
    std::string address,
    std::optional< std::string > reason,
    std::chrono::seconds duration,
    std::optional< std::string > source
) override = 0
```





**Parameters:**


* `address` The IP address of the target. 
* `reason` The reason for the ban, std::nullopt indicates implementation default. 
* `duration` The duration of the ban 
* `source` The source of the ban, std::nullopt indicates implementation default. 



**Returns:**

[**IpBanEntry**](classendstone_1_1IpBanEntry.md)& The entry for the newly created ban, or the entry for the (updated) previous ban. 





        
Implements [*endstone::BanList::addBan*](classendstone_1_1BanList.md#function-addban-22)


<hr>



### function getBanEntry [1/2]

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by IP address._
```C++
virtual const  IpBanEntry * endstone::IpBanList::getBanEntry (
    std::string address
) override const = 0
```





**Parameters:**


* `address` The IP address to search for. 



**Returns:**

IpBanEntry\* The corresponding entry, or nullptr if none found. 





        
Implements [*endstone::BanList::getBanEntry*](classendstone_1_1BanList.md#function-getbanentry-12)


<hr>



### function getBanEntry [2/2]

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by IP address._
```C++
virtual IpBanEntry * endstone::IpBanList::getBanEntry (
    std::string address
) override = 0
```





**Parameters:**


* `address` The IP address to search for. 



**Returns:**

IpBanEntry\* The corresponding entry, or nullptr if none found. 





        
Implements [*endstone::BanList::getBanEntry*](classendstone_1_1BanList.md#function-getbanentry-22)


<hr>



### function getEntries [1/2]

_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._
```C++
virtual std::vector< const  IpBanEntry * > endstone::IpBanList::getEntries () override const = 0
```





**Returns:**

A vector containing pointers to every entry tracked by this list. 





        
Implements [*endstone::BanList::getEntries*](classendstone_1_1BanList.md#function-getentries-12)


<hr>



### function getEntries [2/2]

_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._
```C++
virtual std::vector< IpBanEntry * > endstone::IpBanList::getEntries () override = 0
```





**Returns:**

A vector containing pointers to every entry tracked by this list. 





        
Implements [*endstone::BanList::getEntries*](classendstone_1_1BanList.md#function-getentries-22)


<hr>



### function isBanned 

_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._
```C++
virtual bool endstone::IpBanList::isBanned (
    std::string address
) override const = 0
```





**Parameters:**


* `address` The IP address to find. 



**Returns:**

true If a [**BanEntry**](classendstone_1_1BanEntry.md) exists for the target, indicating an active ban status. 




**Returns:**

false Otherwise. 





        
Implements [*endstone::BanList::isBanned*](classendstone_1_1BanList.md#function-isbanned)


<hr>



### function removeBan 

_Removes the specified IP address from this list, therefore indicating a "not banned" status._ 
```C++
virtual void endstone::IpBanList::removeBan (
    std::string address
) override = 0
```





**Parameters:**


* `address` The IP address to remove from this list. 




        
Implements [*endstone::BanList::removeBan*](classendstone_1_1BanList.md#function-removeban)


<hr>



### function ~IpBanList 

```C++
endstone::IpBanList::~IpBanList () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/ip_ban_list.h`

