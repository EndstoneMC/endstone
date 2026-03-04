

# Class endstone::ScriptMessageEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ScriptMessageEvent**](classendstone_1_1ScriptMessageEvent.md)



_Called when a message is sent by_ `/scriptevent` _command._

* `#include <endstone/event/server/script_message_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ScriptMessageEvent**](classendstone_1_1ScriptMessageEvent.md)) <br> |
|   | [**ScriptMessageEvent**](#function-scriptmessageevent) (std::string message\_id, std::string message, [**const**](classendstone_1_1Identifier.md) [**CommandSender**](classendstone_1_1CommandSender.md) & sender) <br> |
|  [**const**](classendstone_1_1Identifier.md) std::string & | [**getMessage**](#function-getmessage) () const<br> |
|  [**const**](classendstone_1_1Identifier.md) std::string & | [**getMessageId**](#function-getmessageid) () const<br> |
|  [**const**](classendstone_1_1Identifier.md) [**CommandSender**](classendstone_1_1CommandSender.md) & | [**getSender**](#function-getsender) () const<br> |


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
endstone::ScriptMessageEvent::ENDSTONE_EVENT (
    ScriptMessageEvent
) 
```




<hr>



### function ScriptMessageEvent 

```C++
inline endstone::ScriptMessageEvent::ScriptMessageEvent (
    std::string message_id,
    std::string message,
    const  CommandSender & sender
) 
```




<hr>



### function getMessage 

```C++
inline const std::string & endstone::ScriptMessageEvent::getMessage () const
```



Get the message to send.




**Returns:**

Message to send 





        

<hr>



### function getMessageId 

```C++
inline const std::string & endstone::ScriptMessageEvent::getMessageId () const
```



Get the message id to send.




**Returns:**

Message id to send 





        

<hr>



### function getSender 

```C++
inline const  CommandSender & endstone::ScriptMessageEvent::getSender () const
```



Gets the command sender who initiated the command




**Returns:**

[**Command**](classendstone_1_1Command.md) sender who initiated the command 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/script_message_event.h`

