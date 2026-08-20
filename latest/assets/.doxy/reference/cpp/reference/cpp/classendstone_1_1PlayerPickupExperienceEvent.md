

# Class endstone::PlayerPickupExperienceEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerPickupExperienceEvent**](classendstone_1_1PlayerPickupExperienceEvent.md)



_Called when a player picks up an experience orb._ [More...](#detailed-description)

* `#include <endstone/event/player/player_pickup_experience_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerPickupExperienceEvent**](classendstone_1_1PlayerPickupExperienceEvent.md)) <br> |
|   | [**PlayerPickupExperienceEvent**](#function-playerpickupexperienceevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**int**](classendstone_1_1Identifier.md) amount) <br> |
|  [**int**](classendstone_1_1Identifier.md) | [**getAmount**](#function-getamount) () const<br>_Gets the amount of experience the orb is worth._  |


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


Cancelling the event leaves the orb in the world. 


    
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerPickupExperienceEvent::ENDSTONE_EVENT (
    PlayerPickupExperienceEvent
) 
```




<hr>



### function PlayerPickupExperienceEvent 

```C++
inline explicit endstone::PlayerPickupExperienceEvent::PlayerPickupExperienceEvent (
    const  NotNull < Player > & player,
    int amount
) 
```




<hr>



### function getAmount 

_Gets the amount of experience the orb is worth._ 
```C++
inline int endstone::PlayerPickupExperienceEvent::getAmount () const
```





**Returns:**

the experience amount 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_pickup_experience_event.h`

