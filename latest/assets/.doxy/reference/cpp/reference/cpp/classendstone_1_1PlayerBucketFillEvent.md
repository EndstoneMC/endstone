

# Class endstone::PlayerBucketFillEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerBucketFillEvent**](classendstone_1_1PlayerBucketFillEvent.md)



_Called when a player fills a bucket._ 

* `#include <endstone/event/player/player_bucket_fill_event.h>`



Inherits the following classes: [endstone::PlayerBucketEvent](classendstone_1_1PlayerBucketEvent.md)






























































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerBucketFillEvent**](classendstone_1_1PlayerBucketFillEvent.md)) <br> |
|   | [**PlayerBucketEvent**](#function-playerbucketevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & block, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & block\_clicked, [**BlockFace**](namespaceendstone.md#enum-blockface) block\_face, [**const**](classendstone_1_1Identifier.md) [**ItemType**](classendstone_1_1ItemType.md) & bucket, std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; item\_stack, [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) hand) <br> |


## Public Functions inherited from endstone::PlayerBucketEvent

See [endstone::PlayerBucketEvent](classendstone_1_1PlayerBucketEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**PlayerBucketEvent**](classendstone_1_1PlayerBucketEvent.md#function-playerbucketevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & block, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & block\_clicked, [**BlockFace**](namespaceendstone.md#enum-blockface) block\_face, [**const**](classendstone_1_1Identifier.md) [**ItemType**](classendstone_1_1ItemType.md) & bucket, std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; item\_stack, [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) hand) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & | [**getBlock**](classendstone_1_1PlayerBucketEvent.md#function-getblock) () const<br>_Gets the block involved in this event._  |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & | [**getBlockClicked**](classendstone_1_1PlayerBucketEvent.md#function-getblockclicked) () const<br>_Gets the block clicked by the player._  |
|  [**BlockFace**](namespaceendstone.md#enum-blockface) | [**getBlockFace**](classendstone_1_1PlayerBucketEvent.md#function-getblockface) () const<br>_Gets the face on the clicked block._  |
|  [**const**](classendstone_1_1Identifier.md) [**ItemType**](classendstone_1_1ItemType.md) & | [**getBucket**](classendstone_1_1PlayerBucketEvent.md#function-getbucket) () const<br>_Gets the bucket used in this event._  |
|  [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) | [**getHand**](classendstone_1_1PlayerBucketEvent.md#function-gethand) () const<br>_Gets the hand used in this event._  |
|  [**const**](classendstone_1_1Identifier.md) std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; & | [**getItemStack**](classendstone_1_1PlayerBucketEvent.md#function-getitemstack) () const<br>_Gets the resulting item in the player's hand._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setItemStack**](classendstone_1_1PlayerBucketEvent.md#function-setitemstack) (std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; item\_stack) <br>_Sets the resulting item in the player's hand._  |
|   | [**~PlayerBucketEvent**](classendstone_1_1PlayerBucketEvent.md#function-playerbucketevent) () override<br> |


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




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerBucketFillEvent::ENDSTONE_EVENT (
    PlayerBucketFillEvent
) 
```




<hr>



### function PlayerBucketEvent 

```C++
inline endstone::PlayerBucketFillEvent::PlayerBucketEvent (
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

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_bucket_fill_event.h`

