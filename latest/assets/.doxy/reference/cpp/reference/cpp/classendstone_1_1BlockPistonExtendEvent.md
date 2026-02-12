

# Class endstone::BlockPistonExtendEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockPistonExtendEvent**](classendstone_1_1BlockPistonExtendEvent.md)



_Called when a piston extends._ 

* `#include <endstone/event/block/block_piston_extend_event.h>`



Inherits the following classes: [endstone::BlockPistonEvent](classendstone_1_1BlockPistonEvent.md)






























































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockPistonEvent**](#function-blockpistonevent) (std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block, [**BlockFace**](namespaceendstone.md#enum-blockface) direction) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**BlockPistonExtendEvent**](classendstone_1_1BlockPistonExtendEvent.md)) <br> |


## Public Functions inherited from endstone::BlockPistonEvent

See [endstone::BlockPistonEvent](classendstone_1_1BlockPistonEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**BlockPistonEvent**](classendstone_1_1BlockPistonEvent.md#function-blockpistonevent) (std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block, [**BlockFace**](namespaceendstone.md#enum-blockface) direction) <br> |
|  [**BlockFace**](namespaceendstone.md#enum-blockface) | [**getDirection**](classendstone_1_1BlockPistonEvent.md#function-getdirection) () const<br>_Return the direction in which the piston will operate._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |










































































































## Public Functions Documentation




### function BlockPistonEvent 

```C++
inline explicit endstone::BlockPistonExtendEvent::BlockPistonEvent (
    std::unique_ptr< Block > block,
    BlockFace direction
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::BlockPistonExtendEvent::ENDSTONE_EVENT (
    BlockPistonExtendEvent
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_piston_extend_event.h`

