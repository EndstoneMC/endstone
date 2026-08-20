

# Class endstone::ActorChangeBlockEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActorChangeBlockEvent**](classendstone_1_1ActorChangeBlockEvent.md)



_Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _changes a block as part of its own behaviour, such as a creeper exploding, an enderman picking a block up, a ravager trampling crops or a zombie breaking a door._[More...](#detailed-description)

* `#include <endstone/event/actor/actor_change_block_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActorChangeBlockEvent**](#function-actorchangeblockevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & actor, std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ActorChangeBlockEvent**](classendstone_1_1ActorChangeBlockEvent.md)) <br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getBlock**](#function-getblock) () const<br>_Gets the block that will be destroyed or changed._  |


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


Unlike Bukkit's equivalent, this covers only the mob griefing paths. It is not called for falling blocks landing or for sheep eating grass, and the resulting block state is not available. Blocks broken by a player are covered by [**BlockBreakEvent**](classendstone_1_1BlockBreakEvent.md). 


    
## Public Functions Documentation




### function ActorChangeBlockEvent 

```C++
inline explicit endstone::ActorChangeBlockEvent::ActorChangeBlockEvent (
    const  NotNull < Actor > & actor,
    std::unique_ptr< Block > block
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::ActorChangeBlockEvent::ENDSTONE_EVENT (
    ActorChangeBlockEvent
) 
```




<hr>



### function getBlock 

_Gets the block that will be destroyed or changed._ 
```C++
inline Block & endstone::ActorChangeBlockEvent::getBlock () const
```





**Returns:**

the affected block 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/actor/actor_change_block_event.h`

