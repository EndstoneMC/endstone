

# Class endstone::PlayerGameModeChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerGameModeChangeEvent**](classendstone_1_1PlayerGameModeChangeEvent.md)



_Called when the GameMode of the player is changed._ 

* `#include <endstone/event/player/player_game_mode_change_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"PlayerGameModeChangeEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PlayerGameModeChangeEvent**](#function-playergamemodechangeevent) ([**Player**](classendstone_1_1Player.md) & player, [**GameMode**](namespaceendstone.md#enum-gamemode) new\_game\_mode) <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**GameMode**](namespaceendstone.md#enum-gamemode) | [**getNewGameMode**](#function-getnewgamemode) () const<br>_Gets the GameMode the player is switched to._  |
|   | [**~PlayerGameModeChangeEvent**](#function-playergamemodechangeevent) () override<br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual bool | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual void | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) (bool cancel) override<br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual bool | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual void | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) (bool cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::PlayerGameModeChangeEvent::NAME;
```




<hr>
## Public Functions Documentation




### function PlayerGameModeChangeEvent 

```C++
inline explicit endstone::PlayerGameModeChangeEvent::PlayerGameModeChangeEvent (
    Player & player,
    GameMode new_game_mode
) 
```




<hr>



### function getEventName 

```C++
inline virtual std::string endstone::PlayerGameModeChangeEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getNewGameMode 

_Gets the GameMode the player is switched to._ 
```C++
inline GameMode endstone::PlayerGameModeChangeEvent::getNewGameMode () const
```





**Returns:**

player's new GameMode 





        

<hr>



### function ~PlayerGameModeChangeEvent 

```C++
endstone::PlayerGameModeChangeEvent::~PlayerGameModeChangeEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_game_mode_change_event.h`

