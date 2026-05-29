

# Class endstone::WeatherChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**WeatherChangeEvent**](classendstone_1_1WeatherChangeEvent.md)



_Called when the weather (rain) state in a world is changing._ 

* `#include <endstone/event/weather/weather_change_event.h>`



Inherits the following classes: [endstone::WeatherEvent](classendstone_1_1WeatherEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "WeatherChangeEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**WeatherChangeEvent**](#function-weatherchangeevent) ([**Level**](classendstone_1_1Level.md) & level, bool to) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  bool | [**toWeatherState**](#function-toweatherstate) () const<br> |
|   | [**~WeatherChangeEvent**](#function-weatherchangeevent) () override<br> |


## Public Functions inherited from endstone::WeatherEvent

See [endstone::WeatherEvent](classendstone_1_1WeatherEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**WeatherEvent**](classendstone_1_1WeatherEvent.md#function-weatherevent) ([**Level**](classendstone_1_1Level.md) & level) <br> |
|  [**Level**](classendstone_1_1Level.md) & | [**getLevel**](classendstone_1_1WeatherEvent.md#function-getlevel) () const<br> |
|   | [**~WeatherEvent**](classendstone_1_1WeatherEvent.md#function-weatherevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
| virtual bool | [**isCancellable**](classendstone_1_1Event.md#function-iscancellable) () const = 0<br> |
|  bool | [**isCancelled**](classendstone_1_1Event.md#function-iscancelled) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  void | [**setCancelled**](classendstone_1_1Event.md#function-setcancelled) (bool cancel) <br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::WeatherChangeEvent::NAME;
```




<hr>
## Public Functions Documentation




### function WeatherChangeEvent 

```C++
inline endstone::WeatherChangeEvent::WeatherChangeEvent (
    Level & level,
    bool to
) 
```




<hr>



### function getEventName 


```C++
inline virtual std::string endstone::WeatherChangeEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function isCancellable 


```C++
inline virtual bool endstone::WeatherChangeEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function toWeatherState 


```C++
inline bool endstone::WeatherChangeEvent::toWeatherState () const
```



Gets the state of weather that the world is being set to




**Returns:**

true if the weather is being set to raining, false otherwise 





        

<hr>



### function ~WeatherChangeEvent 

```C++
endstone::WeatherChangeEvent::~WeatherChangeEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/weather/weather_change_event.h`

