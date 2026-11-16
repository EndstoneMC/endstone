

# Class endstone::PlayerGameModeChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerGameModeChangeEvent**](classendstone_1_1PlayerGameModeChangeEvent.md)



_Called when the_ [_**GameMode**_](namespaceendstone.md#enum-gamemode) _of the player is changed._

* `#include <endstone/event/player/player_game_mode_change_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) (PlayerGameModeChangeEvent) <br> |
|   | [**PlayerGameModeChangeEvent**](#function-playergamemodechangeevent) ([**Player**](classendstone_1_1Player.md) & player, [**GameMode**](namespaceendstone.md#enum-gamemode) new\_game\_mode) <br> |
|  [**GameMode**](namespaceendstone.md#enum-gamemode) | [**getNewGameMode**](#function-getnewgamemode) () const<br>_Gets the_ [_**GameMode**_](namespaceendstone.md#enum-gamemode) _the player is switched to._ |
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
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerGameModeChangeEvent::ENDSTONE_EVENT (
    PlayerGameModeChangeEvent
) 
```




<hr>



### function PlayerGameModeChangeEvent 

```C++
inline explicit endstone::PlayerGameModeChangeEvent::PlayerGameModeChangeEvent (
    Player & player,
    GameMode new_game_mode
) 
```




<hr>



### function getNewGameMode 

_Gets the_ [_**GameMode**_](namespaceendstone.md#enum-gamemode) _the player is switched to._
```C++
inline GameMode endstone::PlayerGameModeChangeEvent::getNewGameMode () const
```





**Returns:**

player's new [**GameMode**](namespaceendstone.md#enum-gamemode) 





        

<hr>



### function ~PlayerGameModeChangeEvent 

```C++
endstone::PlayerGameModeChangeEvent::~PlayerGameModeChangeEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_game_mode_change_event.h`

