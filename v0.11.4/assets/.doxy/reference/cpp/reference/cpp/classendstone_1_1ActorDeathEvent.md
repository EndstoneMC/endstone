

# Class endstone::ActorDeathEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorDeathEvent**](classendstone_1_1ActorDeathEvent.md)



_Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _dies._

* `#include <endstone/event/actor/actor_death_event.h>`



Inherits the following classes: [endstone::ActorEvent](classendstone_1_1ActorEvent.md)


Inherited by the following classes: [endstone::PlayerDeathEvent](classendstone_1_1PlayerDeathEvent.md)








































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorDeathEvent**](#function-actordeathevent) ([**Mob**](classendstone_1_1Mob.md) & actor, std::unique\_ptr&lt; [**DamageSource**](classendstone_1_1DamageSource.md) &gt; damage\_source) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ActorDeathEvent**](classendstone_1_1ActorDeathEvent.md)) <br> |
|  [**DamageSource**](classendstone_1_1DamageSource.md) & | [**getDamageSource**](#function-getdamagesource) () const<br>_Gets the source of damage which caused the death._  |


## Public Functions inherited from endstone::ActorEvent

See [endstone::ActorEvent](classendstone_1_1ActorEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) ([**ActorType**](classendstone_1_1Identifier.md) & actor) <br> |
|  [**ActorType**](classendstone_1_1Identifier.md) & | [**getActor**](classendstone_1_1ActorEvent.md#function-getactor) () const<br>_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._ |
|   | [**~ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Functions Documentation




### function ActorDeathEvent 

```C++
inline endstone::ActorDeathEvent::ActorDeathEvent (
    Mob & actor,
    std::unique_ptr< DamageSource > damage_source
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::ActorDeathEvent::ENDSTONE_EVENT (
    ActorDeathEvent
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

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_death_event.h`

