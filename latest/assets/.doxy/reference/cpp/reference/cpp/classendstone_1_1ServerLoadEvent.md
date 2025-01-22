

# Class endstone::ServerLoadEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServerLoadEvent**](classendstone_1_1ServerLoadEvent.md)



_Called when either the server startup or reload has completed._ 

* `#include <endstone/event/server/server_load_event.h>`



Inherits the following classes: [endstone::ServerEvent](classendstone_1_1ServerEvent.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**LoadType**](#enum-loadtype)  <br> |


















## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**NAME**](#variable-name)   = `"ServerLoadEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ServerLoadEvent**](#function-serverloadevent) (LoadType type) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  LoadType | [**getType**](#function-gettype) () const<br> |


## Public Functions inherited from endstone::ServerEvent

See [endstone::ServerEvent](classendstone_1_1ServerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-12) ([**bool**](classendstone_1_1Vector.md) async=[**false**](classendstone_1_1Vector.md)) <br> |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-22) ([**const**](classendstone_1_1Vector.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) ([**bool**](classendstone_1_1Vector.md) async=[**false**](classendstone_1_1Vector.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) ([**const**](classendstone_1_1Vector.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Vector.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Vector.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Types Documentation




### enum LoadType 

```C++
enum endstone::ServerLoadEvent::LoadType {
    Startup,
    Reload
};
```




<hr>
## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::ServerLoadEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ServerLoadEvent 

```C++
inline explicit endstone::ServerLoadEvent::ServerLoadEvent (
    LoadType type
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::ServerLoadEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getType 

```C++
inline LoadType endstone::ServerLoadEvent::getType () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/server_load_event.h`

