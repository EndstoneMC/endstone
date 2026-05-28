

# Class endstone::BlockGrowEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockGrowEvent**](classendstone_1_1BlockGrowEvent.md)



_Called when a block grows naturally in the world._ [More...](#detailed-description)

* `#include <endstone/event/block/block_grow_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)


Inherited by the following classes: [endstone::BlockFormEvent](classendstone_1_1BlockFormEvent.md)








































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockGrowEvent**](#function-blockgrowevent) (std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block, std::unique\_ptr&lt; [**BlockState**](classendstone_1_1BlockState.md) &gt; new\_state) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**BlockGrowEvent**](classendstone_1_1BlockGrowEvent.md)) <br> |
|  [**BlockState**](classendstone_1_1BlockState.md) & | [**getNewState**](#function-getnewstate) () const<br>_Gets the state of the block where it will form or spread to._  |


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
















































































## Detailed Description


If a [**Block**](classendstone_1_1Block.md) Grow event is cancelled, the block will not grow. 


    
## Public Functions Documentation




### function BlockGrowEvent 

```C++
inline explicit endstone::BlockGrowEvent::BlockGrowEvent (
    std::unique_ptr< Block > block,
    std::unique_ptr< BlockState > new_state
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::BlockGrowEvent::ENDSTONE_EVENT (
    BlockGrowEvent
) 
```




<hr>



### function getNewState 

_Gets the state of the block where it will form or spread to._ 
```C++
inline BlockState & endstone::BlockGrowEvent::getNewState () const
```





**Returns:**

The block state for this events block 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_grow_event.h`

