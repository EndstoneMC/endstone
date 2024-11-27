

# Class endstone::PlayerBanEntry



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md)








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
|  const std::string | [**DateFormat**](classendstone_1_1BanEntry.md#variable-dateformat)   = = "%FT%T%Ez"<br> |


























## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerBanEntry**](#function-playerbanentry) (std::string name, std::optional&lt; [**UUID**](classendstone_1_1UUID.md) &gt; uuid=std::nullopt, std::optional&lt; std::string &gt; xuid=std::nullopt) <br> |
|  std::string | [**getName**](#function-getname) () const<br>_Gets the banned player's name._  |
|  std::optional&lt; [**UUID**](classendstone_1_1UUID.md) &gt; | [**getUniqueId**](#function-getuniqueid) () const<br>_Gets the banned player's unique id._  |
|  std::optional&lt; std::string &gt; | [**getXuid**](#function-getxuid) () const<br>_Gets the banned player's xbox user id (xuid)._  |


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




### function PlayerBanEntry 

```C++
inline explicit endstone::PlayerBanEntry::PlayerBanEntry (
    std::string name,
    std::optional< UUID > uuid=std::nullopt,
    std::optional< std::string > xuid=std::nullopt
) 
```




<hr>



### function getName 

_Gets the banned player's name._ 
```C++
inline std::string endstone::PlayerBanEntry::getName () const
```





**Returns:**

the player name 





        

<hr>



### function getUniqueId 

_Gets the banned player's unique id._ 
```C++
inline std::optional< UUID > endstone::PlayerBanEntry::getUniqueId () const
```





**Returns:**

the player's unique id, or std::nullopt if not available 





        

<hr>



### function getXuid 

_Gets the banned player's xbox user id (xuid)._ 
```C++
inline std::optional< std::string > endstone::PlayerBanEntry::getXuid () const
```





**Returns:**

the player's xbox user id (xuid), or std::nullopt if not available 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/ban/player_ban_entry.h`

