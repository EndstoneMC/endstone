

# Class endstone::ServerLoadEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServerLoadEvent**](classendstone_1_1ServerLoadEvent.md)



_Called when either the server startup or reload has completed._ 

* `#include <endstone/event/server/server_load_event.h>`



Inherits the following classes: [endstone::ServerEvent](classendstone_1_1ServerEvent.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**LoadType**](#enum-loadtype)  <br>_Represents the context in which the server was loaded._  |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ServerLoadEvent**](classendstone_1_1ServerLoadEvent.md)) <br> |
|   | [**ServerLoadEvent**](#function-serverloadevent) ([**LoadType**](classendstone_1_1ServerLoadEvent.md#enum-loadtype) type) <br> |
|  [**LoadType**](classendstone_1_1ServerLoadEvent.md#enum-loadtype) | [**getType**](#function-gettype) () const<br>_Gets the context in which the server was loaded._  |


## Public Functions inherited from endstone::ServerEvent

See [endstone::ServerEvent](classendstone_1_1ServerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |


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
















































































## Public Types Documentation




### enum LoadType 

_Represents the context in which the server was loaded._ 
```C++
enum endstone::ServerLoadEvent::LoadType {
    Startup,
    Reload
};
```




<hr>
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::ServerLoadEvent::ENDSTONE_EVENT (
    ServerLoadEvent
) 
```




<hr>



### function ServerLoadEvent 

```C++
inline explicit endstone::ServerLoadEvent::ServerLoadEvent (
    LoadType type
) 
```




<hr>



### function getType 

_Gets the context in which the server was loaded._ 
```C++
inline LoadType endstone::ServerLoadEvent::getType () const
```





**Returns:**

the load type for this event 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/server_load_event.h`

