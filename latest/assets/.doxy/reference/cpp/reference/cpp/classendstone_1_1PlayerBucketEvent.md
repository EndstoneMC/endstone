

# Class endstone::PlayerBucketEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerBucketEvent**](classendstone_1_1PlayerBucketEvent.md)



_Provides common data for events involving a player's bucket interaction._ 

* `#include <endstone/event/player/player_bucket_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)


Inherited by the following classes: [endstone::PlayerBucketEmptyEvent](classendstone_1_1PlayerBucketEmptyEvent.md),  [endstone::PlayerBucketFillEvent](classendstone_1_1PlayerBucketFillEvent.md)








































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerBucketEvent**](#function-playerbucketevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & block, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & block\_clicked, [**BlockFace**](namespaceendstone.md#enum-blockface) block\_face, [**const**](classendstone_1_1Identifier.md) [**ItemType**](classendstone_1_1ItemType.md) & bucket, std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; item\_stack, [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) hand) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & | [**getBlock**](#function-getblock) () const<br>_Gets the block involved in this event._  |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & | [**getBlockClicked**](#function-getblockclicked) () const<br>_Gets the block clicked by the player._  |
|  [**BlockFace**](namespaceendstone.md#enum-blockface) | [**getBlockFace**](#function-getblockface) () const<br>_Gets the face on the clicked block._  |
|  [**const**](classendstone_1_1Identifier.md) [**ItemType**](classendstone_1_1ItemType.md) & | [**getBucket**](#function-getbucket) () const<br>_Gets the bucket used in this event._  |
|  [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) | [**getHand**](#function-gethand) () const<br>_Gets the hand used in this event._  |
|  [**const**](classendstone_1_1Identifier.md) std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; & | [**getItemStack**](#function-getitemstack) () const<br>_Gets the resulting item in the player's hand._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setItemStack**](#function-setitemstack) (std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; item\_stack) <br>_Sets the resulting item in the player's hand._  |
|   | [**~PlayerBucketEvent**](#function-playerbucketevent) () override<br> |


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
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br>_Cancels this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br>_Sets the cancellation state of this event._  |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Functions Documentation




### function PlayerBucketEvent 

```C++
inline endstone::PlayerBucketEvent::PlayerBucketEvent (
    const  NotNull < Player > & player,
    const  Nullable < Block > & block,
    const  NotNull < Block > & block_clicked,
    BlockFace block_face,
    const  ItemType & bucket,
    std::optional< ItemStack > item_stack,
    EquipmentSlot hand
) 
```




<hr>



### function getBlock 

_Gets the block involved in this event._ 
```C++
inline const  Nullable < Block > & endstone::PlayerBucketEvent::getBlock () const
```





**Returns:**

the block involved in this event, or a null handle if unavailable 





        

<hr>



### function getBlockClicked 

_Gets the block clicked by the player._ 
```C++
inline const  NotNull < Block > & endstone::PlayerBucketEvent::getBlockClicked () const
```





**Returns:**

the clicked block 





        

<hr>



### function getBlockFace 

_Gets the face on the clicked block._ 
```C++
inline BlockFace endstone::PlayerBucketEvent::getBlockFace () const
```





**Returns:**

the clicked block face 





        

<hr>



### function getBucket 

_Gets the bucket used in this event._ 
```C++
inline const  ItemType & endstone::PlayerBucketEvent::getBucket () const
```





**Returns:**

the bucket item type 





        

<hr>



### function getHand 

_Gets the hand used in this event._ 
```C++
inline EquipmentSlot endstone::PlayerBucketEvent::getHand () const
```





**Note:**

This is always EquipmentSlot::Hand, because Bedrock does not report which hand was used for this interaction.




**Returns:**

the hand 





        

<hr>



### function getItemStack 

_Gets the resulting item in the player's hand._ 
```C++
inline const std::optional< ItemStack > & endstone::PlayerBucketEvent::getItemStack () const
```





**Returns:**

the resulting item stack, or std::nullopt if unavailable 





        

<hr>



### function setItemStack 

_Sets the resulting item in the player's hand._ 
```C++
inline void endstone::PlayerBucketEvent::setItemStack (
    std::optional< ItemStack > item_stack
) 
```





**Parameters:**


* `item_stack` the item stack to write back after the event, or std::nullopt 




        

<hr>



### function ~PlayerBucketEvent 

```C++
endstone::PlayerBucketEvent::~PlayerBucketEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_bucket_event.h`

