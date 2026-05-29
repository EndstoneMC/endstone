

# Class endstone::IpBanEntry



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**IpBanEntry**](classendstone_1_1IpBanEntry.md)








Inherits the following classes: [endstone::BanEntry](classendstone_1_1BanEntry.md)
















## Public Types inherited from endstone::BanEntry

See [endstone::BanEntry](classendstone_1_1BanEntry.md)

| Type | Name |
| ---: | :--- |
| typedef std::chrono::system\_clock::time\_point | [**Date**](classendstone_1_1BanEntry.md#typedef-date)  <br> |












## Public Static Attributes inherited from endstone::BanEntry

See [endstone::BanEntry](classendstone_1_1BanEntry.md)

| Type | Name |
| ---: | :--- |
|  const std::string | [**DateFormat**](classendstone_1_1BanEntry.md#variable-dateformat)   = `"%FT%T%Ez"`<br> |


























## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**IpBanEntry**](#function-ipbanentry) (std::string address) <br> |
|  std::string | [**getAddress**](#function-getaddress) () const<br>_Gets the banned IP address._  |


## Public Functions inherited from endstone::BanEntry

See [endstone::BanEntry](classendstone_1_1BanEntry.md)

| Type | Name |
| ---: | :--- |
|  Date | [**getCreated**](classendstone_1_1BanEntry.md#function-getcreated) () const<br>_Gets the date this ban entry was created._  |
|  std::optional&lt; Date &gt; | [**getExpiration**](classendstone_1_1BanEntry.md#function-getexpiration) () const<br>_Gets the date this ban expires on, or std::nullopt for no defined end date._  |
|  std::string | [**getReason**](classendstone_1_1BanEntry.md#function-getreason) () const<br>_Gets the reason for this ban._  |
|  std::string | [**getSource**](classendstone_1_1BanEntry.md#function-getsource) () const<br>_Gets the source of this ban._  |
|  void | [**setCreated**](classendstone_1_1BanEntry.md#function-setcreated) (Date created) <br>_Sets the date this ban entry was created._  |
|  void | [**setExpiration**](classendstone_1_1BanEntry.md#function-setexpiration) (std::optional&lt; Date &gt; expiration) <br>_Sets the date this ban expires on. std::nullopt values are considered "infinite" bans._  |
|  void | [**setReason**](classendstone_1_1BanEntry.md#function-setreason) (std::string reason) <br>_Sets the reason for this ban._  |
|  void | [**setSource**](classendstone_1_1BanEntry.md#function-setsource) (std::string source) <br>_Sets the source of this ban._  |






















































## Public Functions Documentation




### function IpBanEntry 

```C++
inline explicit endstone::IpBanEntry::IpBanEntry (
    std::string address
) 
```




<hr>



### function getAddress 

_Gets the banned IP address._ 
```C++
inline std::string endstone::IpBanEntry::getAddress () const
```





**Returns:**

the IP address 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/ip_ban_entry.h`

