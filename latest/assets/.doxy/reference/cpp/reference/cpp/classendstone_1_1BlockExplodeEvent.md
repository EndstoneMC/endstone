

# Class endstone::BlockExplodeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockExplodeEvent**](classendstone_1_1BlockExplodeEvent.md)



_Called when a block explodes (e.g. bed in the Nether, respawn anchor in the Overworld)._ [More...](#detailed-description)

* `#include <endstone/event/block/block_explode_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockExplodeEvent**](#function-blockexplodeevent) (std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block, BlockList blocks) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**BlockExplodeEvent**](classendstone_1_1BlockExplodeEvent.md)) <br> |
|  [**const**](classendstone_1_1Identifier.md) BlockList & | [**getBlockList**](#function-getblocklist-12) () const<br>_Returns the list of blocks that would have been removed or were removed from the explosion event._  |
|  BlockList & | [**getBlockList**](#function-getblocklist-22) () <br>_Returns the list of blocks that would have been removed or were removed from the explosion event._  |
|   | [**~BlockExplodeEvent**](#function-blockexplodeevent) () override<br> |


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


If a [**BlockExplodeEvent**](classendstone_1_1BlockExplodeEvent.md) is cancelled, the explosion will not occur. 


    
## Public Functions Documentation




### function BlockExplodeEvent 

```C++
inline explicit endstone::BlockExplodeEvent::BlockExplodeEvent (
    std::unique_ptr< Block > block,
    BlockList blocks
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::BlockExplodeEvent::ENDSTONE_EVENT (
    BlockExplodeEvent
) 
```




<hr>



### function getBlockList [1/2]

_Returns the list of blocks that would have been removed or were removed from the explosion event._ 
```C++
inline const BlockList & endstone::BlockExplodeEvent::getBlockList () const
```





**Returns:**

All blown-up blocks 





        

<hr>



### function getBlockList [2/2]

_Returns the list of blocks that would have been removed or were removed from the explosion event._ 
```C++
inline BlockList & endstone::BlockExplodeEvent::getBlockList () 
```





**Returns:**

All blown-up blocks 





        

<hr>



### function ~BlockExplodeEvent 

```C++
endstone::BlockExplodeEvent::~BlockExplodeEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_explode_event.h`

