

# Class endstone::BlockBreakEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockBreakEvent**](classendstone_1_1BlockBreakEvent.md)



_Called when a block is broken by a player._ [More...](#detailed-description)

* `#include <endstone/event/block/block_break_event.h>`



Inherits the following classes: [endstone::BlockEvent](classendstone_1_1BlockEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "BlockBreakEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockBreakEvent**](#function-blockbreakevent) ([**Block**](classendstone_1_1Block.md) & block, [**Player**](classendstone_1_1Player.md) & player) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](#function-getplayer) () const<br>_Gets the_ [_**Player**_](classendstone_1_1Player.md) _that is breaking the block involved in this event._ |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|   | [**~BlockBreakEvent**](#function-blockbreakevent) () override<br> |


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
    Block & block,
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



### function isCancellable 


```C++
inline virtual bool endstone::BlockBreakEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function ~BlockBreakEvent 

```C++
endstone::BlockBreakEvent::~BlockBreakEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_break_event.h`

