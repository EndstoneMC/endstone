

# Class endstone::PlayerSetSpawnEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerSetSpawnEvent**](classendstone_1_1PlayerSetSpawnEvent.md)



_Called when a player's spawn is set, either by themselves or otherwise._ [More...](#detailed-description)

* `#include <endstone/event/player/player_set_spawn_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Cause**](#enum-cause)  <br>_Represents the cause of the spawn change._  |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerSetSpawnEvent**](classendstone_1_1PlayerSetSpawnEvent.md)) <br> |
|   | [**PlayerSetSpawnEvent**](#function-playersetspawnevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**Cause**](classendstone_1_1PlayerSetSpawnEvent.md#enum-cause) cause, std::optional&lt; [**Location**](classendstone_1_1Location.md) &gt; location) <br> |
|  [**Cause**](classendstone_1_1PlayerSetSpawnEvent.md#enum-cause) | [**getCause**](#function-getcause) () const<br>_Gets the cause of this event._  |
|  [**const**](classendstone_1_1Identifier.md) std::optional&lt; [**Location**](classendstone_1_1Location.md) &gt; & | [**getLocation**](#function-getlocation) () const<br>_Gets the location that the spawn is set to._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setLocation**](#function-setlocation) (std::optional&lt; [**Location**](classendstone_1_1Location.md) &gt; location) <br>_Sets the location to be set as the spawn location._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br>_Cancels this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br>_Sets the cancellation state of this event._  |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Detailed Description


Assigning a new location through [**setLocation()**](classendstone_1_1PlayerSetSpawnEvent.md#function-setlocation) redirects the spawn that is about to be written; cancelling leaves the respawn point untouched.




**Note:**

Only the location's block coordinates and dimension are written back; Bedrock does not persist yaw/pitch for a respawn point. Cancelling stops the respawn point from changing, but not the feedback around it: `/spawnpoint` still reports success and a respawn anchor still plays its sound and message, because neither consults the setter. The event is not fired when Bedrock clears a respawn point, so `/clearspawnpoint` and breaking the bed a player is bound to are both silent. 





    
## Public Types Documentation




### enum Cause 

_Represents the cause of the spawn change._ 
```C++
enum endstone::PlayerSetSpawnEvent::Cause {
    Bed,
    RespawnAnchor,
    Command,
    Plugin,
    Unknown
};
```




<hr>
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerSetSpawnEvent::ENDSTONE_EVENT (
    PlayerSetSpawnEvent
) 
```




<hr>



### function PlayerSetSpawnEvent 

```C++
inline endstone::PlayerSetSpawnEvent::PlayerSetSpawnEvent (
    const  NotNull < Player > & player,
    Cause cause,
    std::optional< Location > location
) 
```




<hr>



### function getCause 

_Gets the cause of this event._ 
```C++
inline Cause endstone::PlayerSetSpawnEvent::getCause () const
```





**Returns:**

the cause 





        

<hr>



### function getLocation 

_Gets the location that the spawn is set to._ 
```C++
inline const std::optional< Location > & endstone::PlayerSetSpawnEvent::getLocation () const
```





**Returns:**

the spawn location, or std::nullopt if removing the location 





        

<hr>



### function setLocation 

_Sets the location to be set as the spawn location._ 
```C++
inline void endstone::PlayerSetSpawnEvent::setLocation (
    std::optional< Location > location
) 
```





**Parameters:**


* `location` the spawn location, or std::nullopt to remove the spawn location 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_set_spawn_event.h`

