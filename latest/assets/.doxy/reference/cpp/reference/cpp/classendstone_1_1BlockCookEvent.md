

# Class endstone::BlockCookEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockCookEvent**](classendstone_1_1BlockCookEvent.md)



_Called when an_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _is successfully cooked in a block._

* `#include <endstone/event/block/block_cook_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BlockCookEvent**](#function-blockcookevent) (std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block, std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; source, std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; result) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**BlockCookEvent**](classendstone_1_1BlockCookEvent.md)) <br> |
|  const [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getResult**](#function-getresult) () const<br>_Gets the resultant_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._ |
|  const [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getSource**](#function-getsource) () const<br>_Gets the smelted_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._ |
|  void | [**setResult**](#function-setresult) (std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; result) <br>_Sets the resultant_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._ |


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
















































































## Public Functions Documentation




### function BlockCookEvent 

```C++
inline endstone::BlockCookEvent::BlockCookEvent (
    std::unique_ptr< Block > block,
    std::unique_ptr< ItemStack > source,
    std::unique_ptr< ItemStack > result
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
inline const ItemStack & endstone::BlockCookEvent::getResult () const
```





**Returns:**

smelting result [**ItemStack**](classendstone_1_1ItemStack.md) 





        

<hr>



### function getSource 

_Gets the smelted_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._
```C++
inline const ItemStack & endstone::BlockCookEvent::getSource () const
```





**Returns:**

smelting source [**ItemStack**](classendstone_1_1ItemStack.md) 





        

<hr>



### function setResult 

_Sets the resultant_ [_**ItemStack**_](classendstone_1_1ItemStack.md) _for this event._
```C++
inline void endstone::BlockCookEvent::setResult (
    std::unique_ptr< ItemStack > result
) 
```





**Parameters:**


* `result` new result [**ItemStack**](classendstone_1_1ItemStack.md) 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/block_cook_event.h`

