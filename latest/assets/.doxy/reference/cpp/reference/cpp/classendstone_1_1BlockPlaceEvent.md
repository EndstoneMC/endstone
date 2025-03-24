

# Class endstone::BlockPlaceEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockPlaceEvent**](classendstone_1_1BlockPlaceEvent.md)



_Called when a block is placed by a player._ [More...](#detailed-description)

* `#include <endstone/event/block/block_place_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**NAME**](#variable-name)   = `"BlockPlaceEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockPlaceEvent**](#function-blockplaceevent) (std::unique\_ptr&lt; [**BlockState**](classendstone_1_1BlockState.md) &gt; placed\_block, [**Block**](classendstone_1_1Block.md) & replaced\_block, [**Block**](classendstone_1_1Block.md) & placed\_against, [**Player**](classendstone_1_1Player.md) & player) <br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getBlockAgainst**](#function-getblockagainst) () const<br>_Gets the block that this block was placed against._  |
|  [**BlockState**](classendstone_1_1BlockState.md) & | [**getBlockPlacedState**](#function-getblockplacedstate) () const<br>_Gets the_ [_**BlockState**_](classendstone_1_1BlockState.md) _for the block which was placed._ |
|  [**Block**](classendstone_1_1Block.md) & | [**getBlockReplaced**](#function-getblockreplaced) () const<br>_Gets the block which was replaced._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](#function-getplayer) () const<br>_Gets the player who placed the block involved in this event._  |
|   | [**~BlockPlaceEvent**](#function-blockplaceevent) () override<br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Vector.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Vector.md) cancel) <br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Vector.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Vector.md) cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Detailed Description


If a [**BlockPlaceEvent**](classendstone_1_1BlockPlaceEvent.md) is cancelled, the block will not be placed. 


    
## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::BlockPlaceEvent::NAME;
```




<hr>
## Public Functions Documentation




### function BlockPlaceEvent 

```C++
inline explicit endstone::BlockPlaceEvent::BlockPlaceEvent (
    std::unique_ptr< BlockState > placed_block,
    Block & replaced_block,
    Block & placed_against,
    Player & player
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



### function getBlockPlacedState 

_Gets the_ [_**BlockState**_](classendstone_1_1BlockState.md) _for the block which was placed._
```C++
inline BlockState & endstone::BlockPlaceEvent::getBlockPlacedState () const
```





**Returns:**

The [**BlockState**](classendstone_1_1BlockState.md) for the block which was placed. 





        

<hr>



### function getBlockReplaced 

_Gets the block which was replaced._ 
```C++
inline Block & endstone::BlockPlaceEvent::getBlockReplaced () const
```





**Returns:**

The [**Block**](classendstone_1_1Block.md) which was replaced. 





        

<hr>



### function getEventName 

```C++
inline virtual std::string endstone::BlockPlaceEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


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

