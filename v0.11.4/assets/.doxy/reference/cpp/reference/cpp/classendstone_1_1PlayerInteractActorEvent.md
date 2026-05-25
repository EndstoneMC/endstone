

# Class endstone::PlayerInteractActorEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerInteractActorEvent**](classendstone_1_1PlayerInteractActorEvent.md)



_Represents an event that is called when a player right-clicks an actor._ 

* `#include <endstone/event/player/player_interact_actor_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerInteractActorEvent**](classendstone_1_1PlayerInteractActorEvent.md)) <br> |
|   | [**PlayerInteractActorEvent**](#function-playerinteractactorevent) ([**Player**](classendstone_1_1Player.md) & player, [**Actor**](classendstone_1_1Actor.md) & actor) <br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**getActor**](#function-getactor) () const<br>_Gets the actor that was right-clicked by the player._  |
|   | [**~PlayerInteractActorEvent**](#function-playerinteractactorevent) () override<br> |


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
endstone::PlayerInteractActorEvent::ENDSTONE_EVENT (
    PlayerInteractActorEvent
) 
```




<hr>



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



### function ~PlayerInteractActorEvent 

```C++
endstone::PlayerInteractActorEvent::~PlayerInteractActorEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_interact_actor_event.h`

