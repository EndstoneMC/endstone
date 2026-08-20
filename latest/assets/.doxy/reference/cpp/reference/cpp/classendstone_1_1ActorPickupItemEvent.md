

# Class endstone::ActorPickupItemEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorPickupItemEvent**](classendstone_1_1ActorPickupItemEvent.md)



_Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _picks an item up from the ground._[More...](#detailed-description)

* `#include <endstone/event/actor/actor_pickup_item_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorPickupItemEvent**](#function-actorpickupitemevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & actor, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Item**](classendstone_1_1Item.md) &gt; & item, [**int**](classendstone_1_1Identifier.md) amount) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ActorPickupItemEvent**](classendstone_1_1ActorPickupItemEvent.md)) <br> |
|  [**int**](classendstone_1_1Identifier.md) | [**getAmount**](#function-getamount) () const<br>_Gets the number of items that will be picked up from the stack._  |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Item**](classendstone_1_1Item.md) &gt; & | [**getItem**](#function-getitem) () const<br>_Gets the_ [_**Item**_](classendstone_1_1Item.md) _picked up by the actor._ |


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


This is not called for players; see [**PlayerPickupItemEvent**](classendstone_1_1PlayerPickupItemEvent.md) instead. 


    
## Public Functions Documentation




### function ActorPickupItemEvent 

```C++
inline explicit endstone::ActorPickupItemEvent::ActorPickupItemEvent (
    const  NotNull < Actor > & actor,
    const  NotNull < Item > & item,
    int amount
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::ActorPickupItemEvent::ENDSTONE_EVENT (
    ActorPickupItemEvent
) 
```




<hr>



### function getAmount 

_Gets the number of items that will be picked up from the stack._ 
```C++
inline int endstone::ActorPickupItemEvent::getAmount () const
```





**Returns:**

the amount picked up 





        

<hr>



### function getItem 

_Gets the_ [_**Item**_](classendstone_1_1Item.md) _picked up by the actor._
```C++
inline const  NotNull < Item > & endstone::ActorPickupItemEvent::getItem () const
```





**Returns:**

the item 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_pickup_item_event.h`

