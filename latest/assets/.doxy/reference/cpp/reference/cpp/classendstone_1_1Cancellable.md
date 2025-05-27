

# Class endstone::Cancellable

**template &lt;typename EventType&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Cancellable**](classendstone_1_1Cancellable.md)



_A type characterizing events that may be cancelled by a plugin or the server._ 

* `#include <endstone/event/cancellable.h>`



Inherits the following classes: EventType,  [endstone::ICancellable](classendstone_1_1ICancellable.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual bool | [**isCancelled**](#function-iscancelled) () const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual void | [**setCancelled**](#function-setcancelled) (bool cancel) <br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual bool | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual void | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) (bool cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |






















































## Public Functions Documentation




### function cancel 

_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._ 
```C++
inline virtual void endstone::Cancellable::cancel () 
```



Implements [*endstone::ICancellable::cancel*](classendstone_1_1ICancellable.md#function-cancel)


<hr>



### function isCancelled 

_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._ 
```C++
inline virtual bool endstone::Cancellable::isCancelled () const
```





**Returns:**

true if this event is cancelled 





        
Implements [*endstone::ICancellable::isCancelled*](classendstone_1_1ICancellable.md#function-iscancelled)


<hr>



### function setCancelled 

_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._ 
```C++
inline virtual void endstone::Cancellable::setCancelled (
    bool cancel
) 
```





**Parameters:**


* `cancel` true if you wish to cancel this event 




        
Implements [*endstone::ICancellable::setCancelled*](classendstone_1_1ICancellable.md#function-setcancelled)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/cancellable.h`

