

# Class endstone::PlayerRecipeBookClickEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerRecipeBookClickEvent**](classendstone_1_1PlayerRecipeBookClickEvent.md)



_Called when a player clicks a recipe in the recipe book._ [More...](#detailed-description)

* `#include <endstone/event/player/player_recipe_book_click_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerRecipeBookClickEvent**](classendstone_1_1PlayerRecipeBookClickEvent.md)) <br> |
|   | [**PlayerRecipeBookClickEvent**](#function-playerrecipebookclickevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, std::string recipe, [**int**](classendstone_1_1Identifier.md) amount) <br> |
|  [**int**](classendstone_1_1Identifier.md) | [**getAmount**](#function-getamount) () const<br>_Gets the number of times the recipe is being crafted._  |
|  [**const**](classendstone_1_1Identifier.md) std::string & | [**getRecipe**](#function-getrecipe) () const<br>_Gets the identifier of the recipe clicked by the player._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setAmount**](#function-setamount) ([**int**](classendstone_1_1Identifier.md) amount) <br>_Sets the number of times the recipe is being crafted._  |


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

If the event is cancelled the recipe will not be crafted and no ingredients will be consumed. 





    
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerRecipeBookClickEvent::ENDSTONE_EVENT (
    PlayerRecipeBookClickEvent
) 
```




<hr>



### function PlayerRecipeBookClickEvent 

```C++
inline endstone::PlayerRecipeBookClickEvent::PlayerRecipeBookClickEvent (
    const  NotNull < Player > & player,
    std::string recipe,
    int amount
) 
```




<hr>



### function getAmount 

_Gets the number of times the recipe is being crafted._ 
```C++
inline int endstone::PlayerRecipeBookClickEvent::getAmount () const
```





**Returns:**

the number of crafts 





        

<hr>



### function getRecipe 

_Gets the identifier of the recipe clicked by the player._ 
```C++
inline const std::string & endstone::PlayerRecipeBookClickEvent::getRecipe () const
```





**Returns:**

the recipe identifier 





        

<hr>



### function setAmount 

_Sets the number of times the recipe is being crafted._ 
```C++
inline void endstone::PlayerRecipeBookClickEvent::setAmount (
    int amount
) 
```





**Note:**

Values are clamped to the 0-255 range the server accepts.




**Parameters:**


* `amount` the number of crafts 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_recipe_book_click_event.h`

