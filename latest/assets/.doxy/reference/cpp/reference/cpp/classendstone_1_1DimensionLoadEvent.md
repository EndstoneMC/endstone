

# Class endstone::DimensionLoadEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**DimensionLoadEvent**](classendstone_1_1DimensionLoadEvent.md)



_Called when a dimension is loaded._ 

* `#include <endstone/event/level/dimension_load_event.h>`



Inherits the following classes: [endstone::DimensionEvent](classendstone_1_1DimensionEvent.md)






























































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DimensionLoadEvent**](#function-dimensionloadevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; & dimension) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**DimensionLoadEvent**](classendstone_1_1DimensionLoadEvent.md)) <br> |
|   | [**~DimensionLoadEvent**](#function-dimensionloadevent) () override<br> |


## Public Functions inherited from endstone::DimensionEvent

See [endstone::DimensionEvent](classendstone_1_1DimensionEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**DimensionEvent**](classendstone_1_1DimensionEvent.md#function-dimensionevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; & dimension) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; & | [**getDimension**](classendstone_1_1DimensionEvent.md#function-getdimension) () const<br>_Gets the dimension primarily involved with this event._  |


## Public Functions inherited from endstone::LevelEvent

See [endstone::LevelEvent](classendstone_1_1LevelEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**LevelEvent**](classendstone_1_1LevelEvent.md#function-levelevent) ([**Level**](classendstone_1_1Level.md) & level) <br> |
|  [**Level**](classendstone_1_1Level.md) & | [**getLevel**](classendstone_1_1LevelEvent.md#function-getlevel) () const<br>_Gets the level primarily involved with this event._  |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br>_Gets a user-friendly identifier for this event._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br>_Any custom event that should not by synchronized with other events must use the specific constructor._  |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |










































































































## Public Functions Documentation




### function DimensionLoadEvent 

```C++
inline explicit endstone::DimensionLoadEvent::DimensionLoadEvent (
    const  NotNull < Dimension > & dimension
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::DimensionLoadEvent::ENDSTONE_EVENT (
    DimensionLoadEvent
) 
```




<hr>



### function ~DimensionLoadEvent 

```C++
endstone::DimensionLoadEvent::~DimensionLoadEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/level/dimension_load_event.h`

