

# Class endstone::ActorKnockbackEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorKnockbackEvent**](classendstone_1_1ActorKnockbackEvent.md)



_Called when a living entity receives knockback._ 

* `#include <endstone/event/actor/actor_knockback_event.h>`



Inherits the following classes: [endstone::ActorEvent](classendstone_1_1ActorEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "ActorKnockbackEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorKnockbackEvent**](#function-actorknockbackevent) ([**Mob**](classendstone_1_1Mob.md) & mob, [**Actor**](classendstone_1_1Actor.md) \* source, [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; knockback) <br> |
|  [**Mob**](classendstone_1_1Mob.md) & | [**getActor**](#function-getactor) () const<br>_Returns the_ [_**Mob**_](classendstone_1_1Mob.md) _involved in this event._ |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; | [**getKnockback**](#function-getknockback) () const<br> |
|  [**Actor**](classendstone_1_1Actor.md) \* | [**getSource**](#function-getsource) () const<br>_Get the source actor that has caused knockback to the defender, if exists._  |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setKnockback**](#function-setknockback) ([**Vector**](classendstone_1_1Vector.md)&lt; float &gt; knockback) <br>_Sets the knockback that will be applied to the entity._  |
|   | [**~ActorKnockbackEvent**](#function-actorknockbackevent) () override<br> |


## Public Functions inherited from endstone::ActorEvent

See [endstone::ActorEvent](classendstone_1_1ActorEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) ([**Actor**](classendstone_1_1Actor.md) & actor) <br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**getActor**](classendstone_1_1ActorEvent.md#function-getactor) () const<br>_Returns the_ [_**Actor**_](classendstone_1_1Actor.md) _involved in this event._ |
|   | [**~ActorEvent**](classendstone_1_1ActorEvent.md#function-actorevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
| virtual bool | [**isCancellable**](classendstone_1_1Event.md#function-iscancellable) () const = 0<br> |
|  bool | [**isCancelled**](classendstone_1_1Event.md#function-iscancelled) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  void | [**setCancelled**](classendstone_1_1Event.md#function-setcancelled) (bool cancel) <br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































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



### function getActor 

_Returns the_ [_**Mob**_](classendstone_1_1Mob.md) _involved in this event._
```C++
inline Mob & endstone::ActorKnockbackEvent::getActor () const
```





**Returns:**

[**Mob**](classendstone_1_1Mob.md) which is involved in this event 





        

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



### function isCancellable 


```C++
inline virtual bool endstone::ActorKnockbackEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


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

