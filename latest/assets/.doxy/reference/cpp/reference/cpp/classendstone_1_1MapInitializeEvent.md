

# Class endstone::MapInitializeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**MapInitializeEvent**](classendstone_1_1MapInitializeEvent.md)



_Called when a map is initialized._ 

* `#include <endstone/event/server/map_initialize_event.h>`



Inherits the following classes: [endstone::ServerEvent](classendstone_1_1ServerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MapInitializeEvent**](#function-mapinitializeevent) ([**MapView**](classendstone_1_1MapView.md) & map) <br> |
|  [**MapView**](classendstone_1_1MapView.md) & | [**getMap**](#function-getmap) () const<br>_Gets the map initialized in this event._  |


## Public Functions inherited from endstone::ServerEvent

See [endstone::ServerEvent](classendstone_1_1ServerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-12) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-22) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Functions Documentation




### function MapInitializeEvent 

```C++
inline explicit endstone::MapInitializeEvent::MapInitializeEvent (
    MapView & map
) 
```




<hr>



### function getMap 

_Gets the map initialized in this event._ 
```C++
inline MapView & endstone::MapInitializeEvent::getMap () const
```





**Returns:**

Map for this event 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/map_initialize_event.h`

