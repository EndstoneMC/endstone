

# Class endstone::PlayerQuitEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerQuitEvent**](classendstone_1_1PlayerQuitEvent.md)



_Called when a player leaves a server._ 

* `#include <endstone/event/player/player_quit_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerQuitEvent**](classendstone_1_1PlayerQuitEvent.md)) <br> |
|   | [**PlayerQuitEvent**](#function-playerquitevent) ([**Player**](classendstone_1_1Player.md) & player, std::optional&lt; Message &gt; quit\_message) <br> |
|  std::optional&lt; Message &gt; | [**getQuitMessage**](#function-getquitmessage) () const<br>_Gets the quit message to send to all online players._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setQuitMessage**](#function-setquitmessage) (std::optional&lt; Message &gt; message) <br>_Sets the quit message to send to all online players._  |


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
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















## Protected Attributes inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|  std::reference\_wrapper&lt; [**Player**](classendstone_1_1Player.md) &gt; | [**player\_**](classendstone_1_1PlayerEvent.md#variable-player_)  <br> |


























































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerQuitEvent::ENDSTONE_EVENT (
    PlayerQuitEvent
) 
```




<hr>



### function PlayerQuitEvent 

```C++
inline explicit endstone::PlayerQuitEvent::PlayerQuitEvent (
    Player & player,
    std::optional< Message > quit_message
) 
```




<hr>



### function getQuitMessage 

_Gets the quit message to send to all online players._ 
```C++
inline std::optional< Message > endstone::PlayerQuitEvent::getQuitMessage () const
```





**Returns:**

Message to appear to other players on the server. 





        

<hr>



### function setQuitMessage 

_Sets the quit message to send to all online players._ 
```C++
inline void endstone::PlayerQuitEvent::setQuitMessage (
    std::optional< Message > message
) 
```





**Parameters:**


* `message` Message to appear to other players on the server. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_quit_event.h`

