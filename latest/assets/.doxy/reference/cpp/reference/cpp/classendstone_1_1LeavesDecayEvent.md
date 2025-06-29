

# Class endstone::LeavesDecayEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**LeavesDecayEvent**](classendstone_1_1LeavesDecayEvent.md)



_Called when leaves are decaying naturally._ [More...](#detailed-description)

* `#include <endstone/event/block/leaves_decay_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**LeavesDecayEvent**](classendstone_1_1LeavesDecayEvent.md)) <br> |
|   | [**LeavesDecayEvent**](#function-leavesdecayevent) ([**Block**](classendstone_1_1Block.md) & block) <br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual bool | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual void | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) (bool cancel) <br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual bool | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual void | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) (bool cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Detailed Description




**Note:**

If a Leaves Decay event is cancelled, the leaves will not decay. 





    
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::LeavesDecayEvent::ENDSTONE_EVENT (
    LeavesDecayEvent
) 
```




<hr>



### function LeavesDecayEvent 

```C++
inline explicit endstone::LeavesDecayEvent::LeavesDecayEvent (
    Block & block
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/leaves_decay_event.h`

