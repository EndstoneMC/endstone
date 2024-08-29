

# Class endstone::ServerCommandEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServerCommandEvent**](classendstone_1_1ServerCommandEvent.md)



_Called when the console runs a command, early in the process._ [More...](#detailed-description)

* `#include <endstone/event/server/server_command_event.h>`



Inherits the following classes: [endstone::ServerEvent](classendstone_1_1ServerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "ServerCommandEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ServerCommandEvent**](#function-servercommandevent) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, std::string command) <br> |
|  std::string | [**getCommand**](#function-getcommand) () const<br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**CommandSender**](classendstone_1_1CommandSender.md) & | [**getSender**](#function-getsender) () const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setCommand**](#function-setcommand) (std::string message) <br> |


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
















































































# Detailed Description


You should not use this except for a few cases like logging commands, blocking commands on certain places, or applying modifiers.


The command message contains a slash at the start 


    
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



### function isCancellable 


```C++
inline virtual bool endstone::ServerCommandEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


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

