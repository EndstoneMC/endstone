

# Class endstone::BlockCookEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockCookEvent**](classendstone_1_1BlockCookEvent.md)



_Called when an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _is successfully cooked in a block._

* `#include <endstone/event/block/block_cook_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockCookEvent**](#function-blockcookevent) (std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block, [**ItemStack**](classendstone_1_1ItemStack.md) source, [**ItemStack**](classendstone_1_1ItemStack.md) result) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**BlockCookEvent**](classendstone_1_1BlockCookEvent.md)) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getResult**](#function-getresult) () const<br>_Gets the resultant_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._ |
|  [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getSource**](#function-getsource) () const<br>_Gets the smelted_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._ |
|  [**void**](classendstone_1_1Identifier.md) | [**setResult**](#function-setresult) ([**ItemStack**](classendstone_1_1ItemStack.md) result) <br>_Sets the resultant_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._ |


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
















































































## Public Functions Documentation




### function BlockCookEvent 

```C++
inline endstone::BlockCookEvent::BlockCookEvent (
    std::unique_ptr< Block > block,
    ItemStack source,
    ItemStack result
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::BlockCookEvent::ENDSTONE_EVENT (
    BlockCookEvent
) 
```




<hr>



### function getResult 

_Gets the resultant_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._
```C++
inline const  ItemStack & endstone::BlockCookEvent::getResult () const
```





**Returns:**

smelting result [**ItemStack**](classendstone_1_1ItemStack.md) 





        

<hr>



### function getSource 

_Gets the smelted_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._
```C++
inline const  ItemStack & endstone::BlockCookEvent::getSource () const
```





**Returns:**

smelting source [**ItemStack**](classendstone_1_1ItemStack.md) 





        

<hr>



### function setResult 

_Sets the resultant_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._
```C++
inline void endstone::BlockCookEvent::setResult (
    ItemStack result
) 
```





**Parameters:**


* `result` new result [**ItemStack**](classendstone_1_1ItemStack.md) 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_cook_event.h`

