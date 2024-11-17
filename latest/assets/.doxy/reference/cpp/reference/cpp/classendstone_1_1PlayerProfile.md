

# Class endstone::PlayerProfile



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerProfile**](classendstone_1_1PlayerProfile.md)



_A player profile._ 

* `#include <endstone/profile/player_profile.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::optional&lt; std::string &gt; | [**getName**](#function-getname) () const = 0<br>_Gets the player name._  |
| virtual std::optional&lt; [**UUID**](classendstone_1_1UUID.md) &gt; | [**getUniqueId**](#function-getuniqueid) () const = 0<br>_Gets the player's unique id._  |
| virtual std::optional&lt; std::string &gt; | [**getXuid**](#function-getxuid) () const = 0<br>_Gets the player's xbox user id (xuid)._  |
| virtual bool | [**isComplete**](#function-iscomplete) () const = 0<br>_Checks whether this profile is complete._  |
| virtual  | [**~PlayerProfile**](#function-playerprofile) () = default<br> |




























## Public Functions Documentation




### function getName 

_Gets the player name._ 
```C++
virtual std::optional< std::string > endstone::PlayerProfile::getName () const = 0
```





**Returns:**

the player name, or std::nullopt if not available 





        

<hr>



### function getUniqueId 

_Gets the player's unique id._ 
```C++
virtual std::optional< UUID > endstone::PlayerProfile::getUniqueId () const = 0
```





**Returns:**

the player's unique id, or std::nullopt if not available 





        

<hr>



### function getXuid 

_Gets the player's xbox user id (xuid)._ 
```C++
virtual std::optional< std::string > endstone::PlayerProfile::getXuid () const = 0
```





**Returns:**

the player's xbox user id (xuid), or std::nullopt if not available 





        

<hr>



### function isComplete 

_Checks whether this profile is complete._ 
```C++
virtual bool endstone::PlayerProfile::isComplete () const = 0
```



A profile is currently considered complete if it has a name and a unique id.




**Returns:**

true if this profile is complete 





        

<hr>



### function ~PlayerProfile 

```C++
virtual endstone::PlayerProfile::~PlayerProfile () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/profile/player_profile.h`

