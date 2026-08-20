

# Class endstone::PlayerArmorStandManipulateEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerArmorStandManipulateEvent**](classendstone_1_1PlayerArmorStandManipulateEvent.md)



_Called when a player interacts with an armor stand and will either swap, retrieve or place an item._ 

* `#include <endstone/event/player/player_armor_stand_manipulate_event.h>`



Inherits the following classes: [endstone::PlayerInteractActorEvent](classendstone_1_1PlayerInteractActorEvent.md)






























































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerArmorStandManipulateEvent**](classendstone_1_1PlayerArmorStandManipulateEvent.md)) <br> |
|   | [**PlayerArmorStandManipulateEvent**](#function-playerarmorstandmanipulateevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & right\_clicked, [**ItemStack**](classendstone_1_1ItemStack.md) armor\_stand\_item, [**ItemStack**](classendstone_1_1ItemStack.md) player\_item, [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) slot) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getArmorStandItem**](#function-getarmorstanditem) () const<br>_Gets the item held by the armor stand in the affected slot._  |
|  [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getPlayerItem**](#function-getplayeritem) () const<br>_Gets the item held by the player during the interaction._  |
|  [**EquipmentSlot**](namespaceendstone.md#enum-equipmentslot) | [**getSlot**](#function-getslot) () const<br>_Gets the armor stand equipment slot affected by the interaction._  |


## Public Functions inherited from endstone::PlayerInteractActorEvent

See [endstone::PlayerInteractActorEvent](classendstone_1_1PlayerInteractActorEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](classendstone_1_1PlayerInteractActorEvent.md#function-endstone_event) ([**PlayerInteractActorEvent**](classendstone_1_1PlayerInteractActorEvent.md)) <br> |
|   | [**PlayerInteractActorEvent**](classendstone_1_1PlayerInteractActorEvent.md#function-playerinteractactorevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & actor) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & | [**getActor**](classendstone_1_1PlayerInteractActorEvent.md#function-getactor) () const<br>_Gets the actor that was right-clicked by the player._  |
|   | [**~PlayerInteractActorEvent**](classendstone_1_1PlayerInteractActorEvent.md#function-playerinteractactorevent) () override<br> |


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
endstone::PlayerArmorStandManipulateEvent::ENDSTONE_EVENT (
    PlayerArmorStandManipulateEvent
) 
```




<hr>



### function PlayerArmorStandManipulateEvent 

```C++
inline endstone::PlayerArmorStandManipulateEvent::PlayerArmorStandManipulateEvent (
    const  NotNull < Player > & player,
    const  NotNull < Actor > & right_clicked,
    ItemStack armor_stand_item,
    ItemStack player_item,
    EquipmentSlot slot
) 
```




<hr>



### function getArmorStandItem 

_Gets the item held by the armor stand in the affected slot._ 
```C++
inline const  ItemStack & endstone::PlayerArmorStandManipulateEvent::getArmorStandItem () const
```





**Returns:**

the item held by the armor stand 





        

<hr>



### function getPlayerItem 

_Gets the item held by the player during the interaction._ 
```C++
inline const  ItemStack & endstone::PlayerArmorStandManipulateEvent::getPlayerItem () const
```





**Returns:**

the item held by the player 





        

<hr>



### function getSlot 

_Gets the armor stand equipment slot affected by the interaction._ 
```C++
inline EquipmentSlot endstone::PlayerArmorStandManipulateEvent::getSlot () const
```





**Returns:**

the affected armor stand slot 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_armor_stand_manipulate_event.h`

