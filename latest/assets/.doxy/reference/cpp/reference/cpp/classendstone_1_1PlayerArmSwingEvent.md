

# Class endstone::PlayerArmSwingEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerArmSwingEvent**](classendstone_1_1PlayerArmSwingEvent.md)



_Called when a player swings their arm._ [More...](#detailed-description)

* `#include <endstone/event/player/player_arm_swing_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerArmSwingEvent**](classendstone_1_1PlayerArmSwingEvent.md)) <br> |
|   | [**PlayerArmSwingEvent**](#function-playerarmswingevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; item) <br> |
|  [**const**](classendstone_1_1Identifier.md) std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; & | [**getItem**](#function-getitem) () const<br>_Gets the item the player was holding when they swung their arm._  |


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
















































































## Detailed Description


Cancelling stops the server acting on the swing at all. The swing is neither recorded nor shown to the other players in the dimension.




**Note:**

The swinging player still sees their own arm move, because their client plays the animation without waiting for the server. The event covers swings the player starts. Swings the server drives itself, such as dropping an item, do not fire it. 





    
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerArmSwingEvent::ENDSTONE_EVENT (
    PlayerArmSwingEvent
) 
```




<hr>



### function PlayerArmSwingEvent 

```C++
inline endstone::PlayerArmSwingEvent::PlayerArmSwingEvent (
    const  NotNull < Player > & player,
    std::optional< ItemStack > item
) 
```




<hr>



### function getItem 

_Gets the item the player was holding when they swung their arm._ 
```C++
inline const std::optional< ItemStack > & endstone::PlayerArmSwingEvent::getItem () const
```





**Returns:**

the item in the player's hand, or std::nullopt if the hand was empty. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_arm_swing_event.h`

