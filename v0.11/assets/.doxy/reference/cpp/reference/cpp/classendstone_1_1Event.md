

# Class endstone::Event



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Event**](classendstone_1_1Event.md)



_Represents an event._ 

* `#include <endstone/event/event.h>`





Inherited by the following classes: [endstone::ActorEvent](classendstone_1_1ActorEvent.md),  [endstone::ActorEvent](classendstone_1_1ActorEvent.md),  [endstone::ActorEvent](classendstone_1_1ActorEvent.md),  [endstone::BlockEvent](classendstone_1_1BlockEvent.md),  [endstone::LevelEvent](classendstone_1_1LevelEvent.md),  [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md),  [endstone::ServerEvent](classendstone_1_1ServerEvent.md),  [endstone::WeatherEvent](classendstone_1_1WeatherEvent.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Event**](#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](#function-event) () = default<br> |




























## Public Functions Documentation




### function Event [1/3]

```C++
inline explicit endstone::Event::Event (
    bool async=false
) 
```




<hr>



### function Event [2/3]

```C++
endstone::Event::Event (
    const  Event &
) = delete
```




<hr>



### function Event [3/3]

```C++
endstone::Event::Event (
    Event &&
) = default
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



### function operator= 

```C++
Event & endstone::Event::operator= (
    const  Event &
) = delete
```




<hr>



### function operator= 

```C++
Event & endstone::Event::operator= (
    Event &&
) = default
```




<hr>



### function ~Event 

```C++
virtual endstone::Event::~Event () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/event.h`

