

# Class endstone::EventHandler



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**EventHandler**](classendstone_1_1EventHandler.md)



_Represents a registered_ [_**EventHandler**_](classendstone_1_1EventHandler.md) _which associates with a_[_**Plugin**_](classendstone_1_1Plugin.md) _._

* `#include <endstone/event/event_handler.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EventHandler**](#function-eventhandler) (std::string event, std::function&lt; void([**Event**](classendstone_1_1Event.md) &)&gt; executor, EventPriority priority, [**Plugin**](classendstone_1_1Plugin.md) & plugin, bool ignore\_cancelled) <br> |
|  void | [**callEvent**](#function-callevent) ([**Event**](classendstone_1_1Event.md) & event) <br> |
|  std::string | [**getEventType**](#function-geteventtype) () const<br> |
|  [**Plugin**](classendstone_1_1Plugin.md) & | [**getPlugin**](#function-getplugin) () const<br> |
|  EventPriority | [**getPriority**](#function-getpriority) () const<br> |
|  bool | [**isIgnoreCancelled**](#function-isignorecancelled) () const<br> |




























## Public Functions Documentation




### function EventHandler 

```C++
inline endstone::EventHandler::EventHandler (
    std::string event,
    std::function< void( Event &)> executor,
    EventPriority priority,
    Plugin & plugin,
    bool ignore_cancelled
) 
```




<hr>



### function callEvent 


```C++
inline void endstone::EventHandler::callEvent (
    Event & event
) 
```



Calls the event executor




**Parameters:**


* `event` The event 




        

<hr>



### function getEventType 


```C++
inline std::string endstone::EventHandler::getEventType () const
```



Gets the event type for this registration




**Returns:**

Registered event type 





        

<hr>



### function getPlugin 


```C++
inline Plugin & endstone::EventHandler::getPlugin () const
```



Gets the plugin for this registration




**Returns:**

Registered [**Plugin**](classendstone_1_1Plugin.md) 





        

<hr>



### function getPriority 


```C++
inline EventPriority endstone::EventHandler::getPriority () const
```



Gets the priority for this registration




**Returns:**

Registered Priority 





        

<hr>



### function isIgnoreCancelled 


```C++
inline bool endstone::EventHandler::isIgnoreCancelled () const
```



Whether this listener accepts cancelled events




**Returns:**

True when ignoring cancelled events 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/event_handler.h`

