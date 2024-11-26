

# Class endstone::PlayerBanEntry



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md)








Inherits the following classes: [endstone::BanEntry](classendstone_1_1BanEntry.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::shared\_ptr&lt; [**PlayerProfile**](classendstone_1_1PlayerProfile.md) &gt; | [**getTarget**](#function-gettarget) () const = 0<br>_Gets the target involved._  |


## Public Functions inherited from endstone::BanEntry

See [endstone::BanEntry](classendstone_1_1BanEntry.md)

| Type | Name |
| ---: | :--- |
| virtual Date | [**getCreated**](classendstone_1_1BanEntry.md#function-getcreated) () const = 0<br>_Gets the date this ban entry was created._  |
| virtual std::optional&lt; Date &gt; | [**getExpiration**](classendstone_1_1BanEntry.md#function-getexpiration) () const = 0<br>_Gets the date this ban expires on, or std::nullopt for no defined end date._  |
| virtual std::optional&lt; std::string &gt; | [**getReason**](classendstone_1_1BanEntry.md#function-getreason) () const = 0<br>_Gets the reason for this ban._  |
| virtual std::string | [**getSource**](classendstone_1_1BanEntry.md#function-getsource) () const = 0<br>_Gets the source of this ban._  |
| virtual void | [**remove**](classendstone_1_1BanEntry.md#function-remove) () = 0<br>_Removes this ban entry from the associated ban list._  |
| virtual void | [**save**](classendstone_1_1BanEntry.md#function-save) () = 0<br>_Saves the ban entry, overwriting any previous data in the ban list._  |
| virtual void | [**setCreated**](classendstone_1_1BanEntry.md#function-setcreated) (Date created) = 0<br>_Sets the date this ban entry was created._  |
| virtual void | [**setExpiration**](classendstone_1_1BanEntry.md#function-setexpiration) (std::optional&lt; Date &gt; expiration) = 0<br>_Sets the date this ban expires on. std::nullopt values are considered "infinite" bans._  |
| virtual void | [**setReason**](classendstone_1_1BanEntry.md#function-setreason) (std::optional&lt; std::string &gt; reason) = 0<br>_Sets the reason for this ban._  |
| virtual void | [**setSource**](classendstone_1_1BanEntry.md#function-setsource) (std::string source) = 0<br>_Sets the source of this ban._  |
| virtual  | [**~BanEntry**](classendstone_1_1BanEntry.md#function-banentry) () = default<br> |








## Protected Types inherited from endstone::BanEntry

See [endstone::BanEntry](classendstone_1_1BanEntry.md)

| Type | Name |
| ---: | :--- |
| typedef std::chrono::system\_clock::time\_point | [**Date**](classendstone_1_1BanEntry.md#typedef-date)  <br> |














































## Public Functions Documentation




### function getTarget 

_Gets the target involved._ 
```C++
virtual std::shared_ptr< PlayerProfile > endstone::PlayerBanEntry::getTarget () const = 0
```





**Returns:**

the target, i.e. the banned player 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/player_ban_entry.h`

