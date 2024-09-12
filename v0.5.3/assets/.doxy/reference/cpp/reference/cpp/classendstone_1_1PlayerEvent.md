

# Class endstone::PlayerEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerEvent**](classendstone_1_1PlayerEvent.md)



_Represents a player related event._ 

* `#include <endstone/event/player/player_event.h>`



Inherits the following classes: [endstone::Event](classendstone_1_1Event.md)


Inherited by the following classes: [endstone::PlayerChatEvent](classendstone_1_1PlayerChatEvent.md),  [endstone::PlayerCommandEvent](classendstone_1_1PlayerCommandEvent.md),  [endstone::PlayerDeathEvent](classendstone_1_1PlayerDeathEvent.md),  [endstone::PlayerInteractActorEvent](classendstone_1_1PlayerInteractActorEvent.md),  [endstone::PlayerInteractEvent](classendstone_1_1PlayerInteractEvent.md),  [endstone::PlayerJoinEvent](classendstone_1_1PlayerJoinEvent.md),  [endstone::PlayerKickEvent](classendstone_1_1PlayerKickEvent.md),  [endstone::PlayerLoginEvent](classendstone_1_1PlayerLoginEvent.md),  [endstone::PlayerQuitEvent](classendstone_1_1PlayerQuitEvent.md),  [endstone::PlayerTeleportEvent](classendstone_1_1PlayerTeleportEvent.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerEvent**](#function-playerevent) ([**Player**](classendstone_1_1Player.md) & player) <br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](#function-getplayer) () const<br> |
|   | [**~PlayerEvent**](#function-playerevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
| virtual bool | [**isCancellable**](classendstone_1_1Event.md#function-iscancellable) () const = 0<br> |
|  bool | [**isCancelled**](classendstone_1_1Event.md#function-iscancelled) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  void | [**setCancelled**](classendstone_1_1Event.md#function-setcancelled) (bool cancel) <br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















































## Public Functions Documentation




### function PlayerEvent 

```C++
inline explicit endstone::PlayerEvent::PlayerEvent (
    Player & player
) 
```




<hr>



### function getPlayer 


```C++
inline Player & endstone::PlayerEvent::getPlayer () const
```



Returns the player involved in this event




**Returns:**

[**Player**](classendstone_1_1Player.md) who is involved in this event 





        

<hr>



### function ~PlayerEvent 

```C++
endstone::PlayerEvent::~PlayerEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_event.h`

