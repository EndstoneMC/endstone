

# Class endstone::OfflinePlayer



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**OfflinePlayer**](classendstone_1_1OfflinePlayer.md)



_Represents a reference to a player identity and the data belonging to a player that is stored on the disk and can, thus, be retrieved without the player needing to be online._ 

* `#include <endstone/offline_player.h>`





Inherited by the following classes: [endstone::Player](classendstone_1_1Player.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::string | [**getName**](#function-getname) () const = 0<br>_Returns the name of this player._  |
| virtual [**UUID**](classendstone_1_1UUID.md) | [**getUniqueId**](#function-getuniqueid) () const = 0<br>_Returns the_ [_**UUID**_](classendstone_1_1UUID.md) _of this player._ |
| virtual  | [**~OfflinePlayer**](#function-offlineplayer) () = default<br> |




























## Public Functions Documentation




### function getName 

_Returns the name of this player._ 
```C++
virtual std::string endstone::OfflinePlayer::getName () const = 0
```





**Returns:**

[**Player**](classendstone_1_1Player.md) name or null if we have not seen a name for this player yet 





        

<hr>



### function getUniqueId 

_Returns the_ [_**UUID**_](classendstone_1_1UUID.md) _of this player._
```C++
virtual UUID endstone::OfflinePlayer::getUniqueId () const = 0
```





**Returns:**

[**Player**](classendstone_1_1Player.md) [**UUID**](classendstone_1_1UUID.md) 





        

<hr>



### function ~OfflinePlayer 

```C++
virtual endstone::OfflinePlayer::~OfflinePlayer () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/offline_player.h`

