

# Class endstone::PlayerInteractEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerInteractEvent**](classendstone_1_1PlayerInteractEvent.md)



_Represents an event that is called when a player right-clicks a block._ 

* `#include <endstone/event/player/player_interact_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Action**](#enum-action)  <br> |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerInteractEvent**](classendstone_1_1PlayerInteractEvent.md)) <br> |
|   | [**PlayerInteractEvent**](#function-playerinteractevent) ([**Player**](classendstone_1_1Player.md) & player, [**Action**](classendstone_1_1PlayerInteractEvent.md#enum-action) action, [**ItemStack**](classendstone_1_1ItemStack.md) \* item, [**Block**](classendstone_1_1Block.md) \* block\_clicked, BlockFace block\_face, const std::optional&lt; [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; &gt; & clicked\_position) <br> |
|  [**Action**](classendstone_1_1PlayerInteractEvent.md#enum-action) | [**getAction**](#function-getaction) () const<br>_Returns the action type._  |
|  [**Block**](classendstone_1_1Block.md) \* | [**getBlock**](#function-getblock) () const<br>_Returns the clicked block._  |
|  BlockFace | [**getBlockFace**](#function-getblockface) () const<br>_Returns the face of the block that was clicked._  |
|  std::optional&lt; [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; &gt; | [**getClickedPosition**](#function-getclickedposition) () const<br>_Gets the exact position on the block the player interacted with._  |
|  [**ItemStack**](classendstone_1_1ItemStack.md) \* | [**getItem**](#function-getitem) () const<br>_Returns the item in hand represented by this event._  |
|  bool | [**hasBlock**](#function-hasblock) () const<br>_Check if this event involved a block._  |
|  bool | [**hasItem**](#function-hasitem) () const<br>_Check if this event involved an item._  |


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
















































































## Public Types Documentation




### enum Action 

```C++
enum endstone::PlayerInteractEvent::Action {
    LeftClickBlock,
    RightClickBlock,
    LeftClickAir,
    RightClickAir
};
```




<hr>
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerInteractEvent::ENDSTONE_EVENT (
    PlayerInteractEvent
) 
```




<hr>



### function PlayerInteractEvent 

```C++
inline endstone::PlayerInteractEvent::PlayerInteractEvent (
    Player & player,
    Action action,
    ItemStack * item,
    Block * block_clicked,
    BlockFace block_face,
    const std::optional< Vector < float > > & clicked_position
) 
```




<hr>



### function getAction 

_Returns the action type._ 
```C++
inline Action endstone::PlayerInteractEvent::getAction () const
```





**Returns:**

Action returns the type of interaction 





        

<hr>



### function getBlock 

_Returns the clicked block._ 
```C++
inline Block * endstone::PlayerInteractEvent::getBlock () const
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
inline std::optional< Vector < float > > endstone::PlayerInteractEvent::getClickedPosition () const
```





**Note:**

This will be std::nullopt outside of Action.RightClickBlock 




**Note:**

All vector components are between 0.0 and 1.0 inclusive.




**Returns:**

the clicked position. 





        

<hr>



### function getItem 

_Returns the item in hand represented by this event._ 
```C++
inline ItemStack * endstone::PlayerInteractEvent::getItem () const
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

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_interact_event.h`

