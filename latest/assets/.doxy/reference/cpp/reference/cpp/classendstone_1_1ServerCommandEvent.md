

# Class endstone::ServerCommandEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServerCommandEvent**](classendstone_1_1ServerCommandEvent.md)



_Called when a command is run by a non-player, early in the command handling process._ [More...](#detailed-description)

* `#include <endstone/event/server/server_command_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ServerCommandEvent**](classendstone_1_1ServerCommandEvent.md)) <br> |
|   | [**ServerCommandEvent**](#function-servercommandevent) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, std::string command) <br> |
|  std::string | [**getCommand**](#function-getcommand) () const<br>_Gets the command that the server is attempting to execute from the console._  |
|  [**CommandSender**](classendstone_1_1CommandSender.md) & | [**getSender**](#function-getsender) () const<br>_Get the command sender._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setCommand**](#function-setcommand) (std::string command) <br>_Sets the command that the server will execute._  |


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


You should not use this except for a few cases like logging commands, blocking commands on certain places, or applying modifiers.


**The command message contains a slash '/' at the start** 


    
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::ServerCommandEvent::ENDSTONE_EVENT (
    ServerCommandEvent
) 
```




<hr>



### function ServerCommandEvent 

```C++
inline endstone::ServerCommandEvent::ServerCommandEvent (
    CommandSender & sender,
    std::string command
) 
```




<hr>



### function getCommand 

_Gets the command that the server is attempting to execute from the console._ 
```C++
inline std::string endstone::ServerCommandEvent::getCommand () const
```





**Returns:**

[**Command**](classendstone_1_1Command.md) the server is attempting to execute 





        

<hr>



### function getSender 

_Get the command sender._ 
```C++
inline CommandSender & endstone::ServerCommandEvent::getSender () const
```





**Returns:**

The sender 





        

<hr>



### function setCommand 

_Sets the command that the server will execute._ 
```C++
inline void endstone::ServerCommandEvent::setCommand (
    std::string command
) 
```





**Parameters:**


* `command` New command that the server will execute. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/server_command_event.h`

