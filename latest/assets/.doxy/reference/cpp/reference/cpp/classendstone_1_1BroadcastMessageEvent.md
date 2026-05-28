

# Class endstone::BroadcastMessageEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BroadcastMessageEvent**](classendstone_1_1BroadcastMessageEvent.md)



[_**Event**_](classendstone_1_1Event.md) _triggered for server broadcast messages such as from_[_**Server::broadcast**_](classendstone_1_1Server.md#function-broadcast) _._[More...](#detailed-description)

* `#include <endstone/event/server/broadcast_message_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BroadcastMessageEvent**](#function-broadcastmessageevent) ([**bool**](classendstone_1_1Identifier.md) async, Message message, std::unordered\_set&lt; [**const**](classendstone_1_1Identifier.md) [**CommandSender**](classendstone_1_1CommandSender.md) \* &gt; recipients) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**BroadcastMessageEvent**](classendstone_1_1BroadcastMessageEvent.md)) <br> |
|  [**const**](classendstone_1_1Identifier.md) Message & | [**getMessage**](#function-getmessage) () const<br>_Get the message to broadcast._  |
|  [**const**](classendstone_1_1Identifier.md) std::unordered\_set&lt; [**const**](classendstone_1_1Identifier.md) [**CommandSender**](classendstone_1_1CommandSender.md) \* &gt; & | [**getRecipients**](#function-getrecipients) () const<br>_Gets a set of recipients that this broadcast message will be displayed to._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setMessage**](#function-setmessage) (Message message) <br>_Set the message to broadcast._  |


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
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Detailed Description


This event should be async if fired from an async thread. 


    
## Public Functions Documentation




### function BroadcastMessageEvent 

```C++
inline endstone::BroadcastMessageEvent::BroadcastMessageEvent (
    bool async,
    Message message,
    std::unordered_set< const  CommandSender * > recipients
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::BroadcastMessageEvent::ENDSTONE_EVENT (
    BroadcastMessageEvent
) 
```




<hr>



### function getMessage 

_Get the message to broadcast._ 
```C++
inline const Message & endstone::BroadcastMessageEvent::getMessage () const
```





**Returns:**

Message to broadcast 





        

<hr>



### function getRecipients 

_Gets a set of recipients that this broadcast message will be displayed to._ 
```C++
inline const std::unordered_set< const  CommandSender * > & endstone::BroadcastMessageEvent::getRecipients () const
```





**Returns:**

All CommandSenders who will see this broadcast message 





        

<hr>



### function setMessage 

_Set the message to broadcast._ 
```C++
inline void endstone::BroadcastMessageEvent::setMessage (
    Message message
) 
```





**Parameters:**


* `message` New message to broadcast 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/broadcast_message_event.h`

