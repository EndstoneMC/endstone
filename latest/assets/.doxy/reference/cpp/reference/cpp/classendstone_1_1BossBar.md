

# Class endstone::BossBar



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BossBar**](classendstone_1_1BossBar.md)



_Represents a boss bar that is displayed to players._ 

* `#include <endstone/boss/boss_bar.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**addFlag**](#function-addflag) (BarFlag flag) = 0<br>_Add an optional flag to this boss bar._  |
| virtual void | [**addPlayer**](#function-addplayer) ([**Player**](classendstone_1_1Player.md) & player) = 0<br>_Adds the player to this boss bar causing it to display on their screen._  |
| virtual BarColor | [**getColor**](#function-getcolor) () const = 0<br>_Returns the color of this boss bar._  |
| virtual std::vector&lt; [**Player**](classendstone_1_1Player.md) \* &gt; | [**getPlayers**](#function-getplayers) () const = 0<br>_Returns all players viewing this boss bar._  |
| virtual float | [**getProgress**](#function-getprogress) () const = 0<br>_Returns the progress of the bar between 0.0 and 1.0._  |
| virtual BarStyle | [**getStyle**](#function-getstyle) () const = 0<br>_Returns the style of this boss bar._  |
| virtual std::string | [**getTitle**](#function-gettitle) () const = 0<br>_Returns the title of this boss bar._  |
| virtual bool | [**hasFlag**](#function-hasflag) (BarFlag flag) const = 0<br>_Returns whether this boss bar has the passed flag set._  |
| virtual bool | [**isVisible**](#function-isvisible) () const = 0<br>_Return if the boss bar is displayed to attached players._  |
| virtual void | [**removeAll**](#function-removeall) () = 0<br>_Removes all players from this boss bar._  |
| virtual void | [**removeFlag**](#function-removeflag) (BarFlag flag) = 0<br>_Remove an existing flag on this boss bar._  |
| virtual void | [**removePlayer**](#function-removeplayer) ([**Player**](classendstone_1_1Player.md) & player) = 0<br>_Removes the player from this boss bar causing it to be removed from their screen._  |
| virtual void | [**setColor**](#function-setcolor) (BarColor color) = 0<br>_Sets the color of this boss bar._  |
| virtual void | [**setProgress**](#function-setprogress) (float progress) = 0<br>_Sets the progress of the bar. Values should be between 0.0 (empty) and 1.0 (full)_  |
| virtual void | [**setStyle**](#function-setstyle) (BarStyle style) = 0<br>_Sets the bar style of this boss bar._  |
| virtual void | [**setTitle**](#function-settitle) (std::string title) = 0<br>_Sets the title of this boss bar._  |
| virtual void | [**setVisible**](#function-setvisible) (bool visible) = 0<br>_Set if the boss bar is displayed to attached players._  |
| virtual  | [**~BossBar**](#function-bossbar) () = default<br> |




























## Public Functions Documentation




### function addFlag 

_Add an optional flag to this boss bar._ 
```C++
virtual void endstone::BossBar::addFlag (
    BarFlag flag
) = 0
```





**Parameters:**


* `flag` an optional flag to set on the boss bar. 




        

<hr>



### function addPlayer 

_Adds the player to this boss bar causing it to display on their screen._ 
```C++
virtual void endstone::BossBar::addPlayer (
    Player & player
) = 0
```





**Parameters:**


* `player` the player to add 




        

<hr>



### function getColor 

_Returns the color of this boss bar._ 
```C++
virtual BarColor endstone::BossBar::getColor () const = 0
```





**Returns:**

the color of the bar 





        

<hr>



### function getPlayers 

_Returns all players viewing this boss bar._ 
```C++
virtual std::vector< Player * > endstone::BossBar::getPlayers () const = 0
```





**Returns:**

a list of players 





        

<hr>



### function getProgress 

_Returns the progress of the bar between 0.0 and 1.0._ 
```C++
virtual float endstone::BossBar::getProgress () const = 0
```





**Returns:**

the progress of the bar 





        

<hr>



### function getStyle 

_Returns the style of this boss bar._ 
```C++
virtual BarStyle endstone::BossBar::getStyle () const = 0
```





**Returns:**

the style of the bar. 





        

<hr>



### function getTitle 

_Returns the title of this boss bar._ 
```C++
virtual std::string endstone::BossBar::getTitle () const = 0
```





**Returns:**

the title of the bar 





        

<hr>



### function hasFlag 

_Returns whether this boss bar has the passed flag set._ 
```C++
virtual bool endstone::BossBar::hasFlag (
    BarFlag flag
) const = 0
```





**Parameters:**


* `flag` the flag to check. 



**Returns:**

whether it has the flag. 





        

<hr>



### function isVisible 

_Return if the boss bar is displayed to attached players._ 
```C++
virtual bool endstone::BossBar::isVisible () const = 0
```





**Returns:**

visible status 





        

<hr>



### function removeAll 

```C++
virtual void endstone::BossBar::removeAll () = 0
```




<hr>



### function removeFlag 

_Remove an existing flag on this boss bar._ 
```C++
virtual void endstone::BossBar::removeFlag (
    BarFlag flag
) = 0
```





**Parameters:**


* `flag` the existing flag to remove. 




        

<hr>



### function removePlayer 

_Removes the player from this boss bar causing it to be removed from their screen._ 
```C++
virtual void endstone::BossBar::removePlayer (
    Player & player
) = 0
```





**Parameters:**


* `player` the player to remove 




        

<hr>



### function setColor 

_Sets the color of this boss bar._ 
```C++
virtual void endstone::BossBar::setColor (
    BarColor color
) = 0
```





**Parameters:**


* `color` the color of the bar 




        

<hr>



### function setProgress 

_Sets the progress of the bar. Values should be between 0.0 (empty) and 1.0 (full)_ 
```C++
virtual void endstone::BossBar::setProgress (
    float progress
) = 0
```





**Parameters:**


* `progress` the progress of the bar 




        

<hr>



### function setStyle 

_Sets the bar style of this boss bar._ 
```C++
virtual void endstone::BossBar::setStyle (
    BarStyle style
) = 0
```





**Parameters:**


* `style` the style of the bar. 




        

<hr>



### function setTitle 

_Sets the title of this boss bar._ 
```C++
virtual void endstone::BossBar::setTitle (
    std::string title
) = 0
```





**Parameters:**


* `title` the title of the bar 




        

<hr>



### function setVisible 

_Set if the boss bar is displayed to attached players._ 
```C++
virtual void endstone::BossBar::setVisible (
    bool visible
) = 0
```





**Parameters:**


* `visible` visible status 




        

<hr>



### function ~BossBar 

```C++
virtual endstone::BossBar::~BossBar () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/boss/boss_bar.h`

