

# Class endstone::EventHandler



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**EventHandler**](classendstone_1_1EventHandler.md)



_Represents a registered_ [_**EventHandler**_](classendstone_1_1EventHandler.md) _which associates with a_[_**Plugin**_](classendstone_1_1Plugin.md) _._

* `#include <endstone/event/event_handler.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EventHandler**](#function-eventhandler) (std::string event, std::function&lt; [**void**](classendstone_1_1Identifier.md)([**Event**](classendstone_1_1Event.md) &)&gt; executor, [**EventPriority**](namespaceendstone.md#enum-eventpriority) priority, [**Plugin**](classendstone_1_1Plugin.md) & plugin, [**bool**](classendstone_1_1Identifier.md) ignore\_cancelled) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**callEvent**](#function-callevent) ([**Event**](classendstone_1_1Event.md) & event) const<br>_Calls the event executor._  |
|  std::string | [**getEventType**](#function-geteventtype) () const<br>_Gets the event type for this registration._  |
|  [**Plugin**](classendstone_1_1Plugin.md) & | [**getPlugin**](#function-getplugin) () const<br>_Gets the plugin for this registration._  |
|  [**EventPriority**](namespaceendstone.md#enum-eventpriority) | [**getPriority**](#function-getpriority) () const<br>_Gets the priority for this registration._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isIgnoreCancelled**](#function-isignorecancelled) () const<br>_Whether this listener accepts cancelled events._  |




























## Public Functions Documentation




### function EventHandler 

```C++
inline endstone::EventHandler::EventHandler (
    std::string event,
    std::function< void ( Event &)> executor,
    EventPriority priority,
    Plugin & plugin,
    bool ignore_cancelled
) 
```




<hr>



### function callEvent 

_Calls the event executor._ 
```C++
inline void endstone::EventHandler::callEvent (
    Event & event
) const
```





**Parameters:**


* `event` The event 




        

<hr>



### function getEventType 

_Gets the event type for this registration._ 
```C++
inline std::string endstone::EventHandler::getEventType () const
```





**Returns:**

Registered event type 





        

<hr>



### function getPlugin 

_Gets the plugin for this registration._ 
```C++
inline Plugin & endstone::EventHandler::getPlugin () const
```





**Returns:**

Registered [**Plugin**](classendstone_1_1Plugin.md) 





        

<hr>



### function getPriority 

_Gets the priority for this registration._ 
```C++
inline EventPriority endstone::EventHandler::getPriority () const
```





**Returns:**

Registered Priority 





        

<hr>



### function isIgnoreCancelled 

_Whether this listener accepts cancelled events._ 
```C++
inline bool endstone::EventHandler::isIgnoreCancelled () const
```





**Returns:**

True when ignoring cancelled events 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/event_handler.h`

