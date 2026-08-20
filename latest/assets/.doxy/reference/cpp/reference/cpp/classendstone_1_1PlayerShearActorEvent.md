

# Class endstone::PlayerShearActorEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerShearActorEvent**](classendstone_1_1PlayerShearActorEvent.md)



_Called when a player shears an_ [_**Actor**_](classendstone_1_1Actor.md) _._

* `#include <endstone/event/player/player_shear_actor_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerShearActorEvent**](classendstone_1_1PlayerShearActorEvent.md)) <br> |
|   | [**PlayerShearActorEvent**](#function-playershearactorevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & actor, [**ItemStack**](classendstone_1_1ItemStack.md) item, [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) hand) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & | [**getActor**](#function-getactor) () const<br>_Gets the_ [_**Actor**_](classendstone_1_1Actor.md) _the player is shearing._ |
|  [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) | [**getHand**](#function-gethand) () const<br>_Gets the hand used to shear the_ [_**Actor**_](classendstone_1_1Actor.md) _._ |
|  [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getItem**](#function-getitem) () const<br>_Gets the item used to shear the_ [_**Actor**_](classendstone_1_1Actor.md) _._ |


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
endstone::PlayerShearActorEvent::ENDSTONE_EVENT (
    PlayerShearActorEvent
) 
```




<hr>



### function PlayerShearActorEvent 

```C++
inline endstone::PlayerShearActorEvent::PlayerShearActorEvent (
    const  NotNull < Player > & player,
    const  NotNull < Actor > & actor,
    ItemStack item,
    EquipmentSlot hand
) 
```




<hr>



### function getActor 

_Gets the_ [_**Actor**_](classendstone_1_1Actor.md) _the player is shearing._
```C++
inline const  NotNull < Actor > & endstone::PlayerShearActorEvent::getActor () const
```





**Returns:**

the [**Actor**](classendstone_1_1Actor.md) the player is shearing 





        

<hr>



### function getHand 

_Gets the hand used to shear the_ [_**Actor**_](classendstone_1_1Actor.md) _._
```C++
inline EquipmentSlot endstone::PlayerShearActorEvent::getHand () const
```





**Returns:**

the hand 





        

<hr>



### function getItem 

_Gets the item used to shear the_ [_**Actor**_](classendstone_1_1Actor.md) _._
```C++
inline const  ItemStack & endstone::PlayerShearActorEvent::getItem () const
```





**Returns:**

the shears 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_shear_actor_event.h`

