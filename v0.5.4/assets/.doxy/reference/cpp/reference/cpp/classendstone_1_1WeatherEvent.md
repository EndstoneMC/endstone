

# Class endstone::WeatherEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**WeatherEvent**](classendstone_1_1WeatherEvent.md)



_Represents a Weather-related event._ 

* `#include <endstone/event/weather/weather_event.h>`



Inherits the following classes: [endstone::Event](classendstone_1_1Event.md)


Inherited by the following classes: [endstone::ThunderChangeEvent](classendstone_1_1ThunderChangeEvent.md),  [endstone::WeatherChangeEvent](classendstone_1_1WeatherChangeEvent.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**WeatherEvent**](#function-weatherevent) ([**Level**](classendstone_1_1Level.md) & level) <br> |
|  [**Level**](classendstone_1_1Level.md) & | [**getLevel**](#function-getlevel) () const<br> |
|   | [**~WeatherEvent**](#function-weatherevent) () override<br> |


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






















































## Public Functions Documentation




### function WeatherEvent 

```C++
inline explicit endstone::WeatherEvent::WeatherEvent (
    Level & level
) 
```




<hr>



### function getLevel 


```C++
inline Level & endstone::WeatherEvent::getLevel () const
```



Returns the [**Level**](classendstone_1_1Level.md) where this event is occurring




**Returns:**

[**Level**](classendstone_1_1Level.md) this event is occurring in 





        

<hr>



### function ~WeatherEvent 

```C++
endstone::WeatherEvent::~WeatherEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/weather/weather_event.h`

