

# Class endstone::ActorEffectEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorEffectEvent**](classendstone_1_1ActorEffectEvent.md)



_Called when an effect on a_ [_**Mob**_](classendstone_1_1Mob.md) _changes._[More...](#detailed-description)

* `#include <endstone/event/actor/actor_effect_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Action**](#enum-action)  <br>_An enum to specify how the effect changed._  |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorEffectEvent**](#function-actoreffectevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Mob**](classendstone_1_1Mob.md) &gt; & mob, [**Action**](classendstone_1_1ActorEffectEvent.md#enum-action) action, [**Effect**](classendstone_1_1Effect.md) effect) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ActorEffectEvent**](classendstone_1_1ActorEffectEvent.md)) <br> |
|  [**Action**](classendstone_1_1ActorEffectEvent.md#enum-action) | [**getAction**](#function-getaction) () const<br>_Gets how the effect changed._  |
|  [**const**](classendstone_1_1Identifier.md) [**Effect**](classendstone_1_1Effect.md) & | [**getEffect**](#function-geteffect) () const<br>_Gets the effect involved in this event._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setEffect**](#function-seteffect) ([**Effect**](classendstone_1_1Effect.md) effect) <br>_Sets the effect involved in this event._  |


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


This is fired before the change is applied. Cancelling the event prevents it, and the effect may be replaced with a different one via setEffect. 


    
## Public Types Documentation




### enum Action 

_An enum to specify how the effect changed._ 
```C++
enum endstone::ActorEffectEvent::Action {
    Added
};
```




<hr>
## Public Functions Documentation




### function ActorEffectEvent 

```C++
inline explicit endstone::ActorEffectEvent::ActorEffectEvent (
    const  NotNull < Mob > & mob,
    Action action,
    Effect effect
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::ActorEffectEvent::ENDSTONE_EVENT (
    ActorEffectEvent
) 
```




<hr>



### function getAction 

_Gets how the effect changed._ 
```C++
inline Action endstone::ActorEffectEvent::getAction () const
```





**Returns:**

the action taken on the effect 





        

<hr>



### function getEffect 

_Gets the effect involved in this event._ 
```C++
inline const  Effect & endstone::ActorEffectEvent::getEffect () const
```





**Returns:**

the effect 





        

<hr>



### function setEffect 

_Sets the effect involved in this event._ 
```C++
inline void endstone::ActorEffectEvent::setEffect (
    Effect effect
) 
```





**Parameters:**


* `effect` the effect to apply instead 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_effect_event.h`

