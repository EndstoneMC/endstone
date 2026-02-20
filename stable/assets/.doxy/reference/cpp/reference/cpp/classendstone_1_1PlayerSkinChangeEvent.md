

# Class endstone::PlayerSkinChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerSkinChangeEvent**](classendstone_1_1PlayerSkinChangeEvent.md)



_Called when a player changes their skin._ 

* `#include <endstone/event/player/player_skin_change_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerSkinChangeEvent**](classendstone_1_1PlayerSkinChangeEvent.md)) <br> |
|   | [**PlayerSkinChangeEvent**](#function-playerskinchangeevent) ([**Player**](classendstone_1_1Player.md) & player, [**Skin**](classendstone_1_1Skin.md) new\_skin, std::optional&lt; Message &gt; message) <br> |
|  [**Skin**](classendstone_1_1Skin.md) | [**getNewSkin**](#function-getnewskin) () const<br>_Gets the player's new skin._  |
|  std::optional&lt; Message &gt; | [**getSkinChangeMessage**](#function-getskinchangemessage) () const<br>_Gets the message to send to all online players for this skin change._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setSkinChangeMessage**](#function-setskinchangemessage) (std::optional&lt; Message &gt; message) <br>_Sets the message to send to all online players for this skin change._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerSkinChangeEvent::ENDSTONE_EVENT (
    PlayerSkinChangeEvent
) 
```




<hr>



### function PlayerSkinChangeEvent 

```C++
inline explicit endstone::PlayerSkinChangeEvent::PlayerSkinChangeEvent (
    Player & player,
    Skin new_skin,
    std::optional< Message > message
) 
```




<hr>



### function getNewSkin 

_Gets the player's new skin._ 
```C++
inline Skin endstone::PlayerSkinChangeEvent::getNewSkin () const
```





**Returns:**

The skin that will be applied. 





        

<hr>



### function getSkinChangeMessage 

_Gets the message to send to all online players for this skin change._ 
```C++
inline std::optional< Message > endstone::PlayerSkinChangeEvent::getSkinChangeMessage () const
```





**Returns:**

Message to appear to other players on the server. 





        

<hr>



### function setSkinChangeMessage 

_Sets the message to send to all online players for this skin change._ 
```C++
inline void endstone::PlayerSkinChangeEvent::setSkinChangeMessage (
    std::optional< Message > message
) 
```





**Parameters:**


* `message` Message to appear to other players on the server. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_skin_change_event.h`

