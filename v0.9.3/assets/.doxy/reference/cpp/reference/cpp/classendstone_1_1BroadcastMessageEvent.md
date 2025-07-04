

# Class endstone::BroadcastMessageEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BroadcastMessageEvent**](classendstone_1_1BroadcastMessageEvent.md)



[_**Event**_](classendstone_1_1Event.md) _triggered for server broadcast messages such as from_[_**Server::broadcast**_](classendstone_1_1Server.md#function-broadcast) _._[More...](#detailed-description)

* `#include <endstone/event/server/broadcast_message_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"BroadcastMessageEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BroadcastMessageEvent**](#function-broadcastmessageevent) (bool async, Message message, std::unordered\_set&lt; const [**CommandSender**](classendstone_1_1CommandSender.md) \* &gt; recipients) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  const Message & | [**getMessage**](#function-getmessage) () const<br> |
|  const std::unordered\_set&lt; const [**CommandSender**](classendstone_1_1CommandSender.md) \* &gt; & | [**getRecipients**](#function-getrecipients) () const<br> |
|  void | [**setMessage**](#function-setmessage) (Message message) <br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual bool | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual void | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) (bool cancel) <br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual bool | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual void | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) (bool cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Detailed Description


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
    Message message,
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
inline const Message & endstone::BroadcastMessageEvent::getMessage () const
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



### function setMessage 

```C++
inline void endstone::BroadcastMessageEvent::setMessage (
    Message message
) 
```



Set the message to broadcast.




**Parameters:**


* `message` New message to broadcast 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/broadcast_message_event.h`

