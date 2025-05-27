

# Class endstone::ActorDamageEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorDamageEvent**](classendstone_1_1ActorDamageEvent.md)



_Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _is damaged._

* `#include <endstone/event/actor/actor_damage_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"ActorDamageEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorDamageEvent**](#function-actordamageevent) ([**Mob**](classendstone_1_1Mob.md) & actor, std::unique\_ptr&lt; [**DamageSource**](classendstone_1_1DamageSource.md) &gt; damage\_source, const float damage) <br> |
|  float | [**getDamage**](#function-getdamage) () const<br>_Gets the raw amount of damage caused by the event._  |
|  [**DamageSource**](classendstone_1_1DamageSource.md) & | [**getDamageSource**](#function-getdamagesource) () const<br>_Get the source of damage._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  void | [**setDamage**](#function-setdamage) (const float damage) <br>_Sets the raw amount of damage caused by the event._  |


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
const std::string endstone::ActorDamageEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ActorDamageEvent 

```C++
inline endstone::ActorDamageEvent::ActorDamageEvent (
    Mob & actor,
    std::unique_ptr< DamageSource > damage_source,
    const float damage
) 
```




<hr>



### function getDamage 

_Gets the raw amount of damage caused by the event._ 
```C++
inline float endstone::ActorDamageEvent::getDamage () const
```





**Returns:**

The raw amount of damage caused by the event 





        

<hr>



### function getDamageSource 

_Get the source of damage._ 
```C++
inline DamageSource & endstone::ActorDamageEvent::getDamageSource () const
```





**Returns:**

a [**DamageSource**](classendstone_1_1DamageSource.md) detailing the source of the damage. 





        

<hr>



### function getEventName 

```C++
inline virtual std::string endstone::ActorDamageEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function setDamage 

_Sets the raw amount of damage caused by the event._ 
```C++
inline void endstone::ActorDamageEvent::setDamage (
    const float damage
) 
```





**Parameters:**


* `damage` The raw amount of damage caused by the event 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_damage_event.h`

