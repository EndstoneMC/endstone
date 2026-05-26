

# Class endstone::BlockPlaceEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockPlaceEvent**](classendstone_1_1BlockPlaceEvent.md)



_Called when a block is placed by a player._ [More...](#detailed-description)

* `#include <endstone/event/block/block_place_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockPlaceEvent**](#function-blockplaceevent) (std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; placed\_block, std::unique\_ptr&lt; [**BlockState**](classendstone_1_1BlockState.md) &gt; replaced\_state, std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; placed\_against, [**Player**](classendstone_1_1Player.md) & player) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**BlockPlaceEvent**](classendstone_1_1BlockPlaceEvent.md)) <br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getBlockAgainst**](#function-getblockagainst) () const<br>_Gets the block that this block was placed against._  |
|  [**Block**](classendstone_1_1Block.md) & | [**getBlockPlaced**](#function-getblockplaced) () const<br>_Gets the block placed._  |
|  [**BlockState**](classendstone_1_1BlockState.md) & | [**getBlockReplacedState**](#function-getblockreplacedstate) () const<br>_Gets the_ [_**BlockState**_](classendstone_1_1BlockState.md) _for the block which was replaced._ |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](#function-getplayer) () const<br>_Gets the player who placed the block involved in this event._  |
|   | [**~BlockPlaceEvent**](#function-blockplaceevent) () override<br> |


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


If a [**BlockPlaceEvent**](classendstone_1_1BlockPlaceEvent.md) is cancelled, the block will not be placed. 


    
## Public Functions Documentation




### function BlockPlaceEvent 

```C++
inline explicit endstone::BlockPlaceEvent::BlockPlaceEvent (
    std::unique_ptr< Block > placed_block,
    std::unique_ptr< BlockState > replaced_state,
    std::unique_ptr< Block > placed_against,
    Player & player
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::BlockPlaceEvent::ENDSTONE_EVENT (
    BlockPlaceEvent
) 
```




<hr>



### function getBlockAgainst 

_Gets the block that this block was placed against._ 
```C++
inline Block & endstone::BlockPlaceEvent::getBlockAgainst () const
```





**Returns:**

[**Block**](classendstone_1_1Block.md) the block that the new block was placed against 





        

<hr>



### function getBlockPlaced 

_Gets the block placed._ 
```C++
inline Block & endstone::BlockPlaceEvent::getBlockPlaced () const
```





**Returns:**

The [**Block**](classendstone_1_1Block.md) that was placed. 





        

<hr>



### function getBlockReplacedState 

_Gets the_ [_**BlockState**_](classendstone_1_1BlockState.md) _for the block which was replaced._
```C++
inline BlockState & endstone::BlockPlaceEvent::getBlockReplacedState () const
```





**Returns:**

The [**BlockState**](classendstone_1_1BlockState.md) of the block that was replaced. 





        

<hr>



### function getPlayer 

_Gets the player who placed the block involved in this event._ 
```C++
inline Player & endstone::BlockPlaceEvent::getPlayer () const
```





**Returns:**

The [**Player**](classendstone_1_1Player.md) who placed the block involved in this event 





        

<hr>



### function ~BlockPlaceEvent 

```C++
endstone::BlockPlaceEvent::~BlockPlaceEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_place_event.h`

