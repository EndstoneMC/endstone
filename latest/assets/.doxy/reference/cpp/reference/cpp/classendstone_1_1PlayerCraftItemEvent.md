

# Class endstone::PlayerCraftItemEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerCraftItemEvent**](classendstone_1_1PlayerCraftItemEvent.md)



_Called when a player crafts an item._ [More...](#detailed-description)

* `#include <endstone/event/player/player_craft_item_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerCraftItemEvent**](classendstone_1_1PlayerCraftItemEvent.md)) <br> |
|   | [**PlayerCraftItemEvent**](#function-playercraftitemevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**ItemStack**](classendstone_1_1ItemStack.md) item, std::string recipe\_id, [**int**](classendstone_1_1Identifier.md) amount) <br> |
|  [**int**](classendstone_1_1Identifier.md) | [**getAmount**](#function-getamount) () const<br>_Gets the number of times the recipe is being crafted._  |
|  [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getItem**](#function-getitem) () const<br>_Gets the item that is being crafted._  |
|  [**const**](classendstone_1_1Identifier.md) std::string & | [**getRecipeId**](#function-getrecipeid) () const<br>_Gets the identifier of the recipe being used._  |


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




**Note:**

If the event is cancelled the item will not be crafted and the ingredients will not be consumed. 





    
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerCraftItemEvent::ENDSTONE_EVENT (
    PlayerCraftItemEvent
) 
```




<hr>



### function PlayerCraftItemEvent 

```C++
inline endstone::PlayerCraftItemEvent::PlayerCraftItemEvent (
    const  NotNull < Player > & player,
    ItemStack item,
    std::string recipe_id,
    int amount
) 
```




<hr>



### function getAmount 

_Gets the number of times the recipe is being crafted._ 
```C++
inline int endstone::PlayerCraftItemEvent::getAmount () const
```





**Returns:**

the number of crafts 





        

<hr>



### function getItem 

_Gets the item that is being crafted._ 
```C++
inline const  ItemStack & endstone::PlayerCraftItemEvent::getItem () const
```





**Returns:**

an [**ItemStack**](classendstone_1_1ItemStack.md) for the item being crafted 





        

<hr>



### function getRecipeId 

_Gets the identifier of the recipe being used._ 
```C++
inline const std::string & endstone::PlayerCraftItemEvent::getRecipeId () const
```





**Returns:**

the recipe identifier 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_craft_item_event.h`

