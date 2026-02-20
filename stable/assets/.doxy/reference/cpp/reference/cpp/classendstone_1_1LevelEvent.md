

# Class endstone::LevelEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**LevelEvent**](classendstone_1_1LevelEvent.md)



_Represents events within a level._ 

* `#include <endstone/event/level/level_event.h>`



Inherits the following classes: [endstone::Event](classendstone_1_1Event.md)


Inherited by the following classes: [endstone::DimensionEvent](classendstone_1_1DimensionEvent.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LevelEvent**](#function-levelevent) ([**Level**](classendstone_1_1Level.md) & level) <br> |
|  [**Level**](classendstone_1_1Level.md) & | [**getLevel**](#function-getlevel) () const<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















































## Public Functions Documentation




### function LevelEvent 

```C++
inline explicit endstone::LevelEvent::LevelEvent (
    Level & level
) 
```




<hr>



### function getLevel 

```C++
inline Level & endstone::LevelEvent::getLevel () const
```



Gets the level primarily involved with this event




**Returns:**

[**Level**](classendstone_1_1Level.md) which caused this event 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/level/level_event.h`

