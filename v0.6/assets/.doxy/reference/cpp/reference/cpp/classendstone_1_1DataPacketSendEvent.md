

# Class endstone::DataPacketSendEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**DataPacketSendEvent**](classendstone_1_1DataPacketSendEvent.md)



_Called when the server sends a packet to a connected client._ 

* `#include <endstone/event/server/data_packet_send_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**NAME**](#variable-name)   = `"DataPacketSendEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DataPacketSendEvent**](#function-datapacketsendevent) ([**Player**](classendstone_1_1Player.md) & player, std::string\_view data) <br> |
|  std::string\_view | [**getData**](#function-getdata) () const<br>_Gets the raw packet data._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](#function-getplayer) () const<br> |


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
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::DataPacketSendEvent::NAME;
```




<hr>
## Public Functions Documentation




### function DataPacketSendEvent 

```C++
inline endstone::DataPacketSendEvent::DataPacketSendEvent (
    Player & player,
    std::string_view data
) 
```




<hr>



### function getData 

_Gets the raw packet data._ 
```C++
inline std::string_view endstone::DataPacketSendEvent::getData () const
```





**Returns:**

The packet data. 





        

<hr>



### function getEventName 

```C++
inline virtual std::string endstone::DataPacketSendEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getPlayer 

```C++
inline Player & endstone::DataPacketSendEvent::getPlayer () const
```



Returns the player involved in this event




**Returns:**

[**Player**](classendstone_1_1Player.md) who is involved in this event 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/data_packet_send_event.h`

