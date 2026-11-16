

# Class endstone::PluginEnableEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PluginEnableEvent**](classendstone_1_1PluginEnableEvent.md)



_Called when a plugin is enabled._ 

* `#include <endstone/event/server/plugin_enable_event.h>`



Inherits the following classes: [endstone::ServerEvent](classendstone_1_1ServerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) (PluginEnableEvent) <br> |
|   | [**PluginEnableEvent**](#function-pluginenableevent) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) <br> |
|  [**Plugin**](classendstone_1_1Plugin.md) & | [**getPlugin**](#function-getplugin) () const<br> |


## Public Functions inherited from endstone::ServerEvent

See [endstone::ServerEvent](classendstone_1_1ServerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-13) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-23) (const Event &) = delete<br> |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-33) (Event &&) = default<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-13) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) (const Event &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) (Event &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  Event & | [**operator=**](classendstone_1_1Event.md#function-operator) (const Event &) = delete<br> |
|  Event & | [**operator=**](classendstone_1_1Event.md#function-operator_1) (Event &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PluginEnableEvent::ENDSTONE_EVENT (
    PluginEnableEvent
) 
```




<hr>



### function PluginEnableEvent 

```C++
inline explicit endstone::PluginEnableEvent::PluginEnableEvent (
    Plugin & plugin
) 
```




<hr>



### function getPlugin 

```C++
inline Plugin & endstone::PluginEnableEvent::getPlugin () const
```



Gets the plugin involved in this event




**Returns:**

[**Plugin**](classendstone_1_1Plugin.md) for this event 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/plugin_enable_event.h`

