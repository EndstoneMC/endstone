

# Class endstone::BanEntry



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BanEntry**](classendstone_1_1BanEntry.md)



_A single entry from a ban list._ 

* `#include <endstone/ban/ban_entry.h>`





Inherited by the following classes: [endstone::IpBanEntry](classendstone_1_1IpBanEntry.md),  [endstone::PlayerBanEntry](classendstone_1_1PlayerBanEntry.md)












## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::chrono::system\_clock::time\_point | [**Date**](#typedef-date)  <br> |






## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**DateFormat**](#variable-dateformat)   = `"%FT%T%Ez"`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  Date | [**getCreated**](#function-getcreated) () const<br>_Gets the date this ban entry was created._  |
|  std::optional&lt; Date &gt; | [**getExpiration**](#function-getexpiration) () const<br>_Gets the date this ban expires on, or std::nullopt for no defined end date._  |
|  std::string | [**getReason**](#function-getreason) () const<br>_Gets the reason for this ban._  |
|  std::string | [**getSource**](#function-getsource) () const<br>_Gets the source of this ban._  |
|  void | [**setCreated**](#function-setcreated) (Date created) <br>_Sets the date this ban entry was created._  |
|  void | [**setExpiration**](#function-setexpiration) (std::optional&lt; Date &gt; expiration) <br>_Sets the date this ban expires on. std::nullopt values are considered "infinite" bans._  |
|  void | [**setReason**](#function-setreason) (std::string reason) <br>_Sets the reason for this ban._  |
|  void | [**setSource**](#function-setsource) (std::string source) <br>_Sets the source of this ban._  |




























## Public Types Documentation




### typedef Date 

```C++
using endstone::BanEntry::Date =  std::chrono::system_clock::time_point;
```




<hr>
## Public Static Attributes Documentation




### variable DateFormat 

```C++
const std::string endstone::BanEntry::DateFormat;
```




<hr>
## Public Functions Documentation




### function getCreated 

_Gets the date this ban entry was created._ 
```C++
inline Date endstone::BanEntry::getCreated () const
```





**Returns:**

the creation date 





        

<hr>



### function getExpiration 

_Gets the date this ban expires on, or std::nullopt for no defined end date._ 
```C++
inline std::optional< Date > endstone::BanEntry::getExpiration () const
```





**Returns:**

the expiration date 





        

<hr>



### function getReason 

_Gets the reason for this ban._ 
```C++
inline std::string endstone::BanEntry::getReason () const
```





**Returns:**

the ban reason 





        

<hr>



### function getSource 

_Gets the source of this ban._ 
```C++
inline std::string endstone::BanEntry::getSource () const
```





**Returns:**

the source of the ban 





        

<hr>



### function setCreated 

_Sets the date this ban entry was created._ 
```C++
inline void endstone::BanEntry::setCreated (
    Date created
) 
```





**Parameters:**


* `created` the new created date 



**See also:** save() saving changes 



        

<hr>



### function setExpiration 

_Sets the date this ban expires on. std::nullopt values are considered "infinite" bans._ 
```C++
inline void endstone::BanEntry::setExpiration (
    std::optional< Date > expiration
) 
```





**Parameters:**


* `expiration` the new expiration date, or std::nullopt to indicate an infinite ban 




        

<hr>



### function setReason 

_Sets the reason for this ban._ 
```C++
inline void endstone::BanEntry::setReason (
    std::string reason
) 
```





**Parameters:**


* `reason` the new reason, empty values assume the implementation default 



**See also:** save() saving changes 



        

<hr>



### function setSource 

_Sets the source of this ban._ 
```C++
inline void endstone::BanEntry::setSource (
    std::string source
) 
```





**Parameters:**


* `source` the new source 



**See also:** save() saving changes 



        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/ban_entry.h`

