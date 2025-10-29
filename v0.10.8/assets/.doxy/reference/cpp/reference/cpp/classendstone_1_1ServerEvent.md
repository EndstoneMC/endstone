

# Class endstone::ServerEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServerEvent**](classendstone_1_1ServerEvent.md)



_Represents a Server-related event._ 

* `#include <endstone/event/server/server_event.h>`



Inherits the following classes: [endstone::Event](classendstone_1_1Event.md)


Inherited by the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md),  [endstone::PluginDisableEvent](classendstone_1_1PluginDisableEvent.md),  [endstone::PluginEnableEvent](classendstone_1_1PluginEnableEvent.md),  [endstone::ServerLoadEvent](classendstone_1_1ServerLoadEvent.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Event**](#function-event-12) (bool async=false) <br> |
|   | [**Event**](#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |


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




### function Event [1/2]

```C++
inline explicit endstone::ServerEvent::Event (
    bool async=false
) 
```




<hr>



### function Event [2/2]

```C++
endstone::ServerEvent::Event (
    const Event &
) = delete
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/server_event.h`

