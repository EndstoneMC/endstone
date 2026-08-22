

# Class endstone::PlayerCraftItemEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerCraftItemEvent**](classendstone_1_1PlayerCraftItemEvent.md)



_Called when a player crafts an item, either inside a crafting grid or straight from the recipe book._ [More...](#detailed-description)

* `#include <endstone/event/player/player_craft_item_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerCraftItemEvent**](classendstone_1_1PlayerCraftItemEvent.md)) <br> |
|   | [**PlayerCraftItemEvent**](#function-playercraftitemevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; ingredients, std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; results, [**int**](classendstone_1_1Identifier.md) repetitions) <br> |
|  [**const**](classendstone_1_1Identifier.md) std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; & | [**getIngredients**](#function-getingredients) () const<br>_Gets the ingredients a single craft consumes._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getRepetitions**](#function-getrepetitions) () const<br>_Gets the number of times the recipe is being crafted._  |
|  [**const**](classendstone_1_1Identifier.md) std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; & | [**getResults**](#function-getresults) () const<br>_Gets the items a single craft produces._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setRepetitions**](#function-setrepetitions) ([**int**](classendstone_1_1Identifier.md) repetitions) <br>_Sets the number of times the recipe is being crafted._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setResults**](#function-setresults) (std::vector&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; results) <br>_Sets the items a single craft produces._  |


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
    std::vector< ItemStack > ingredients,
    std::vector< ItemStack > results,
    int repetitions
) 
```




<hr>



### function getIngredients 

_Gets the ingredients a single craft consumes._ 
```C++
inline const std::vector< ItemStack > & endstone::PlayerCraftItemEvent::getIngredients () const
```





**Note:**

These are the items in the crafting grid where the player used one. Crafting from the recipe book never fills the grid, so the ingredients then come from the recipe instead, and an ingredient that accepts several items reports the one the recipe names rather than the one the player supplied.




**Returns:**

the ingredients the craft consumes 





        

<hr>



### function getRepetitions 

_Gets the number of times the recipe is being crafted._ 
```C++
inline int endstone::PlayerCraftItemEvent::getRepetitions () const
```



This is usually 1, but is higher when a batch is crafted at once, such as a shift click in the recipe book.




**Returns:**

the number of times the recipe is being crafted 





        

<hr>



### function getResults 

_Gets the items a single craft produces._ 
```C++
inline const std::vector< ItemStack > & endstone::PlayerCraftItemEvent::getResults () const
```



A recipe usually produces one item, but may produce several, and an ingredient that leaves a remainder behind contributes one too.




**Returns:**

the items the craft produces 





        

<hr>



### function setRepetitions 

_Sets the number of times the recipe is being crafted._ 
```C++
inline void endstone::PlayerCraftItemEvent::setRepetitions (
    int repetitions
) 
```





**Note:**

Values are clamped to the 0-255 range the server accepts.




**Parameters:**


* `repetitions` the number of times the recipe is being crafted 




        

<hr>



### function setResults 

_Sets the items a single craft produces._ 
```C++
inline void endstone::PlayerCraftItemEvent::setResults (
    std::vector< ItemStack > results
) 
```





**Note:**

Results are replaced one for one, so any beyond the number the recipe produces are ignored. Cancel the event to stop the craft instead.




**Parameters:**


* `results` the items the craft should produce 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_craft_item_event.h`

