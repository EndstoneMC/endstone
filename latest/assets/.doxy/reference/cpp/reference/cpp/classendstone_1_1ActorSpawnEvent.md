

# Class endstone::ActorSpawnEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorSpawnEvent**](classendstone_1_1ActorSpawnEvent.md)



_Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _is spawned into a world._[More...](#detailed-description)

* `#include <endstone/event/actor/actor_spawn_event.h>`



Inherits the following classes: [endstone::ActorEvent](classendstone_1_1ActorEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "ActorSpawnEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorSpawnEvent**](#function-actorspawnevent) ([**Actor**](classendstone_1_1Actor.md) & actor) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|   | [**~ActorSpawnEvent**](#function-actorspawnevent) () override<br> |


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
















































































# Detailed Description


If an [**Actor**](classendstone_1_1Actor.md) Spawn event is cancelled, the actor will not spawn. 


    
## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::ActorSpawnEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ActorSpawnEvent 

```C++
inline explicit endstone::ActorSpawnEvent::ActorSpawnEvent (
    Actor & actor
) 
```




<hr>



### function getEventName 


```C++
inline virtual std::string endstone::ActorSpawnEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function isCancellable 


```C++
inline virtual bool endstone::ActorSpawnEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function ~ActorSpawnEvent 

```C++
endstone::ActorSpawnEvent::~ActorSpawnEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_spawn_event.h`

