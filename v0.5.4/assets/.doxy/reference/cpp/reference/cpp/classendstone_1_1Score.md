

# Class endstone::Score



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Score**](classendstone_1_1Score.md)



_Represents a score for an objective on a scoreboard._ 

* `#include <endstone/scoreboard/score.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual ScoreEntry | [**getEntry**](#function-getentry) () const = 0<br>_Gets the entry being tracked by this_ [_**Score**_](classendstone_1_1Score.md) _._ |
| virtual [**Objective**](classendstone_1_1Objective.md) & | [**getObjective**](#function-getobjective) () const = 0<br>_Gets the_ [_**Objective**_](classendstone_1_1Objective.md) _being tracked by this_[_**Score**_](classendstone_1_1Score.md) _._ |
| virtual [**Scoreboard**](classendstone_1_1Scoreboard.md) & | [**getScoreboard**](#function-getscoreboard) () const = 0<br>_Gets the scoreboard for the associated objective._  |
| virtual int | [**getValue**](#function-getvalue) () const = 0<br>_Gets the current score._  |
| virtual bool | [**isScoreSet**](#function-isscoreset) () const = 0<br>_Shows if this score has been set at any point in time._  |
| virtual void | [**setValue**](#function-setvalue) (int score) = 0<br>_Sets the current score._  |
| virtual  | [**~Score**](#function-score) () = default<br> |




























## Public Functions Documentation




### function getEntry 

_Gets the entry being tracked by this_ [_**Score**_](classendstone_1_1Score.md) _._
```C++
virtual ScoreEntry endstone::Score::getEntry () const = 0
```





**Returns:**

this [**Score**](classendstone_1_1Score.md)'s tracked entry 





        

<hr>



### function getObjective 

_Gets the_ [_**Objective**_](classendstone_1_1Objective.md) _being tracked by this_[_**Score**_](classendstone_1_1Score.md) _._
```C++
virtual Objective & endstone::Score::getObjective () const = 0
```





**Returns:**

the owning objective's scoreboard 





        

<hr>



### function getScoreboard 

_Gets the scoreboard for the associated objective._ 
```C++
virtual Scoreboard & endstone::Score::getScoreboard () const = 0
```





**Returns:**

the owning objective's scoreboard 





        

<hr>



### function getValue 

_Gets the current score._ 
```C++
virtual int endstone::Score::getValue () const = 0
```





**Returns:**

the current score 





        

<hr>



### function isScoreSet 

_Shows if this score has been set at any point in time._ 
```C++
virtual bool endstone::Score::isScoreSet () const = 0
```





**Returns:**

if this score has been set before 





        

<hr>



### function setValue 

_Sets the current score._ 
```C++
virtual void endstone::Score::setValue (
    int score
) = 0
```





**Parameters:**


* `score` New score 




        

<hr>



### function ~Score 

```C++
virtual endstone::Score::~Score () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/scoreboard/score.h`

