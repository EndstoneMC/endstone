

# Class endstone::BlockPistonEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockPistonEvent**](classendstone_1_1BlockPistonEvent.md)



_Called when a piston block is triggered._ 

* `#include <endstone/event/block/block_piston_event.h>`



Inherits the following classes: [endstone::BlockEvent](classendstone_1_1BlockEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"BlockPistonEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockPistonEvent**](#function-blockpistonevent) ([**Block**](classendstone_1_1Block.md) & block, [**Player**](classendstone_1_1Player.md) & player) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|   | [**~BlockPistonEvent**](#function-blockpistonevent) () override<br> |


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
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::BlockPistonEvent::NAME;
```




<hr>
## Public Functions Documentation




### function BlockPistonEvent 

```C++
inline explicit endstone::BlockPistonEvent::BlockPistonEvent (
    Block & block,
    Player & player
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::BlockPistonEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function ~BlockPistonEvent 

```C++
endstone::BlockPistonEvent::~BlockPistonEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_piston_event.h`

