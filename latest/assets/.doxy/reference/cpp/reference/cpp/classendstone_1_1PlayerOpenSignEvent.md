

# Class endstone::PlayerOpenSignEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerOpenSignEvent**](classendstone_1_1PlayerOpenSignEvent.md)



_Called when a player begins editing a sign's text._ [More...](#detailed-description)

* `#include <endstone/event/player/player_open_sign_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Cause**](#enum-cause)  <br>_Represents the cause of the sign opening._  |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerOpenSignEvent**](classendstone_1_1PlayerOpenSignEvent.md)) <br> |
|   | [**PlayerOpenSignEvent**](#function-playeropensignevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Sign**](classendstone_1_1Sign.md) &gt; & sign, [**Sign::Side**](classendstone_1_1Sign.md#enum-side) side, [**Cause**](classendstone_1_1PlayerOpenSignEvent.md#enum-cause) cause) <br> |
|  [**Cause**](classendstone_1_1PlayerOpenSignEvent.md#enum-cause) | [**getCause**](#function-getcause) () const<br>_Gets the cause of the sign opening._  |
|  [**Sign::Side**](classendstone_1_1Sign.md#enum-side) | [**getSide**](#function-getside) () const<br>_Gets the side of the sign being opened._  |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Sign**](classendstone_1_1Sign.md) &gt; & | [**getSign**](#function-getsign) () const<br>_Gets a captured state of the sign involved in this event._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br>_Cancels this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br>_Sets the cancellation state of this event._  |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Detailed Description


Cancelling this event stops the sign editing menu from opening. 


    
## Public Types Documentation




### enum Cause 

_Represents the cause of the sign opening._ 
```C++
enum endstone::PlayerOpenSignEvent::Cause {
    Place,
    Interact,
    Plugin,
    Unknown
};
```




<hr>
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerOpenSignEvent::ENDSTONE_EVENT (
    PlayerOpenSignEvent
) 
```




<hr>



### function PlayerOpenSignEvent 

```C++
inline endstone::PlayerOpenSignEvent::PlayerOpenSignEvent (
    const  NotNull < Player > & player,
    const  NotNull < Sign > & sign,
    Sign::Side side,
    Cause cause
) 
```




<hr>



### function getCause 

_Gets the cause of the sign opening._ 
```C++
inline Cause endstone::PlayerOpenSignEvent::getCause () const
```





**Returns:**

the sign opening cause 





        

<hr>



### function getSide 

_Gets the side of the sign being opened._ 
```C++
inline Sign::Side endstone::PlayerOpenSignEvent::getSide () const
```





**Returns:**

the sign side being opened 





        

<hr>



### function getSign 

_Gets a captured state of the sign involved in this event._ 
```C++
inline const  NotNull < Sign > & endstone::PlayerOpenSignEvent::getSign () const
```





**Returns:**

the sign being opened 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_open_sign_event.h`

