

# Class endstone::ScriptMessageEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ScriptMessageEvent**](classendstone_1_1ScriptMessageEvent.md)



_Called when a message is sent by_ `/scriptevent` _command._

* `#include <endstone/event/server/script_message_event.h>`



Inherits the following classes: [endstone::ServerEvent](classendstone_1_1ServerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "ScriptMessageEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ScriptMessageEvent**](#function-scriptmessageevent) (std::string message\_id, std::string message, const [**CommandSender**](classendstone_1_1CommandSender.md) & sender) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  const std::string & | [**getMessage**](#function-getmessage) () const<br> |
|  const std::string & | [**getMessageId**](#function-getmessageid) () const<br> |
|  const [**CommandSender**](classendstone_1_1CommandSender.md) & | [**getSender**](#function-getsender) () const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |


## Public Functions inherited from endstone::ServerEvent

See [endstone::ServerEvent](classendstone_1_1ServerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |


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
const std::string endstone::ScriptMessageEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ScriptMessageEvent 

```C++
inline endstone::ScriptMessageEvent::ScriptMessageEvent (
    std::string message_id,
    std::string message,
    const CommandSender & sender
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::ScriptMessageEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


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
inline const CommandSender & endstone::ScriptMessageEvent::getSender () const
```



Gets the command sender who initiated the command




**Returns:**

[**Command**](classendstone_1_1Command.md) sender who initiated the command 





        

<hr>



### function isCancellable 

```C++
inline virtual bool endstone::ScriptMessageEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/script_message_event.h`

