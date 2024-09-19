

# Class endstone::Objective



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Objective**](classendstone_1_1Objective.md)



_Represents an objective on a scoreboard that can show scores specific to entries._ 

* `#include <endstone/scoreboard/objective.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual const [**Criteria**](classendstone_1_1Criteria.md) \* | [**getCriteria**](#function-getcriteria) () const = 0<br>_Gets the criteria this objective tracks._  |
| virtual std::optional&lt; std::string &gt; | [**getDisplayName**](#function-getdisplayname) () const = 0<br>_Gets the name displayed to players for this objective._  |
| virtual std::optional&lt; DisplaySlot &gt; | [**getDisplaySlot**](#function-getdisplayslot) () const = 0<br>_Gets the display slot this objective is displayed at._  |
| virtual std::optional&lt; std::string &gt; | [**getName**](#function-getname) () const = 0<br>_Gets the name of this_ [_**Objective**_](classendstone_1_1Objective.md) _._ |
| virtual std::optional&lt; RenderType &gt; | [**getRenderType**](#function-getrendertype) () const = 0<br> |
| virtual std::unique\_ptr&lt; [**Score**](classendstone_1_1Score.md) &gt; | [**getScore**](#function-getscore) (ScoreEntry entry) const = 0<br>_Gets an entry's_ [_**Score**_](classendstone_1_1Score.md) _for this objective._ |
| virtual [**Scoreboard**](classendstone_1_1Scoreboard.md) & | [**getScoreboard**](#function-getscoreboard) () const = 0<br>_Gets the scoreboard to which this objective is attached._  |
| virtual std::optional&lt; ObjectiveSortOrder &gt; | [**getSortOrder**](#function-getsortorder) () const = 0<br>_Gets the sort order for this objective._  |
| virtual bool | [**isModifiable**](#function-ismodifiable) () const = 0<br>_Gets if the objective's scores can be modified directly by a plugin._  |
| virtual bool | [**operator!=**](#function-operator) (const [**Objective**](classendstone_1_1Objective.md) & other) const = 0<br> |
| virtual bool | [**operator==**](#function-operator_1) (const [**Objective**](classendstone_1_1Objective.md) & other) const = 0<br> |
| virtual void | [**setDisplay**](#function-setdisplay-12) (std::optional&lt; DisplaySlot &gt; slot) = 0<br>_Sets the display slot for this objective. This will remove it from any other display slot._  |
| virtual void | [**setDisplay**](#function-setdisplay-22) (std::optional&lt; DisplaySlot &gt; slot, ObjectiveSortOrder order) = 0<br>_Sets the display slot and sort order for this objective. This will remove it from any other display slot._  |
| virtual void | [**setDisplayName**](#function-setdisplayname) (std::string display\_name) = 0<br>_Sets the name displayed to players for this objective._  |
| virtual void | [**setRenderType**](#function-setrendertype) (RenderType render\_type) = 0<br>_Sets manner in which this objective will be rendered._  |
| virtual void | [**unregister**](#function-unregister) () const = 0<br>_Unregisters this objective from the associated_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual  | [**~Objective**](#function-objective) () = default<br> |




























## Public Functions Documentation




### function getCriteria 

_Gets the criteria this objective tracks._ 
```C++
virtual const Criteria * endstone::Objective::getCriteria () const = 0
```





**Returns:**

this objective's criteria, or `nullptr` if this objective has been unregistered. 





        

<hr>



### function getDisplayName 

_Gets the name displayed to players for this objective._ 
```C++
virtual std::optional< std::string > endstone::Objective::getDisplayName () const = 0
```





**Returns:**

this objective's display name 





        

<hr>



### function getDisplaySlot 

_Gets the display slot this objective is displayed at._ 
```C++
virtual std::optional< DisplaySlot > endstone::Objective::getDisplaySlot () const = 0
```





**Returns:**

the display slot for this objective 





        

<hr>



### function getName 

_Gets the name of this_ [_**Objective**_](classendstone_1_1Objective.md) _._
```C++
virtual std::optional< std::string > endstone::Objective::getName () const = 0
```





**Returns:**

this objective's name 





        

<hr>



### function getRenderType 


```C++
virtual std::optional< RenderType > endstone::Objective::getRenderType () const = 0
```



Gets manner in which this objective will be rendered.




**Returns:**

the render type 





        

<hr>



### function getScore 

_Gets an entry's_ [_**Score**_](classendstone_1_1Score.md) _for this objective._
```C++
virtual std::unique_ptr< Score > endstone::Objective::getScore (
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
virtual std::optional< ObjectiveSortOrder > endstone::Objective::getSortOrder () const = 0
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



### function operator!= 

```C++
virtual bool endstone::Objective::operator!= (
    const Objective & other
) const = 0
```




<hr>



### function operator== 

```C++
virtual bool endstone::Objective::operator== (
    const Objective & other
) const = 0
```




<hr>



### function setDisplay [1/2]

_Sets the display slot for this objective. This will remove it from any other display slot._ 
```C++
virtual void endstone::Objective::setDisplay (
    std::optional< DisplaySlot > slot
) = 0
```





**Parameters:**


* `slot` The display slot where this objective should be displayed. 




        

<hr>



### function setDisplay [2/2]

_Sets the display slot and sort order for this objective. This will remove it from any other display slot._ 
```C++
virtual void endstone::Objective::setDisplay (
    std::optional< DisplaySlot > slot,
    ObjectiveSortOrder order
) = 0
```





**Parameters:**


* `slot` The display slot where this objective should be displayed. 
* `order` The sort order for this objective in the display slot. 




        

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



### function unregister 

```C++
virtual void endstone::Objective::unregister () const = 0
```




<hr>



### function ~Objective 

```C++
virtual endstone::Objective::~Objective () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/scoreboard/objective.h`

