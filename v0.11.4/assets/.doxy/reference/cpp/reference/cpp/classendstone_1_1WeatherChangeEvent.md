

# Class endstone::WeatherChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**WeatherChangeEvent**](classendstone_1_1WeatherChangeEvent.md)



_Called when the weather (rain) state in a world is changing._ 

* `#include <endstone/event/weather/weather_change_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**WeatherChangeEvent**](classendstone_1_1WeatherChangeEvent.md)) <br> |
|   | [**WeatherChangeEvent**](#function-weatherchangeevent) ([**Level**](classendstone_1_1Level.md) & level, [**const**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) to) <br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**toWeatherState**](#function-toweatherstate) () const<br> |
|   | [**~WeatherChangeEvent**](#function-weatherchangeevent) () override<br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::WeatherChangeEvent::ENDSTONE_EVENT (
    WeatherChangeEvent
) 
```




<hr>



### function WeatherChangeEvent 

```C++
inline endstone::WeatherChangeEvent::WeatherChangeEvent (
    Level & level,
    const  bool to
) 
```




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

