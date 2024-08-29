

# Class endstone::PlayerInteractActorEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerInteractActorEvent**](classendstone_1_1PlayerInteractActorEvent.md)



_Represents an event that is called when a player right-clicks an actor._ 

* `#include <endstone/event/player/player_interact_actor_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "PlayerInteractActorEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerInteractActorEvent**](#function-playerinteractactorevent) ([**Player**](classendstone_1_1Player.md) & player, [**Actor**](classendstone_1_1Actor.md) & actor) <br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**getActor**](#function-getactor) () const<br>_Gets the actor that was right-clicked by the player._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|   | [**~PlayerInteractActorEvent**](#function-playerinteractactorevent) () override<br> |


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
| virtual bool | [**isCancellable**](classendstone_1_1Event.md#function-iscancellable) () const = 0<br> |
|  bool | [**isCancelled**](classendstone_1_1Event.md#function-iscancelled) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  void | [**setCancelled**](classendstone_1_1Event.md#function-setcancelled) (bool cancel) <br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::PlayerInteractActorEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerInteractActorEvent 

```C++
inline explicit endstone::PlayerInteractActorEvent::PlayerInteractActorEvent (
    Player & player,
    Actor & actor
) 
```




<hr>



### function getActor 

_Gets the actor that was right-clicked by the player._ 
```C++
inline Actor & endstone::PlayerInteractActorEvent::getActor () const
```





**Returns:**

actor right-clicked by player 





        

<hr>



### function getEventName 


```C++
inline virtual std::string endstone::PlayerInteractActorEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function isCancellable 


```C++
inline virtual bool endstone::PlayerInteractActorEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function ~PlayerInteractActorEvent 

```C++
endstone::PlayerInteractActorEvent::~PlayerInteractActorEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_interact_actor_event.h`

