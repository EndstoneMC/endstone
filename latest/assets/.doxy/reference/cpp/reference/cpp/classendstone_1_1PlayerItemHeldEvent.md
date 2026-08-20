

# Class endstone::PlayerItemHeldEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerItemHeldEvent**](classendstone_1_1PlayerItemHeldEvent.md)



_Called when a player changes their currently held item._ 

* `#include <endstone/event/player/player_item_held_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerItemHeldEvent**](classendstone_1_1PlayerItemHeldEvent.md)) <br> |
|   | [**PlayerItemHeldEvent**](#function-playeritemheldevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) previous, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) current) <br> |
|  [**int**](classendstone_1_1Identifier.md) | [**getNewSlot**](#function-getnewslot) () const<br>_Gets the new held slot index._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getPreviousSlot**](#function-getpreviousslot) () const<br>_Gets the previous held slot index._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br>_Cancels this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br>_Sets the cancellation state of this event._  |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerItemHeldEvent::ENDSTONE_EVENT (
    PlayerItemHeldEvent
) 
```




<hr>



### function PlayerItemHeldEvent 

```C++
inline explicit endstone::PlayerItemHeldEvent::PlayerItemHeldEvent (
    const  NotNull < Player > & player,
    const  int previous,
    const  int current
) 
```




<hr>



### function getNewSlot 

_Gets the new held slot index._ 
```C++
inline int endstone::PlayerItemHeldEvent::getNewSlot () const
```





**Returns:**

New slot index 





        

<hr>



### function getPreviousSlot 

_Gets the previous held slot index._ 
```C++
inline int endstone::PlayerItemHeldEvent::getPreviousSlot () const
```





**Returns:**

Previous slot index 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_item_held_event.h`

