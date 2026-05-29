

# Class endstone::ThunderChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ThunderChangeEvent**](classendstone_1_1ThunderChangeEvent.md)



_Called when the thunder state in a world is changing._ 

* `#include <endstone/event/weather/thunder_change_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"ThunderChangeEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ThunderChangeEvent**](#function-thunderchangeevent) ([**Level**](classendstone_1_1Level.md) & level, bool to) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  bool | [**toThunderState**](#function-tothunderstate) () const<br> |
|   | [**~ThunderChangeEvent**](#function-thunderchangeevent) () override<br> |


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
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::ThunderChangeEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ThunderChangeEvent 

```C++
inline endstone::ThunderChangeEvent::ThunderChangeEvent (
    Level & level,
    bool to
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::ThunderChangeEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


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

