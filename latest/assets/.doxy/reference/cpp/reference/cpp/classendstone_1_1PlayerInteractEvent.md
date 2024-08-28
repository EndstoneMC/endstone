

# Class endstone::PlayerInteractEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerInteractEvent**](classendstone_1_1PlayerInteractEvent.md)



_Represents an event that is called when a player right-clicks a block._ 

* `#include <endstone/event/player/player_interact_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "PlayerInteractEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerInteractEvent**](#function-playerinteractevent) ([**Player**](classendstone_1_1Player.md) & player, [**Block**](classendstone_1_1Block.md) & block\_clicked, BlockFace block\_face, const [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; & clicked\_position) <br> |
|  BlockFace | [**getBlockFace**](#function-getblockface) () const<br> |
|  [**Block**](classendstone_1_1Block.md) & | [**getClickedBlock**](#function-getclickedblock) () const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; | [**getClickedPosition**](#function-getclickedposition) () const<br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|   | [**~PlayerInteractEvent**](#function-playerinteractevent) () override<br> |


## Public Functions inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) ([**Player**](classendstone_1_1Player.md) & player) <br> |
|  [**Player**](classendstone_1_1Player.md) & | [**getPlayer**](classendstone_1_1PlayerEvent.md#function-getplayer) () const<br> |
|   | [**~PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) () override<br> |


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
const std::string endstone::PlayerInteractEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerInteractEvent 

```C++
inline endstone::PlayerInteractEvent::PlayerInteractEvent (
    Player & player,
    Block & block_clicked,
    BlockFace block_face,
    const Vector < float > & clicked_position
) 
```




<hr>



### function getBlockFace 

```C++
inline BlockFace endstone::PlayerInteractEvent::getBlockFace () const
```




<hr>



### function getClickedBlock 

```C++
inline Block & endstone::PlayerInteractEvent::getClickedBlock () const
```




<hr>



### function getClickedPosition 

```C++
inline Vector < float > endstone::PlayerInteractEvent::getClickedPosition () const
```




<hr>



### function getEventName 


```C++
inline virtual std::string endstone::PlayerInteractEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function isCancellable 


```C++
inline virtual bool endstone::PlayerInteractEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function ~PlayerInteractEvent 

```C++
endstone::PlayerInteractEvent::~PlayerInteractEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_interact_event.h`

