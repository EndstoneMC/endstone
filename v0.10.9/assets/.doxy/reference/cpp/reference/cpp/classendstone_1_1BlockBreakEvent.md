

# Class endstone::BlockBreakEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockBreakEvent**](classendstone_1_1BlockBreakEvent.md)



_Called when a block is broken by a player._ [More...](#detailed-description)

* `#include <endstone/event/block/block_break_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"BlockBreakEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockBreakEvent**](#function-blockbreakevent) (std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block, [**Player**](classendstone_1_1Player.md) & player) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](#function-getplayer) () const<br>_Gets the_ [_**Player**_](classendstone_1_1Player.md) _that is breaking the block involved in this event._ |
|   | [**~BlockBreakEvent**](#function-blockbreakevent) () override<br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual bool | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual void | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) (bool cancel) override<br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual bool | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual void | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) (bool cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Detailed Description


If a [**BlockBreakEvent**](classendstone_1_1BlockBreakEvent.md) is cancelled, the block will not break and experience will not drop. 


    
## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::BlockBreakEvent::NAME;
```




<hr>
## Public Functions Documentation




### function BlockBreakEvent 

```C++
inline explicit endstone::BlockBreakEvent::BlockBreakEvent (
    std::unique_ptr< Block > block,
    Player & player
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::BlockBreakEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getPlayer 

_Gets the_ [_**Player**_](classendstone_1_1Player.md) _that is breaking the block involved in this event._
```C++
inline Player & endstone::BlockBreakEvent::getPlayer () const
```





**Returns:**

The [**Player**](classendstone_1_1Player.md) that is breaking the block involved in this event 





        

<hr>



### function ~BlockBreakEvent 

```C++
endstone::BlockBreakEvent::~BlockBreakEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_break_event.h`

