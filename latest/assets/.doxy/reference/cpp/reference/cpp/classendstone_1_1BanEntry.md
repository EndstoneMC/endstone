

# Class endstone::BanEntry



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BanEntry**](classendstone_1_1BanEntry.md)



_A single entry from a ban list._ 

* `#include <endstone/ban/ban_entry.h>`





Inherited by the following classes: [endstone::PlayerBanEntry](classendstone_1_1PlayerBanEntry.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual Date | [**getCreated**](#function-getcreated) () const = 0<br>_Gets the date this ban entry was created._  |
| virtual std::optional&lt; Date &gt; | [**getExpiration**](#function-getexpiration) () const = 0<br>_Gets the date this ban expires on, or std::nullopt for no defined end date._  |
| virtual std::optional&lt; std::string &gt; | [**getReason**](#function-getreason) () const = 0<br>_Gets the reason for this ban._  |
| virtual std::string | [**getSource**](#function-getsource) () const = 0<br>_Gets the source of this ban._  |
| virtual void | [**remove**](#function-remove) () = 0<br>_Removes this ban entry from the associated ban list._  |
| virtual void | [**save**](#function-save) () = 0<br>_Saves the ban entry, overwriting any previous data in the ban list._  |
| virtual void | [**setCreated**](#function-setcreated) (Date created) = 0<br>_Sets the date this ban entry was created._  |
| virtual void | [**setExpiration**](#function-setexpiration) (std::optional&lt; Date &gt; expiration) = 0<br>_Sets the date this ban expires on. std::nullopt values are considered "infinite" bans._  |
| virtual void | [**setReason**](#function-setreason) (std::optional&lt; std::string &gt; reason) = 0<br>_Sets the reason for this ban._  |
| virtual void | [**setSource**](#function-setsource) (std::string source) = 0<br>_Sets the source of this ban._  |
| virtual  | [**~BanEntry**](#function-banentry) () = default<br> |




## Protected Types

| Type | Name |
| ---: | :--- |
| typedef std::chrono::system\_clock::time\_point | [**Date**](#typedef-date)  <br> |
























## Public Functions Documentation




### function getCreated 

_Gets the date this ban entry was created._ 
```C++
virtual Date endstone::BanEntry::getCreated () const = 0
```





**Returns:**

the creation date 





        

<hr>



### function getExpiration 

_Gets the date this ban expires on, or std::nullopt for no defined end date._ 
```C++
virtual std::optional< Date > endstone::BanEntry::getExpiration () const = 0
```





**Returns:**

the expiration date 





        

<hr>



### function getReason 

_Gets the reason for this ban._ 
```C++
virtual std::optional< std::string > endstone::BanEntry::getReason () const = 0
```





**Returns:**

the ban reason, or std::nullopt if not set 





        

<hr>



### function getSource 

_Gets the source of this ban._ 
```C++
virtual std::string endstone::BanEntry::getSource () const = 0
```





**Returns:**

the source of the ban 





        

<hr>



### function remove 

_Removes this ban entry from the associated ban list._ 
```C++
virtual void endstone::BanEntry::remove () = 0
```




<hr>



### function save 

_Saves the ban entry, overwriting any previous data in the ban list._ 
```C++
virtual void endstone::BanEntry::save () = 0
```



Saving the ban entry of an unbanned player will cause the player to be banned once again. 


        

<hr>



### function setCreated 

_Sets the date this ban entry was created._ 
```C++
virtual void endstone::BanEntry::setCreated (
    Date created
) = 0
```





**Parameters:**


* `created` the new created date 



**See also:** [**save()**](classendstone_1_1BanEntry.md#function-save) saving changes 



        

<hr>



### function setExpiration 

_Sets the date this ban expires on. std::nullopt values are considered "infinite" bans._ 
```C++
virtual void endstone::BanEntry::setExpiration (
    std::optional< Date > expiration
) = 0
```





**Parameters:**


* `expiration` the new expiration date, or std::nullopt to indicate an eternity 



**See also:** [**save()**](classendstone_1_1BanEntry.md#function-save) saving changes 



        

<hr>



### function setReason 

_Sets the reason for this ban._ 
```C++
virtual void endstone::BanEntry::setReason (
    std::optional< std::string > reason
) = 0
```





**Parameters:**


* `reason` the new reason, std::nullopt values assume the implementation default 



**See also:** [**save()**](classendstone_1_1BanEntry.md#function-save) saving changes 



        

<hr>



### function setSource 

_Sets the source of this ban._ 
```C++
virtual void endstone::BanEntry::setSource (
    std::string source
) = 0
```





**Parameters:**


* `source` the new source 



**See also:** [**save()**](classendstone_1_1BanEntry.md#function-save) saving changes 



        

<hr>



### function ~BanEntry 

```C++
virtual endstone::BanEntry::~BanEntry () = default
```




<hr>
## Protected Types Documentation




### typedef Date 

```C++
using endstone::BanEntry::Date =  std::chrono::system_clock::time_point;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/ban_entry.h`

