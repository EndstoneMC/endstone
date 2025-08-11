

# Class endstone::ActorDeathEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorDeathEvent**](classendstone_1_1ActorDeathEvent.md)



_Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _dies._

* `#include <endstone/event/actor/actor_death_event.h>`



Inherits the following classes: [endstone::ActorEvent](classendstone_1_1ActorEvent.md)


Inherited by the following classes: [endstone::PlayerDeathEvent](classendstone_1_1PlayerDeathEvent.md)






























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**NAME**](#variable-name)   = `"ActorDeathEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorDeathEvent**](#function-actordeathevent) ([**Mob**](classendstone_1_1Mob.md) & actor, std::unique\_ptr&lt; [**DamageSource**](classendstone_1_1DamageSource.md) &gt; damage\_source) <br> |
|  [**DamageSource**](classendstone_1_1DamageSource.md) & | [**getDamageSource**](#function-getdamagesource) () const<br>_Gets the source of damage which caused the death._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |


## Public Functions inherited from endstone::ActorEvent

See [endstone::ActorEvent](classendstone_1_1ActorEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) ([**ActorType**](classendstone_1_1Vector.md) & actor) <br> |
|  [**ActorType**](classendstone_1_1Vector.md) & | [**getActor**](classendstone_1_1ActorEvent.md#function-getactor) () const<br>_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._ |
|   | [**~ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) ([**bool**](classendstone_1_1Vector.md) async=[**false**](classendstone_1_1Vector.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) ([**const**](classendstone_1_1Vector.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Vector.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Vector.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::ActorDeathEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ActorDeathEvent 

```C++
inline endstone::ActorDeathEvent::ActorDeathEvent (
    Mob & actor,
    std::unique_ptr< DamageSource > damage_source
) 
```




<hr>



### function getDamageSource 

_Gets the source of damage which caused the death._ 
```C++
inline DamageSource & endstone::ActorDeathEvent::getDamageSource () const
```





**Returns:**

a [**DamageSource**](classendstone_1_1DamageSource.md) detailing the source of the damage for the death. 





        

<hr>



### function getEventName 

```C++
inline virtual std::string endstone::ActorDeathEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_death_event.h`

