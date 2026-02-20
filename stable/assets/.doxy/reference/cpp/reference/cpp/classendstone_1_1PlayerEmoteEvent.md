

# Class endstone::PlayerEmoteEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerEmoteEvent**](classendstone_1_1PlayerEmoteEvent.md)



_Called when a player uses an emote._ 

* `#include <endstone/event/player/player_emote_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Identifier.md) std::string | [**NAME**](#variable-name)   = `"PlayerEmoteEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerEmoteEvent**](#function-playeremoteevent) ([**Player**](classendstone_1_1Player.md) & player, std::string emote\_id, [**bool**](classendstone_1_1Identifier.md) muted) <br> |
|  std::string | [**getEmoteId**](#function-getemoteid) () const<br>_Gets the emote piece ID._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isMuted**](#function-ismuted) () const<br>_Gets the muted state for the emote._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setMuted**](#function-setmuted) ([**bool**](classendstone_1_1Identifier.md) muted) <br>_Sets the muted state for the emote._  |
|   | [**~PlayerEmoteEvent**](#function-playeremoteevent) () override<br> |


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
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::PlayerEmoteEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerEmoteEvent 

```C++
inline explicit endstone::PlayerEmoteEvent::PlayerEmoteEvent (
    Player & player,
    std::string emote_id,
    bool muted
) 
```




<hr>



### function getEmoteId 

_Gets the emote piece ID._ 
```C++
inline std::string endstone::PlayerEmoteEvent::getEmoteId () const
```





**Returns:**

The emote piece ID 





        

<hr>



### function getEventName 

```C++
inline virtual std::string endstone::PlayerEmoteEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function isMuted 

_Gets the muted state for the emote._ 
```C++
inline bool endstone::PlayerEmoteEvent::isMuted () const
```



This method determines whether the emote is being executed without sending a chat message about the emote.




**Returns:**

true if the emote is muted, false otherwise. 





        

<hr>



### function setMuted 

_Sets the muted state for the emote._ 
```C++
inline void endstone::PlayerEmoteEvent::setMuted (
    bool muted
) 
```





**Note:**

If set to true, the emote will be executed silently, and no chat messages will be sent.




**Parameters:**


* `muted` true to mute the emote and disable chat messages, false to unmute it. 




        

<hr>



### function ~PlayerEmoteEvent 

```C++
endstone::PlayerEmoteEvent::~PlayerEmoteEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_emote_event.h`

