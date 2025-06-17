

# Class endstone::PlayerEmoteEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerEmoteEvent**](classendstone_1_1PlayerEmoteEvent.md)



_Called when a player uses an emote._ 

* `#include <endstone/event/player/player_emote_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"PlayerEmoteEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerEmoteEvent**](#function-playeremoteevent) ([**Player**](classendstone_1_1Player.md) & player, std::string emote\_id) <br> |
|  std::string | [**getEmoteId**](#function-getemoteid) () const<br>_Gets the emote ID._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|   | [**~PlayerEmoteEvent**](#function-playeremoteevent) () override<br> |


## Public Functions inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) ([**Player**](classendstone_1_1Player.md) & player) <br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](classendstone_1_1PlayerEvent.md#function-getplayer) () const<br> |
|   | [**~PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































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
    std::string emote_id
) 
```




<hr>



### function getEmoteId 

_Gets the emote ID._ 
```C++
inline std::string endstone::PlayerEmoteEvent::getEmoteId () const
```





**Returns:**

The emote ID 





        

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



### function ~PlayerEmoteEvent 

```C++
endstone::PlayerEmoteEvent::~PlayerEmoteEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_emote_event.h`

