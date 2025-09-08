

# Class endstone::DimensionEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**DimensionEvent**](classendstone_1_1DimensionEvent.md)



_Represents events within a level._ 

* `#include <endstone/event/level/dimension_event.h>`



Inherits the following classes: [endstone::LevelEvent](classendstone_1_1LevelEvent.md)


Inherited by the following classes: [endstone::ChunkEvent](classendstone_1_1ChunkEvent.md)








































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DimensionEvent**](#function-dimensionevent) ([**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |
|  [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](#function-getdimension) () const<br> |


## Public Functions inherited from endstone::LevelEvent

See [endstone::LevelEvent](classendstone_1_1LevelEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**LevelEvent**](classendstone_1_1LevelEvent.md#function-levelevent) ([**Level**](classendstone_1_1Level.md) & level) <br> |
|  [**Level**](classendstone_1_1Level.md) & | [**getLevel**](classendstone_1_1LevelEvent.md#function-getlevel) () const<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Functions Documentation




### function DimensionEvent 

```C++
inline explicit endstone::DimensionEvent::DimensionEvent (
    Dimension & dimension
) 
```




<hr>



### function getDimension 

```C++
inline Dimension & endstone::DimensionEvent::getDimension () const
```



Gets the dimension primarily involved with this event




**Returns:**

[**Dimension**](classendstone_1_1Dimension.md) which caused this event 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/level/dimension_event.h`

