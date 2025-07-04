

# Class endstone::PlayerInteractActorEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerInteractActorEvent**](classendstone_1_1PlayerInteractActorEvent.md)



_Represents an event that is called when a player right-clicks an actor._ 

* `#include <endstone/event/player/player_interact_actor_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"PlayerInteractActorEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerInteractActorEvent**](#function-playerinteractactorevent) ([**Player**](classendstone_1_1Player.md) & player, [**Actor**](classendstone_1_1Actor.md) & actor) <br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**getActor**](#function-getactor) () const<br>_Gets the actor that was right-clicked by the player._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|   | [**~PlayerInteractActorEvent**](#function-playerinteractactorevent) () override<br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual bool | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual void | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) (bool cancel) <br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual bool | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual void | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) (bool cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































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



### function ~PlayerInteractActorEvent 

```C++
endstone::PlayerInteractActorEvent::~PlayerInteractActorEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_interact_actor_event.h`

