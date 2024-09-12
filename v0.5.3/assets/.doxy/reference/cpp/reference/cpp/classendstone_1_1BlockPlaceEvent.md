

# Class endstone::BlockPlaceEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockPlaceEvent**](classendstone_1_1BlockPlaceEvent.md)



_Called when a block is placed by a player._ [More...](#detailed-description)

* `#include <endstone/event/block/block_place_event.h>`



Inherits the following classes: [endstone::BlockEvent](classendstone_1_1BlockEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "BlockPlaceEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockPlaceEvent**](#function-blockplaceevent) (std::unique\_ptr&lt; [**BlockState**](classendstone_1_1BlockState.md) &gt; placed\_block, [**Block**](classendstone_1_1Block.md) & replaced\_block, [**Block**](classendstone_1_1Block.md) & placed\_against, [**Player**](classendstone_1_1Player.md) & player) <br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getBlockAgainst**](#function-getblockagainst) () const<br>_Gets the block that this block was placed against._  |
|  [**BlockState**](classendstone_1_1BlockState.md) & | [**getBlockPlacedState**](#function-getblockplacedstate) () const<br>_Gets the_ [_**BlockState**_](classendstone_1_1BlockState.md) _for the block which was placed._ |
|  [**Block**](classendstone_1_1Block.md) & | [**getBlockReplaced**](#function-getblockreplaced) () const<br>_Gets the block which was replaced._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](#function-getplayer) () const<br>_Gets the player who placed the block involved in this event._  |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|   | [**~BlockPlaceEvent**](#function-blockplaceevent) () override<br> |


## Public Functions inherited from endstone::BlockEvent

See [endstone::BlockEvent](classendstone_1_1BlockEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**BlockEvent**](classendstone_1_1BlockEvent.md#function-blockevent) ([**Block**](classendstone_1_1Block.md) & block) <br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getBlock**](classendstone_1_1BlockEvent.md#function-getblock) () const<br>_Gets the block involved in this event._  |
|   | [**~BlockEvent**](classendstone_1_1BlockEvent.md#function-blockevent) () override<br> |


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



### function isCancellable 


```C++
inline virtual bool endstone::BlockPlaceEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function ~BlockPlaceEvent 

```C++
endstone::BlockPlaceEvent::~BlockPlaceEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_place_event.h`

