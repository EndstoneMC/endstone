

# Class endstone::PlayerInteractEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerInteractEvent**](classendstone_1_1PlayerInteractEvent.md)



_Represents an event that is called when a player right-clicks a block._ 

* `#include <endstone/event/player/player_interact_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**NAME**](#variable-name)   = `"PlayerInteractEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerInteractEvent**](#function-playerinteractevent) ([**Player**](classendstone_1_1Player.md) & player, std::shared\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; item, std::shared\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; block\_clicked, BlockFace block\_face, [**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**float**](classendstone_1_1Vector.md) &gt; & clicked\_position) <br> |
|  std::shared\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlock**](#function-getblock) () const<br>_Returns the clicked block._  |
|  BlockFace | [**getBlockFace**](#function-getblockface) () const<br>_Returns the face of the block that was clicked._  |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**float**](classendstone_1_1Vector.md) &gt; | [**getClickedPosition**](#function-getclickedposition) () const<br>_Gets the exact position on the block the player interacted with._  |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  std::shared\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getItem**](#function-getitem) () const<br>_Returns the item in hand represented by this event._  |
|  [**bool**](classendstone_1_1Vector.md) | [**hasBlock**](#function-hasblock) () const<br>_Check if this event involved a block._  |
|  [**bool**](classendstone_1_1Vector.md) | [**hasItem**](#function-hasitem) () const<br>_Check if this event involved an item._  |
|   | [**~PlayerInteractEvent**](#function-playerinteractevent) () override<br> |


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
const std::string endstone::PlayerInteractEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerInteractEvent 

```C++
inline endstone::PlayerInteractEvent::PlayerInteractEvent (
    Player & player,
    std::shared_ptr< ItemStack > item,
    std::shared_ptr< Block > block_clicked,
    BlockFace block_face,
    const  Vector < float > & clicked_position
) 
```




<hr>



### function getBlock 

_Returns the clicked block._ 
```C++
inline std::shared_ptr< Block > endstone::PlayerInteractEvent::getBlock () const
```





**Returns:**

[**Block**](classendstone_1_1Block.md) returns the block clicked with this item. 





        

<hr>



### function getBlockFace 

_Returns the face of the block that was clicked._ 
```C++
inline BlockFace endstone::PlayerInteractEvent::getBlockFace () const
```





**Returns:**

BlockFace returns the face of the block that was clicked 





        

<hr>



### function getClickedPosition 

_Gets the exact position on the block the player interacted with._ 
```C++
inline Vector < float > endstone::PlayerInteractEvent::getClickedPosition () const
```



All vector components are between 0.0 and 1.0 inclusive.




**Returns:**

the clicked position. 





        

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



### function getItem 

_Returns the item in hand represented by this event._ 
```C++
inline std::shared_ptr< ItemStack > endstone::PlayerInteractEvent::getItem () const
```





**Returns:**

[**ItemStack**](classendstone_1_1ItemStack.md) the item used 





        

<hr>



### function hasBlock 

_Check if this event involved a block._ 
```C++
inline bool endstone::PlayerInteractEvent::hasBlock () const
```





**Returns:**

boolean true if it did 





        

<hr>



### function hasItem 

_Check if this event involved an item._ 
```C++
inline bool endstone::PlayerInteractEvent::hasItem () const
```





**Returns:**

boolean true if it did 





        

<hr>



### function ~PlayerInteractEvent 

```C++
endstone::PlayerInteractEvent::~PlayerInteractEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_interact_event.h`

