

# Class endstone::BroadcastMessageEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BroadcastMessageEvent**](classendstone_1_1BroadcastMessageEvent.md)



[_**Event**_](classendstone_1_1Event.md) _triggered for server broadcast messages such as from_[_**Server::broadcast**_](classendstone_1_1Server.md#function-broadcast) _._[More...](#detailed-description)

* `#include <endstone/event/server/broadcast_message_event.h>`



Inherits the following classes: [endstone::ServerEvent](classendstone_1_1ServerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "BroadcastMessageEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BroadcastMessageEvent**](#function-broadcastmessageevent) (bool async, std::string message, std::unordered\_set&lt; const [**CommandSender**](classendstone_1_1CommandSender.md) \* &gt; recipients) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  const std::string & | [**getMessage**](#function-getmessage) () const<br> |
|  const std::unordered\_set&lt; const [**CommandSender**](classendstone_1_1CommandSender.md) \* &gt; & | [**getRecipients**](#function-getrecipients) () const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  void | [**setMessage**](#function-setmessage) (std::string message) <br> |


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


This event should be async if fired from an async thread. 


    
## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::BroadcastMessageEvent::NAME;
```




<hr>
## Public Functions Documentation




### function BroadcastMessageEvent 

```C++
inline endstone::BroadcastMessageEvent::BroadcastMessageEvent (
    bool async,
    std::string message,
    std::unordered_set< const CommandSender * > recipients
) 
```




<hr>



### function getEventName 


```C++
inline virtual std::string endstone::BroadcastMessageEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getMessage 


```C++
inline const std::string & endstone::BroadcastMessageEvent::getMessage () const
```



Get the message to broadcast.




**Returns:**

Message to broadcast 





        

<hr>



### function getRecipients 


```C++
inline const std::unordered_set< const CommandSender * > & endstone::BroadcastMessageEvent::getRecipients () const
```



Gets a set of recipients that this broadcast message will be displayed to.




**Returns:**

All CommandSenders who will see this broadcast message 





        

<hr>



### function isCancellable 


```C++
inline virtual bool endstone::BroadcastMessageEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function setMessage 


```C++
inline void endstone::BroadcastMessageEvent::setMessage (
    std::string message
) 
```



Set the message to broadcast.




**Parameters:**


* `message` New message to broadcast 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/broadcast_message_event.h`

