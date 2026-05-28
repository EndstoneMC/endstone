

# Class endstone::PluginDisableEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PluginDisableEvent**](classendstone_1_1PluginDisableEvent.md)



_Called when a plugin is disabled._ 

* `#include <endstone/event/server/plugin_disable_event.h>`



Inherits the following classes: [endstone::ServerEvent](classendstone_1_1ServerEvent.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PluginDisableEvent**](classendstone_1_1PluginDisableEvent.md)) <br> |
|   | [**PluginDisableEvent**](#function-plugindisableevent) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) <br> |
|  [**Plugin**](classendstone_1_1Plugin.md) & | [**getPlugin**](#function-getplugin) () const<br> |


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
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PluginDisableEvent::ENDSTONE_EVENT (
    PluginDisableEvent
) 
```




<hr>



### function PluginDisableEvent 

```C++
inline explicit endstone::PluginDisableEvent::PluginDisableEvent (
    Plugin & plugin
) 
```




<hr>



### function getPlugin 

```C++
inline Plugin & endstone::PluginDisableEvent::getPlugin () const
```



Gets the plugin involved in this event




**Returns:**

[**Plugin**](classendstone_1_1Plugin.md) for this event 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/plugin_disable_event.h`

