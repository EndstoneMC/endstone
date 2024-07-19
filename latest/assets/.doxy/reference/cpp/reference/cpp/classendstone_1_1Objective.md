

# Class endstone::Objective



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Objective**](classendstone_1_1Objective.md)



_Represents an objective on a scoreboard that can show scores specific to entries._ 

* `#include <endstone/scoreboard/objective.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Criteria**](classendstone_1_1Criteria.md) & | [**getCriteria**](#function-getcriteria) () const = 0<br>_Gets the criteria this objective tracks._  |
| virtual std::string | [**getDisplayName**](#function-getdisplayname) () const = 0<br>_Gets the name displayed to players for this objective._  |
| virtual DisplaySlot | [**getDisplaySlot**](#function-getdisplayslot) () const = 0<br>_Gets the display slot this objective is displayed at._  |
| virtual std::string | [**getName**](#function-getname) () const = 0<br>_Gets the name of this_ [_**Objective**_](classendstone_1_1Objective.md) _._ |
| virtual RenderType | [**getRenderType**](#function-getrendertype) () const = 0<br> |
| virtual [**Score**](classendstone_1_1Score.md) & | [**getScore**](#function-getscore) (ScoreEntry entry) const = 0<br>_Gets an entry's_ [_**Score**_](classendstone_1_1Score.md) _for this objective._ |
| virtual [**Scoreboard**](classendstone_1_1Scoreboard.md) & | [**getScoreboard**](#function-getscoreboard) () const = 0<br>_Gets the scoreboard to which this objective is attached._  |
| virtual ObjectiveSortOrder | [**getSortOrder**](#function-getsortorder) () const = 0<br>_Gets the sort order for this objective._  |
| virtual bool | [**isModifiable**](#function-ismodifiable) () const = 0<br>_Gets if the objective's scores can be modified directly by a plugin._  |
| virtual void | [**setDisplayName**](#function-setdisplayname) (std::string display\_name) = 0<br>_Sets the name displayed to players for this objective._  |
| virtual void | [**setDisplaySlot**](#function-setdisplayslot) (DisplaySlot slot) = 0<br>_Sets this objective to display on the specified slot for the scoreboard, removing it from any other display slot._  |
| virtual void | [**setRenderType**](#function-setrendertype) (RenderType render\_type) = 0<br>_Sets manner in which this objective will be rendered._  |
| virtual void | [**setSortOrder**](#function-setsortorder) (ObjectiveSortOrder order) = 0<br>_Sets the sort order for this objective._  |
| virtual  | [**~Objective**](#function-objective) () = default<br> |




























## Public Functions Documentation




### function getCriteria 

_Gets the criteria this objective tracks._ 
```C++
virtual Criteria & endstone::Objective::getCriteria () const = 0
```





**Returns:**

this objective's criteria 





        

<hr>



### function getDisplayName 

_Gets the name displayed to players for this objective._ 
```C++
virtual std::string endstone::Objective::getDisplayName () const = 0
```





**Returns:**

this objective's display name 





        

<hr>



### function getDisplaySlot 

_Gets the display slot this objective is displayed at._ 
```C++
virtual DisplaySlot endstone::Objective::getDisplaySlot () const = 0
```





**Returns:**

the display slot for this objective 





        

<hr>



### function getName 

_Gets the name of this_ [_**Objective**_](classendstone_1_1Objective.md) _._
```C++
virtual std::string endstone::Objective::getName () const = 0
```





**Returns:**

this objective's name 





        

<hr>



### function getRenderType 


```C++
virtual RenderType endstone::Objective::getRenderType () const = 0
```



Gets manner in which this objective will be rendered.




**Returns:**

the render type 





        

<hr>



### function getScore 

_Gets an entry's_ [_**Score**_](classendstone_1_1Score.md) _for this objective._
```C++
virtual Score & endstone::Objective::getScore (
    ScoreEntry entry
) const = 0
```





**Parameters:**


* `entry` Entry for the [**Score**](classendstone_1_1Score.md) 



**Returns:**

[**Score**](classendstone_1_1Score.md) tracking the [**Objective**](classendstone_1_1Objective.md) and entry specified 





        

<hr>



### function getScoreboard 

_Gets the scoreboard to which this objective is attached._ 
```C++
virtual Scoreboard & endstone::Objective::getScoreboard () const = 0
```





**Returns:**

Owning scoreboard 





        

<hr>



### function getSortOrder 

_Gets the sort order for this objective._ 
```C++
virtual ObjectiveSortOrder endstone::Objective::getSortOrder () const = 0
```





**Returns:**

The sort order for this objective. 





        

<hr>



### function isModifiable 

_Gets if the objective's scores can be modified directly by a plugin._ 
```C++
virtual bool endstone::Objective::isModifiable () const = 0
```





**Returns:**

true if scores are modifiable 





        

<hr>



### function setDisplayName 

_Sets the name displayed to players for this objective._ 
```C++
virtual void endstone::Objective::setDisplayName (
    std::string display_name
) = 0
```





**Parameters:**


* `displayName` Display name to set 




        

<hr>



### function setDisplaySlot 

_Sets this objective to display on the specified slot for the scoreboard, removing it from any other display slot._ 
```C++
virtual void endstone::Objective::setDisplaySlot (
    DisplaySlot slot
) = 0
```





**Parameters:**


* `slot` display slot to change 




        

<hr>



### function setRenderType 

_Sets manner in which this objective will be rendered._ 
```C++
virtual void endstone::Objective::setRenderType (
    RenderType render_type
) = 0
```





**Parameters:**


* `render_type` new render type 




        

<hr>



### function setSortOrder 

_Sets the sort order for this objective._ 
```C++
virtual void endstone::Objective::setSortOrder (
    ObjectiveSortOrder order
) = 0
```





**Parameters:**


* `order` The sort order to set. 




        

<hr>



### function ~Objective 

```C++
virtual endstone::Objective::~Objective () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/scoreboard/objective.h`

