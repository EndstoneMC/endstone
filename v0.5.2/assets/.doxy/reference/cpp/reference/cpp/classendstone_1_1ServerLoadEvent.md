

# Class endstone::ServerLoadEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServerLoadEvent**](classendstone_1_1ServerLoadEvent.md)



_Called when either the server startup or reload has completed._ 

* `#include <endstone/event/server/server_load_event.h>`



Inherits the following classes: [endstone::ServerEvent](classendstone_1_1ServerEvent.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**LoadType**](#enum-loadtype)  <br> |


















## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "ServerLoadEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ServerLoadEvent**](#function-serverloadevent) (LoadType type) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  LoadType | [**getType**](#function-gettype) () const<br> |
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
















































































## Public Types Documentation




### enum LoadType 

```C++
enum endstone::ServerLoadEvent::LoadType {
    Startup,
    Reload
};
```




<hr>
## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::ServerLoadEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ServerLoadEvent 

```C++
inline explicit endstone::ServerLoadEvent::ServerLoadEvent (
    LoadType type
) 
```




<hr>



### function getEventName 


```C++
inline virtual std::string endstone::ServerLoadEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getType 

```C++
inline LoadType endstone::ServerLoadEvent::getType () const
```




<hr>



### function isCancellable 


```C++
inline virtual bool endstone::ServerLoadEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/server_load_event.h`

