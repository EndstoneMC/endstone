

# Class endstone::Scoreboard



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Scoreboard**](classendstone_1_1Scoreboard.md)



_Represents a scoreboard._ 

* `#include <endstone/scoreboard/scoreboard.h>`



Inherits the following classes: std::enable_shared_from_this< Scoreboard >


































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::unique\_ptr&lt; [**Objective**](classendstone_1_1Objective.md) &gt; | [**addObjective**](#function-addobjective-13) (std::string name, [**Criteria::Type**](classendstone_1_1Criteria.md#enum-type) criteria) = 0<br>_Registers an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual std::unique\_ptr&lt; [**Objective**](classendstone_1_1Objective.md) &gt; | [**addObjective**](#function-addobjective-23) (std::string name, [**Criteria::Type**](classendstone_1_1Criteria.md#enum-type) criteria, std::string display\_name) = 0<br>_Registers an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual std::unique\_ptr&lt; [**Objective**](classendstone_1_1Objective.md) &gt; | [**addObjective**](#function-addobjective-33) (std::string name, [**Criteria::Type**](classendstone_1_1Criteria.md#enum-type) criteria, std::string display\_name, RenderType render\_type) = 0<br>_Registers an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual void | [**clearSlot**](#function-clearslot) (DisplaySlot slot) = 0<br>_Clears any objective in the specified slot._  |
| virtual std::vector&lt; ScoreEntry &gt; | [**getEntries**](#function-getentries) () const = 0<br>_Gets all entries tracked by this_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual std::unique\_ptr&lt; [**Objective**](classendstone_1_1Objective.md) &gt; | [**getObjective**](#function-getobjective-12) (std::string name) const = 0<br>_Gets an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _by name._ |
| virtual std::unique\_ptr&lt; [**Objective**](classendstone_1_1Objective.md) &gt; | [**getObjective**](#function-getobjective-22) (DisplaySlot slot) const = 0<br>_Gets the_ [_**Objective**_](classendstone_1_1Objective.md) _currently displayed in a DisplaySlot on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual std::vector&lt; std::unique\_ptr&lt; [**Objective**](classendstone_1_1Objective.md) &gt; &gt; | [**getObjectives**](#function-getobjectives) () const = 0<br>_Gets all Objectives on this_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual std::vector&lt; std::unique\_ptr&lt; [**Objective**](classendstone_1_1Objective.md) &gt; &gt; | [**getObjectivesByCriteria**](#function-getobjectivesbycriteria) ([**Criteria::Type**](classendstone_1_1Criteria.md#enum-type) criteria) const = 0<br>_Gets all Objectives of a_ [_**Criteria**_](classendstone_1_1Criteria.md) _on the_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual std::vector&lt; std::unique\_ptr&lt; [**Score**](classendstone_1_1Score.md) &gt; &gt; | [**getScores**](#function-getscores) (ScoreEntry entry) const = 0<br>_Gets all scores for an entry on this_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual void | [**resetScores**](#function-resetscores) (ScoreEntry entry) = 0<br>_Removes all scores for an entry on this_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual  | [**~Scoreboard**](#function-scoreboard) () = default<br> |




























## Public Functions Documentation




### function addObjective [1/3]

_Registers an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual std::unique_ptr< Objective > endstone::Scoreboard::addObjective (
    std::string name,
    Criteria::Type criteria
) = 0
```





**Parameters:**


* `name` Name of the [**Objective**](classendstone_1_1Objective.md) 
* `criteria` [**Criteria**](classendstone_1_1Criteria.md) for the [**Objective**](classendstone_1_1Objective.md) 



**Returns:**

A reference to the newly registered [**Objective**](classendstone_1_1Objective.md), or `nullptr` if an objective by that name already exists. 





        

<hr>



### function addObjective [2/3]

_Registers an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual std::unique_ptr< Objective > endstone::Scoreboard::addObjective (
    std::string name,
    Criteria::Type criteria,
    std::string display_name
) = 0
```





**Parameters:**


* `name` Name of the [**Objective**](classendstone_1_1Objective.md) 
* `criteria` [**Criteria**](classendstone_1_1Criteria.md) type for the [**Objective**](classendstone_1_1Objective.md) 
* `display_name` Name displayed to players for the [**Objective**](classendstone_1_1Objective.md). 



**Returns:**

A reference to the newly registered [**Objective**](classendstone_1_1Objective.md), or `nullptr` if an objective by that name already exists. 





        

<hr>



### function addObjective [3/3]

_Registers an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual std::unique_ptr< Objective > endstone::Scoreboard::addObjective (
    std::string name,
    Criteria::Type criteria,
    std::string display_name,
    RenderType render_type
) = 0
```





**Parameters:**


* `name` Name of the [**Objective**](classendstone_1_1Objective.md) 
* `criteria` [**Criteria**](classendstone_1_1Criteria.md) type for the [**Objective**](classendstone_1_1Objective.md) 
* `display_name` Name displayed to players for the [**Objective**](classendstone_1_1Objective.md). 
* `render_type` Manner of rendering the [**Objective**](classendstone_1_1Objective.md) 



**Returns:**

A reference to the newly registered [**Objective**](classendstone_1_1Objective.md), or `nullptr` if an objective by that name already exists. 





        

<hr>



### function clearSlot 

_Clears any objective in the specified slot._ 
```C++
virtual void endstone::Scoreboard::clearSlot (
    DisplaySlot slot
) = 0
```





**Parameters:**


* `slot` the slot to remove objectives 




        

<hr>



### function getEntries 

_Gets all entries tracked by this_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual std::vector< ScoreEntry > endstone::Scoreboard::getEntries () const = 0
```





**Returns:**

A list of all tracked entries 





        

<hr>



### function getObjective [1/2]

_Gets an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _by name._
```C++
virtual std::unique_ptr< Objective > endstone::Scoreboard::getObjective (
    std::string name
) const = 0
```





**Parameters:**


* `name` Name of the [**Objective**](classendstone_1_1Objective.md) 



**Returns:**

the [**Objective**](classendstone_1_1Objective.md) or `nullptr` if it does not exist 





        

<hr>



### function getObjective [2/2]

_Gets the_ [_**Objective**_](classendstone_1_1Objective.md) _currently displayed in a DisplaySlot on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual std::unique_ptr< Objective > endstone::Scoreboard::getObjective (
    DisplaySlot slot
) const = 0
```





**Parameters:**


* `slot` The DisplaySlot 



**Returns:**

the [**Objective**](classendstone_1_1Objective.md) currently displayed or `nullptr` if nothing is displayed in that DisplaySlot 





        

<hr>



### function getObjectives 

_Gets all Objectives on this_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual std::vector< std::unique_ptr< Objective > > endstone::Scoreboard::getObjectives () const = 0
```





**Returns:**

A list of all Objectives on this [**Scoreboard**](classendstone_1_1Scoreboard.md) 





        

<hr>



### function getObjectivesByCriteria 

_Gets all Objectives of a_ [_**Criteria**_](classendstone_1_1Criteria.md) _on the_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual std::vector< std::unique_ptr< Objective > > endstone::Scoreboard::getObjectivesByCriteria (
    Criteria::Type criteria
) const = 0
```





**Parameters:**


* `criteria` [**Criteria**](classendstone_1_1Criteria.md) type to search by 



**Returns:**

A list of Objectives using the specified [**Criteria**](classendstone_1_1Criteria.md) 





        

<hr>



### function getScores 

_Gets all scores for an entry on this_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual std::vector< std::unique_ptr< Score > > endstone::Scoreboard::getScores (
    ScoreEntry entry
) const = 0
```





**Parameters:**


* `entry` the entry whose scores are being retrieved 



**Returns:**

a list of all scores tracked for the entry 





        

<hr>



### function resetScores 

_Removes all scores for an entry on this_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual void endstone::Scoreboard::resetScores (
    ScoreEntry entry
) = 0
```





**Parameters:**


* `entry` the entry to drop all current scores for 




        

<hr>



### function ~Scoreboard 

```C++
virtual endstone::Scoreboard::~Scoreboard () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/scoreboard/scoreboard.h`

