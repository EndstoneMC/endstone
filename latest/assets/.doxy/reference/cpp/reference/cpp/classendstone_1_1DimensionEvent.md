

# Class endstone::DimensionEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**DimensionEvent**](classendstone_1_1DimensionEvent.md)



_Represents events within a dimension._ 

* `#include <endstone/event/level/dimension_event.h>`



Inherits the following classes: [endstone::LevelEvent](classendstone_1_1LevelEvent.md)


Inherited by the following classes: [endstone::ChunkEvent](classendstone_1_1ChunkEvent.md),  [endstone::DimensionLoadEvent](classendstone_1_1DimensionLoadEvent.md)








































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DimensionEvent**](#function-dimensionevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; & dimension) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; & | [**getDimension**](#function-getdimension) () const<br>_Gets the dimension primarily involved with this event._  |


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




### function DimensionEvent 

```C++
inline explicit endstone::DimensionEvent::DimensionEvent (
    const  NotNull < Dimension > & dimension
) 
```




<hr>



### function getDimension 

_Gets the dimension primarily involved with this event._ 
```C++
inline const  NotNull < Dimension > & endstone::DimensionEvent::getDimension () const
```





**Returns:**

[**Dimension**](classendstone_1_1Dimension.md) which caused this event 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/level/dimension_event.h`

