

# Class endstone::UnknownCommandEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**UnknownCommandEvent**](classendstone_1_1UnknownCommandEvent.md)



_Called when a command sender executes a command that is not defined._ 

* `#include <endstone/event/command/unknown_command_event.h>`



Inherits the following classes: [endstone::Event](classendstone_1_1Event.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**UnknownCommandEvent**](classendstone_1_1UnknownCommandEvent.md)) <br> |
|   | [**UnknownCommandEvent**](#function-unknowncommandevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**CommandSender**](classendstone_1_1CommandSender.md) &gt; & sender, std::string command\_line, std::optional&lt; [**Message**](namespaceendstone.md#typedef-message) &gt; message) <br> |
|  std::string | [**getCommandLine**](#function-getcommandline) () const<br>_Gets the command that was sent._  |
|  std::optional&lt; [**Message**](namespaceendstone.md#typedef-message) &gt; | [**getMessage**](#function-getmessage) () const<br>_Gets the message that will be returned._  |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**CommandSender**](classendstone_1_1CommandSender.md) &gt; & | [**getSender**](#function-getsender) () const<br>_Gets the command sender._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setMessage**](#function-setmessage) (std::optional&lt; [**Message**](namespaceendstone.md#typedef-message) &gt; message) <br>_Sets the message that will be returned._  |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br>_Gets a user-friendly identifier for this event._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br>_Any custom event that should not by synchronized with other events must use the specific constructor._  |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::UnknownCommandEvent::ENDSTONE_EVENT (
    UnknownCommandEvent
) 
```




<hr>



### function UnknownCommandEvent 

```C++
inline endstone::UnknownCommandEvent::UnknownCommandEvent (
    const  NotNull < CommandSender > & sender,
    std::string command_line,
    std::optional< Message > message
) 
```




<hr>



### function getCommandLine 

_Gets the command that was sent._ 
```C++
inline std::string endstone::UnknownCommandEvent::getCommandLine () const
```





**Returns:**

command sent 





        

<hr>



### function getMessage 

_Gets the message that will be returned._ 
```C++
inline std::optional< Message > endstone::UnknownCommandEvent::getMessage () const
```





**Returns:**

unknown command message, or no value if no message will be sent 





        

<hr>



### function getSender 

_Gets the command sender._ 
```C++
inline const  NotNull < CommandSender > & endstone::UnknownCommandEvent::getSender () const
```





**Returns:**

sender of the command 





        

<hr>



### function setMessage 

_Sets the message that will be returned._ 
```C++
inline void endstone::UnknownCommandEvent::setMessage (
    std::optional< Message > message
) 
```





**Parameters:**


* `message` the message to be returned, or no value to avoid sending a message 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/command/unknown_command_event.h`

