

# Class endstone::BanEntry

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BanEntry**](classendstone_1_1BanEntry.md)



_A single entry from a ban list. This may represent either a player ban or an IP ban._ [More...](#detailed-description)

* `#include <endstone/ban/ban_entry.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::chrono::system\_clock::time\_point | [**getCreated**](#function-getcreated) () const = 0<br>_Gets the date this ban entry was created._  |
| virtual std::optional&lt; std::chrono::system\_clock::time\_point &gt; | [**getExpiration**](#function-getexpiration) () const = 0<br>_Gets the date this ban expires on, or std::nullopt for no defined end date._  |
| virtual std::optional&lt; std::string &gt; | [**getReason**](#function-getreason) () const = 0<br>_Gets the reason for this ban._  |
| virtual std::string | [**getSource**](#function-getsource) () const = 0<br>_Gets the source of this ban._  |
| virtual T & | [**getTarget**](#function-gettarget) () const = 0<br>_Gets the target involved._  |
| virtual void | [**remove**](#function-remove) () = 0<br>_Removes this ban entry from the appropriate ban list._  |
| virtual void | [**save**](#function-save) () = 0<br>_Saves the ban entry, overwriting any previous data in the ban list._  |
| virtual void | [**setCreated**](#function-setcreated) (std::chrono::system\_clock::time\_point created) = 0<br>_Sets the date this ban entry was created._  |
| virtual void | [**setExpiration**](#function-setexpiration) (std::optional&lt; std::chrono::system\_clock::time\_point &gt; expiration) = 0<br>_Sets the date this ban expires on. std::nullopt values are considered "infinite" bans._  |
| virtual void | [**setReason**](#function-setreason) (std::optional&lt; std::string &gt; reason) = 0<br>_Sets the reason for this ban._  |
| virtual void | [**setSource**](#function-setsource) (std::string source) = 0<br>_Sets the source of this ban._  |
| virtual  | [**~BanEntry**](#function-banentry) () = default<br> |




























# Detailed Description


Unsaved information is not automatically written to the implementation's ban list, instead, the [**save()**](classendstone_1_1BanEntry.md#function-save) method must be called to write the changes to the ban list. If this ban entry has expired (such as from an unban) and is no longer found in the list, the [**save()**](classendstone_1_1BanEntry.md#function-save) call will re-add it to the list, therefore banning the victim specified.


Likewise, changes to the associated [**BanList**](classendstone_1_1BanList.md) or other entries may or may not be reflected in this entry.




**Template parameters:**


* `T` The ban target 




    
## Public Functions Documentation




### function getCreated 

_Gets the date this ban entry was created._ 
```C++
virtual std::chrono::system_clock::time_point endstone::BanEntry::getCreated () const = 0
```





**Returns:**

the creation date 





        

<hr>



### function getExpiration 

_Gets the date this ban expires on, or std::nullopt for no defined end date._ 
```C++
virtual std::optional< std::chrono::system_clock::time_point > endstone::BanEntry::getExpiration () const = 0
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



### function getTarget 

_Gets the target involved._ 
```C++
virtual T & endstone::BanEntry::getTarget () const = 0
```





**Returns:**

the target, e.g. profile or IP address 





        

<hr>



### function remove 

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
    std::chrono::system_clock::time_point created
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
    std::optional< std::chrono::system_clock::time_point > expiration
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

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/ban_entry.h`

