

# Class endstone::PlayerRecipeBookSettingsChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerRecipeBookSettingsChangeEvent**](classendstone_1_1PlayerRecipeBookSettingsChangeEvent.md)



_Called when a player changes recipe book settings._ 

* `#include <endstone/event/player/player_recipe_book_settings_change_event.h>`



Inherits the following classes: [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**RecipeBookType**](#enum-recipebooktype)  <br>_Represents the type of recipe book whose settings changed._  |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerRecipeBookSettingsChangeEvent**](classendstone_1_1PlayerRecipeBookSettingsChangeEvent.md)) <br> |
|   | [**PlayerRecipeBookSettingsChangeEvent**](#function-playerrecipebooksettingschangeevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**RecipeBookType**](classendstone_1_1PlayerRecipeBookSettingsChangeEvent.md#enum-recipebooktype) recipe\_book\_type, [**bool**](classendstone_1_1Identifier.md) is\_open, [**bool**](classendstone_1_1Identifier.md) is\_filtering) <br> |
|  [**RecipeBookType**](classendstone_1_1PlayerRecipeBookSettingsChangeEvent.md#enum-recipebooktype) | [**getRecipeBookType**](#function-getrecipebooktype) () const<br>_Gets the type of recipe book whose settings changed._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isFiltering**](#function-isfiltering) () const<br>_Gets whether recipe filtering is enabled._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isOpen**](#function-isopen) () const<br>_Gets whether the recipe book is open._  |


## Public Functions inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & | [**getPlayer**](classendstone_1_1PlayerEvent.md#function-getplayer) () const<br>_Returns the player involved in this event._  |
|   | [**~PlayerEvent**](classendstone_1_1PlayerEvent.md#function-playerevent) () override<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br>_Gets a user-friendly identifier for this event._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br>_Any custom event that should not by synchronized with other events must use the specific constructor._  |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |






















## Protected Attributes inherited from endstone::PlayerEvent

See [endstone::PlayerEvent](classendstone_1_1PlayerEvent.md)

| Type | Name |
| ---: | :--- |
|  [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; | [**player\_**](classendstone_1_1PlayerEvent.md#variable-player_)  <br> |


























































## Public Types Documentation




### enum RecipeBookType 

_Represents the type of recipe book whose settings changed._ 
```C++
enum endstone::PlayerRecipeBookSettingsChangeEvent::RecipeBookType {
    Crafting,
    Furnace,
    BlastFurnace,
    Smoker
};
```




<hr>
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerRecipeBookSettingsChangeEvent::ENDSTONE_EVENT (
    PlayerRecipeBookSettingsChangeEvent
) 
```




<hr>



### function PlayerRecipeBookSettingsChangeEvent 

```C++
inline endstone::PlayerRecipeBookSettingsChangeEvent::PlayerRecipeBookSettingsChangeEvent (
    const  NotNull < Player > & player,
    RecipeBookType recipe_book_type,
    bool is_open,
    bool is_filtering
) 
```




<hr>



### function getRecipeBookType 

_Gets the type of recipe book whose settings changed._ 
```C++
inline RecipeBookType endstone::PlayerRecipeBookSettingsChangeEvent::getRecipeBookType () const
```





**Returns:**

the recipe book type 





        

<hr>



### function isFiltering 

_Gets whether recipe filtering is enabled._ 
```C++
inline bool endstone::PlayerRecipeBookSettingsChangeEvent::isFiltering () const
```





**Returns:**

true if recipe filtering is enabled 





        

<hr>



### function isOpen 

_Gets whether the recipe book is open._ 
```C++
inline bool endstone::PlayerRecipeBookSettingsChangeEvent::isOpen () const
```





**Returns:**

true if the recipe book is open 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_recipe_book_settings_change_event.h`

