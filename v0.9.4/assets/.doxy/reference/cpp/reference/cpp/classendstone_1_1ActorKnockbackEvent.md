

# Class endstone::ActorKnockbackEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorKnockbackEvent**](classendstone_1_1ActorKnockbackEvent.md)



_Called when a living entity receives knockback._ 

* `#include <endstone/event/actor/actor_knockback_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"ActorKnockbackEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorKnockbackEvent**](#function-actorknockbackevent) ([**Mob**](classendstone_1_1Mob.md) & mob, [**Actor**](classendstone_1_1Actor.md) \* source, [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; knockback) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; | [**getKnockback**](#function-getknockback) () const<br> |
|  [**Actor**](classendstone_1_1Actor.md) \* | [**getSource**](#function-getsource) () const<br>_Get the source actor that has caused knockback to the defender, if exists._  |
|  void | [**setKnockback**](#function-setknockback) ([**Vector**](classendstone_1_1Vector.md)&lt; float &gt; knockback) <br>_Sets the knockback that will be applied to the entity._  |
|   | [**~ActorKnockbackEvent**](#function-actorknockbackevent) () override<br> |


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
const std::string endstone::ActorKnockbackEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ActorKnockbackEvent 

```C++
inline explicit endstone::ActorKnockbackEvent::ActorKnockbackEvent (
    Mob & mob,
    Actor * source,
    Vector < float > knockback
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::ActorKnockbackEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getKnockback 

```C++
inline Vector < float > endstone::ActorKnockbackEvent::getKnockback () const
```



Gets the knockback that will be applied to the entity.


**Note:** this method returns a copy, changes must be applied with setKnockback(Vector)




**Returns:**

the knockback 





        

<hr>



### function getSource 

_Get the source actor that has caused knockback to the defender, if exists._ 
```C++
inline Actor * endstone::ActorKnockbackEvent::getSource () const
```





**Returns:**

actor that caused knockback, or nullptr if the knockback is not caused by an actor. 





        

<hr>



### function setKnockback 

_Sets the knockback that will be applied to the entity._ 
```C++
inline void endstone::ActorKnockbackEvent::setKnockback (
    Vector < float > knockback
) 
```





**Parameters:**


* `knockback` the knockback to apply 




        

<hr>



### function ~ActorKnockbackEvent 

```C++
endstone::ActorKnockbackEvent::~ActorKnockbackEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_knockback_event.h`

