

# Class endstone::ActorDismountEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorDismountEvent**](classendstone_1_1ActorDismountEvent.md)



_Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _stops riding another_[_**Actor**_](classendstone_1_1Actor.md) _._

* `#include <endstone/event/actor/actor_dismount_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorDismountEvent**](#function-actordismountevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & actor, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & vehicle) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ActorDismountEvent**](classendstone_1_1ActorDismountEvent.md)) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & | [**getVehicle**](#function-getvehicle) () const<br>_Gets the actor that is being dismounted._  |


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




### function ActorDismountEvent 

```C++
inline explicit endstone::ActorDismountEvent::ActorDismountEvent (
    const  NotNull < Actor > & actor,
    const  NotNull < Actor > & vehicle
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::ActorDismountEvent::ENDSTONE_EVENT (
    ActorDismountEvent
) 
```




<hr>



### function getVehicle 

_Gets the actor that is being dismounted._ 
```C++
inline const  NotNull < Actor > & endstone::ActorDismountEvent::getVehicle () const
```





**Returns:**

the vehicle 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_dismount_event.h`

