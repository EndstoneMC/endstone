

# Class endstone::PlayerBucketActorEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerBucketActorEvent**](classendstone_1_1PlayerBucketActorEvent.md)



_Represents an event that is called when a player captures an actor in a bucket._ 

* `#include <endstone/event/player/player_bucket_actor_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerBucketActorEvent**](classendstone_1_1PlayerBucketActorEvent.md)) <br> |
|   | [**PlayerBucketActorEvent**](#function-playerbucketactorevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & actor, [**ItemStack**](classendstone_1_1ItemStack.md) original\_bucket, [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) hand) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & | [**getActor**](#function-getactor) () const<br>_Gets the actor being captured._  |
|  [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) | [**getHand**](#function-gethand) () const<br>_Gets the hand used to capture the actor._  |
|  [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getOriginalBucket**](#function-getoriginalbucket) () const<br>_Gets the bucket used to capture the actor._  |


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
endstone::PlayerBucketActorEvent::ENDSTONE_EVENT (
    PlayerBucketActorEvent
) 
```




<hr>



### function PlayerBucketActorEvent 

```C++
inline endstone::PlayerBucketActorEvent::PlayerBucketActorEvent (
    const  NotNull < Player > & player,
    const  NotNull < Actor > & actor,
    ItemStack original_bucket,
    EquipmentSlot hand
) 
```




<hr>



### function getActor 

_Gets the actor being captured._ 
```C++
inline const  NotNull < Actor > & endstone::PlayerBucketActorEvent::getActor () const
```





**Returns:**

actor being captured 





        

<hr>



### function getHand 

_Gets the hand used to capture the actor._ 
```C++
inline EquipmentSlot endstone::PlayerBucketActorEvent::getHand () const
```





**Returns:**

hand used to capture the actor 





        

<hr>



### function getOriginalBucket 

_Gets the bucket used to capture the actor._ 
```C++
inline const  ItemStack & endstone::PlayerBucketActorEvent::getOriginalBucket () const
```



This refers to the bucket clicked with, i.e. a water bucket.




**Returns:**

bucket used to capture the actor 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_bucket_actor_event.h`

