

# Class endstone::BlockFromToEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockFromToEvent**](classendstone_1_1BlockFromToEvent.md)



_Represents events with a source block and a destination block, currently only applies to liquid (lava and water) and teleporting dragon eggs._ [More...](#detailed-description)

* `#include <endstone/event/block/block_from_to_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockFromToEvent**](#function-blockfromtoevent) (std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block, std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; to\_block) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**BlockFromToEvent**](classendstone_1_1BlockFromToEvent.md)) <br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getToBlock**](#function-gettoblock) () const<br>_Convenience method for getting the faced_ [_**Block**_](classendstone_1_1Block.md) _._ |


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




















## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**to\_**](#variable-to_)  <br> |




























































## Detailed Description


If a [**Block**](classendstone_1_1Block.md) From To event is cancelled, the block will not move (the liquid will not flow). 


    
## Public Functions Documentation




### function BlockFromToEvent 

```C++
inline explicit endstone::BlockFromToEvent::BlockFromToEvent (
    std::unique_ptr< Block > block,
    std::unique_ptr< Block > to_block
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::BlockFromToEvent::ENDSTONE_EVENT (
    BlockFromToEvent
) 
```




<hr>



### function getToBlock 

_Convenience method for getting the faced_ [_**Block**_](classendstone_1_1Block.md) _._
```C++
inline Block & endstone::BlockFromToEvent::getToBlock () const
```





**Returns:**

The faced [**Block**](classendstone_1_1Block.md) 





        

<hr>
## Protected Attributes Documentation




### variable to\_ 

```C++
std::unique_ptr<Block> endstone::BlockFromToEvent::to_;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_from_to_event.h`

