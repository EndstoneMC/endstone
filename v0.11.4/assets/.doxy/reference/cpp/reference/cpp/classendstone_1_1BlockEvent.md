

# Class endstone::BlockEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockEvent**](classendstone_1_1BlockEvent.md)



_Represents an Block-related event._ 

* `#include <endstone/event/block/block_event.h>`



Inherits the following classes: [endstone::Event](classendstone_1_1Event.md)


Inherited by the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockEvent**](#function-blockevent) (std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block) <br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getBlock**](#function-getblock) () const<br>_Gets the block involved in this event._  |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |














## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**block\_**](#variable-block_)  <br> |








































## Public Functions Documentation




### function BlockEvent 

```C++
inline explicit endstone::BlockEvent::BlockEvent (
    std::unique_ptr< Block > block
) 
```




<hr>



### function getBlock 

_Gets the block involved in this event._ 
```C++
inline Block & endstone::BlockEvent::getBlock () const
```





**Returns:**

The [**Block**](classendstone_1_1Block.md) which block is involved in this event 





        

<hr>
## Protected Attributes Documentation




### variable block\_ 

```C++
std::unique_ptr<Block> endstone::BlockEvent::block_;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_event.h`

