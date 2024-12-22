

# Class endstone::IpBanList



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**IpBanList**](classendstone_1_1IpBanList.md)



_Represents a ban list, containing banned IP addresses._ 

* `#include <endstone/ban/ip_ban_list.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**IpBanEntry**](classendstone_1_1IpBanEntry.md) & | [**addBan**](#function-addban-12) (std::string address, std::optional&lt; std::string &gt; reason, std::optional&lt; BanEntry::Date &gt; expires, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual [**IpBanEntry**](classendstone_1_1IpBanEntry.md) & | [**addBan**](#function-addban-22) (std::string address, std::optional&lt; std::string &gt; reason, std::chrono::seconds duration, std::optional&lt; std::string &gt; source) = 0<br>_Adds a ban to this list. If a previous ban exists, this will update the previous entry._  |
| virtual const [**IpBanEntry**](classendstone_1_1IpBanEntry.md) \* | [**getBanEntry**](#function-getbanentry-12) (std::string address) const = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by IP address._ |
| virtual [**IpBanEntry**](classendstone_1_1IpBanEntry.md) \* | [**getBanEntry**](#function-getbanentry-22) (std::string address) = 0<br>_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by IP address._ |
| virtual std::vector&lt; const [**IpBanEntry**](classendstone_1_1IpBanEntry.md) \* &gt; | [**getEntries**](#function-getentries-12) () const = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual std::vector&lt; [**IpBanEntry**](classendstone_1_1IpBanEntry.md) \* &gt; | [**getEntries**](#function-getentries-22) () = 0<br>_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._ |
| virtual bool | [**isBanned**](#function-isbanned) (std::string address) const = 0<br>_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._ |
| virtual void | [**removeBan**](#function-removeban) (std::string address) = 0<br>_Removes the specified IP address from this list, therefore indicating a "not banned" status._  |
| virtual  | [**~IpBanList**](#function-ipbanlist) () = default<br> |




























## Public Functions Documentation




### function addBan [1/2]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual IpBanEntry & endstone::IpBanList::addBan (
    std::string address,
    std::optional< std::string > reason,
    std::optional< BanEntry::Date > expires,
    std::optional< std::string > source
) = 0
```





**Parameters:**


* `address` The IP address of the target. 
* `reason` The reason for the ban, std::nullopt indicates implementation default. 
* `expires` The date for the ban's expiration (unban), or std::nullopt to imply forever. 
* `source` The source of the ban, std::nullopt indicates implementation default. 



**Returns:**

[**IpBanEntry**](classendstone_1_1IpBanEntry.md)& The entry for the newly created ban, or the entry for the (updated) previous ban. 





        

<hr>



### function addBan [2/2]

_Adds a ban to this list. If a previous ban exists, this will update the previous entry._ 
```C++
virtual IpBanEntry & endstone::IpBanList::addBan (
    std::string address,
    std::optional< std::string > reason,
    std::chrono::seconds duration,
    std::optional< std::string > source
) = 0
```





**Parameters:**


* `address` The IP address of the target. 
* `reason` The reason for the ban, std::nullopt indicates implementation default. 
* `duration` The duration of the ban 
* `source` The source of the ban, std::nullopt indicates implementation default. 



**Returns:**

[**IpBanEntry**](classendstone_1_1IpBanEntry.md)& The entry for the newly created ban, or the entry for the (updated) previous ban. 





        

<hr>



### function getBanEntry [1/2]

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by IP address._
```C++
virtual const IpBanEntry * endstone::IpBanList::getBanEntry (
    std::string address
) const = 0
```





**Parameters:**


* `address` The IP address to search for. 



**Returns:**

IpBanEntry\* The corresponding entry, or nullptr if none found. 





        

<hr>



### function getBanEntry [2/2]

_Gets a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _by IP address._
```C++
virtual IpBanEntry * endstone::IpBanList::getBanEntry (
    std::string address
) = 0
```





**Parameters:**


* `address` The IP address to search for. 



**Returns:**

IpBanEntry\* The corresponding entry, or nullptr if none found. 





        

<hr>



### function getEntries [1/2]

_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._
```C++
virtual std::vector< const IpBanEntry * > endstone::IpBanList::getEntries () const = 0
```





**Returns:**

A vector containing pointers to every entry tracked by this list. 





        

<hr>



### function getEntries [2/2]

_Gets a vector containing pointers to every_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _in this list._
```C++
virtual std::vector< IpBanEntry * > endstone::IpBanList::getEntries () = 0
```





**Returns:**

A vector containing pointers to every entry tracked by this list. 





        

<hr>



### function isBanned 

_Checks if a_ [_**BanEntry**_](classendstone_1_1BanEntry.md) _exists for the target, indicating an active ban status._
```C++
virtual bool endstone::IpBanList::isBanned (
    std::string address
) const = 0
```





**Parameters:**


* `address` The IP address to find. 



**Returns:**

true If a [**BanEntry**](classendstone_1_1BanEntry.md) exists for the target, indicating an active ban status. 




**Returns:**

false Otherwise. 





        

<hr>



### function removeBan 

_Removes the specified IP address from this list, therefore indicating a "not banned" status._ 
```C++
virtual void endstone::IpBanList::removeBan (
    std::string address
) = 0
```





**Parameters:**


* `address` The IP address to remove from this list. 




        

<hr>



### function ~IpBanList 

```C++
virtual endstone::IpBanList::~IpBanList () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/ip_ban_list.h`

