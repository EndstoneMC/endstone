

# Class endstone::PlayerPickupArrowEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerPickupArrowEvent**](classendstone_1_1PlayerPickupArrowEvent.md)



_Represents an event that is called when a player picks up an arrow from the ground._ 

* `#include <endstone/event/player/player_pickup_arrow_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerPickupArrowEvent**](classendstone_1_1PlayerPickupArrowEvent.md)) <br> |
|   | [**PlayerPickupArrowEvent**](#function-playerpickuparrowevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & arrow) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & | [**getArrow**](#function-getarrow) () const<br>_Gets the arrow picked up by the player._  |


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
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerPickupArrowEvent::ENDSTONE_EVENT (
    PlayerPickupArrowEvent
) 
```




<hr>



### function PlayerPickupArrowEvent 

```C++
inline endstone::PlayerPickupArrowEvent::PlayerPickupArrowEvent (
    const  NotNull < Player > & player,
    const  NotNull < Actor > & arrow
) 
```




<hr>



### function getArrow 

_Gets the arrow picked up by the player._ 
```C++
inline const  NotNull < Actor > & endstone::PlayerPickupArrowEvent::getArrow () const
```





**Returns:**

the arrow picked up by the player 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_pickup_arrow_event.h`

