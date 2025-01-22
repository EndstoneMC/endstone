

# Class endstone::WeatherChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**WeatherChangeEvent**](classendstone_1_1WeatherChangeEvent.md)



_Called when the weather (rain) state in a world is changing._ 

* `#include <endstone/event/weather/weather_change_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**NAME**](#variable-name)   = `"WeatherChangeEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**WeatherChangeEvent**](#function-weatherchangeevent) ([**Level**](classendstone_1_1Level.md) & level, [**const**](classendstone_1_1Vector.md) [**bool**](classendstone_1_1Vector.md) to) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**bool**](classendstone_1_1Vector.md) | [**toWeatherState**](#function-toweatherstate) () const<br> |
|   | [**~WeatherChangeEvent**](#function-weatherchangeevent) () override<br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Vector.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Vector.md) cancel) <br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Vector.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Vector.md) cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































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
    const  bool to
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

