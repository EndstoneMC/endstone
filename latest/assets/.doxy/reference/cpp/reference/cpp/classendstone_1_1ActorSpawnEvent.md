

# Class endstone::ActorSpawnEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorSpawnEvent**](classendstone_1_1ActorSpawnEvent.md)



_Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _is spawned into a world._[More...](#detailed-description)

* `#include <endstone/event/actor/actor_spawn_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"ActorSpawnEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorSpawnEvent**](#function-actorspawnevent) ([**Actor**](classendstone_1_1Actor.md) & actor) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|   | [**~ActorSpawnEvent**](#function-actorspawnevent) () override<br> |


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
















































































## Detailed Description


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



### function ~ActorSpawnEvent 

```C++
endstone::ActorSpawnEvent::~ActorSpawnEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_spawn_event.h`

