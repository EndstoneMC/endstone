

# Class endstone::Event



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Event**](classendstone_1_1Event.md)



_Represents an event._ 

* `#include <endstone/event/event.h>`





Inherited by the following classes: [endstone::ActorEvent](classendstone_1_1ActorEvent.md),  [endstone::BlockEvent](classendstone_1_1BlockEvent.md),  [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md),  [endstone::ServerEvent](classendstone_1_1ServerEvent.md),  [endstone::WeatherEvent](classendstone_1_1WeatherEvent.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Event**](#function-event-12) (bool async=false) <br> |
|   | [**Event**](#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](#function-isasynchronous) () const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () const = 0<br> |
|  bool | [**isCancelled**](#function-iscancelled) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  void | [**setCancelled**](#function-setcancelled) (bool cancel) <br> |
| virtual  | [**~Event**](#function-event) () = default<br> |




























## Public Functions Documentation




### function Event [1/2]

```C++
inline explicit endstone::Event::Event (
    bool async=false
) 
```




<hr>



### function Event [2/2]

```C++
endstone::Event::Event (
    const Event &
) = delete
```




<hr>



### function getEventName 


```C++
virtual std::string endstone::Event::getEventName () const = 0
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        

<hr>



### function isAsynchronous 


```C++
inline bool endstone::Event::isAsynchronous () const
```



Any custom event that should not by synchronized with other events must use the specific constructor.




**Returns:**

false by default, true if the event fires asynchronously 





        

<hr>



### function isCancellable 


```C++
virtual bool endstone::Event::isCancellable () const = 0
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        

<hr>



### function isCancelled 


```C++
inline bool endstone::Event::isCancelled () const
```



Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins




**Returns:**

true if this event is cancelled 





        

<hr>



### function operator= 

```C++
Event & endstone::Event::operator= (
    const Event &
) = delete
```




<hr>



### function setCancelled 


```C++
inline void endstone::Event::setCancelled (
    bool cancel
) 
```



Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins.




**Parameters:**


* `cancel` true if you wish to cancel this event 




        

<hr>



### function ~Event 

```C++
virtual endstone::Event::~Event () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/event.h`

