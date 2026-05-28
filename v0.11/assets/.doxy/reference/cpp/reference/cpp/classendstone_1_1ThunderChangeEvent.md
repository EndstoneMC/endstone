

# Class endstone::ThunderChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ThunderChangeEvent**](classendstone_1_1ThunderChangeEvent.md)



_Called when the thunder state in a world is changing._ 

* `#include <endstone/event/weather/thunder_change_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ThunderChangeEvent**](classendstone_1_1ThunderChangeEvent.md)) <br> |
|   | [**ThunderChangeEvent**](#function-thunderchangeevent) ([**Level**](classendstone_1_1Level.md) & level, [**bool**](classendstone_1_1Identifier.md) to) <br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**toThunderState**](#function-tothunderstate) () const<br> |
|   | [**~ThunderChangeEvent**](#function-thunderchangeevent) () override<br> |


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




### function ENDSTONE\_EVENT 

```C++
endstone::ThunderChangeEvent::ENDSTONE_EVENT (
    ThunderChangeEvent
) 
```




<hr>



### function ThunderChangeEvent 

```C++
inline endstone::ThunderChangeEvent::ThunderChangeEvent (
    Level & level,
    bool to
) 
```




<hr>



### function toThunderState 

```C++
inline bool endstone::ThunderChangeEvent::toThunderState () const
```



Gets the state of thunder that the world is being set to




**Returns:**

true if the weather is being set to thundering, false otherwise 





        

<hr>



### function ~ThunderChangeEvent 

```C++
endstone::ThunderChangeEvent::~ThunderChangeEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/weather/thunder_change_event.h`

