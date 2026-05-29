

# Class endstone::ActorExplodeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorExplodeEvent**](classendstone_1_1ActorExplodeEvent.md)



_Called when an actor explodes._ 

* `#include <endstone/event/actor/actor_explode_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"ActorExplodeEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorExplodeEvent**](#function-actorexplodeevent) ([**Actor**](classendstone_1_1Actor.md) & actor, [**Location**](classendstone_1_1Location.md) location, BlockList blocks) <br> |
|  const BlockList & | [**getBlockList**](#function-getblocklist-12) () const<br>_Returns the list of blocks that would have been removed or were removed from the explosion event._  |
|  BlockList & | [**getBlockList**](#function-getblocklist-22) () <br>_Returns the list of blocks that would have been removed or were removed from the explosion event._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  const [**Location**](classendstone_1_1Location.md) & | [**getLocation**](#function-getlocation) () const<br>_Returns the location where the explosion happened._  |
|   | [**~ActorExplodeEvent**](#function-actorexplodeevent) () override<br> |


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
const std::string endstone::ActorExplodeEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ActorExplodeEvent 

```C++
inline explicit endstone::ActorExplodeEvent::ActorExplodeEvent (
    Actor & actor,
    Location location,
    BlockList blocks
) 
```




<hr>



### function getBlockList [1/2]

_Returns the list of blocks that would have been removed or were removed from the explosion event._ 
```C++
inline const BlockList & endstone::ActorExplodeEvent::getBlockList () const
```





**Returns:**

All blown-up blocks 





        

<hr>



### function getBlockList [2/2]

_Returns the list of blocks that would have been removed or were removed from the explosion event._ 
```C++
inline BlockList & endstone::ActorExplodeEvent::getBlockList () 
```





**Returns:**

All blown-up blocks 





        

<hr>



### function getEventName 

```C++
inline virtual std::string endstone::ActorExplodeEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getLocation 

_Returns the location where the explosion happened._ 
```C++
inline const Location & endstone::ActorExplodeEvent::getLocation () const
```



It is not possible to get this value from the Entity as the Entity no longer exists in the world.




**Returns:**

The location of the explosion 





        

<hr>



### function ~ActorExplodeEvent 

```C++
endstone::ActorExplodeEvent::~ActorExplodeEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_explode_event.h`

