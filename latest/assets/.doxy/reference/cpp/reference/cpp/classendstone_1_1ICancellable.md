

# Class endstone::ICancellable



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ICancellable**](classendstone_1_1ICancellable.md)



_Interface for events that may be cancelled by a plugin or the server._ 

* `#include <endstone/event/cancellable.h>`





Inherited by the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md),  [endstone::Cancellable](classendstone_1_1Cancellable.md),  [endstone::Cancellable](classendstone_1_1Cancellable.md),  [endstone::Cancellable](classendstone_1_1Cancellable.md),  [endstone::Cancellable](classendstone_1_1Cancellable.md),  [endstone::Cancellable](classendstone_1_1Cancellable.md),  [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](#function-cancel) () = 0<br>_Cancels this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](#function-iscancelled) () const = 0<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br>_Sets the cancellation state of this event._  |
| virtual  | [**~ICancellable**](#function-icancellable) () = default<br> |




























## Public Functions Documentation




### function cancel 

_Cancels this event._ 
```C++
virtual void endstone::ICancellable::cancel () = 0
```




<hr>



### function isCancelled 

_Gets the cancellation state of this event._ 
```C++
virtual bool endstone::ICancellable::isCancelled () const = 0
```





**Returns:**

true if this event is cancelled 





        

<hr>



### function setCancelled 

_Sets the cancellation state of this event._ 
```C++
virtual void endstone::ICancellable::setCancelled (
    bool cancel
) = 0
```





**Parameters:**


* `cancel` true if you wish to cancel this event 




        

<hr>



### function ~ICancellable 

```C++
virtual endstone::ICancellable::~ICancellable () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/cancellable.h`

