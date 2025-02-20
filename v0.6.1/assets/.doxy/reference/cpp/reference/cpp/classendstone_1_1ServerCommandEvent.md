

# Class endstone::ServerCommandEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServerCommandEvent**](classendstone_1_1ServerCommandEvent.md)



_Called when the console runs a command, early in the process._ [More...](#detailed-description)

* `#include <endstone/event/server/server_command_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**NAME**](#variable-name)   = `"ServerCommandEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ServerCommandEvent**](#function-servercommandevent) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, std::string command) <br> |
|  std::string | [**getCommand**](#function-getcommand) () const<br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**CommandSender**](classendstone_1_1CommandSender.md) & | [**getSender**](#function-getsender) () const<br> |
|  [**void**](classendstone_1_1Vector.md) | [**setCommand**](#function-setcommand) (std::string message) <br> |


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
















































































## Detailed Description


You should not use this except for a few cases like logging commands, blocking commands on certain places, or applying modifiers.


**The command message contains a slash '/' at the start** 


    
## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::ServerCommandEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ServerCommandEvent 

```C++
inline endstone::ServerCommandEvent::ServerCommandEvent (
    CommandSender & sender,
    std::string command
) 
```




<hr>



### function getCommand 

```C++
inline std::string endstone::ServerCommandEvent::getCommand () const
```



Gets the command that the server is attempting to execute from the console




**Returns:**

[**Command**](classendstone_1_1Command.md) the server is attempting to execute 





        

<hr>



### function getEventName 

```C++
inline virtual std::string endstone::ServerCommandEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getSender 

```C++
inline CommandSender & endstone::ServerCommandEvent::getSender () const
```



Get the command sender.




**Returns:**

The sender 





        

<hr>



### function setCommand 

```C++
inline void endstone::ServerCommandEvent::setCommand (
    std::string message
) 
```



Sets the command that the server will execute




**Parameters:**


* `message` New message that the server will execute 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/server_command_event.h`

