

# Class endstone::Scoreboard



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Scoreboard**](classendstone_1_1Scoreboard.md)



_Represents a scoreboard._ 

* `#include <endstone/scoreboard/scoreboard.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Objective**](classendstone_1_1Objective.md) & | [**addObjective**](#function-addobjective-12) (std::string name, Criteria criteria) = 0<br>_Registers an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual [**Objective**](classendstone_1_1Objective.md) & | [**addObjective**](#function-addobjective-22) (std::string name, Criteria criteria, std::string display\_name) = 0<br>_Registers an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual void | [**clearSlot**](#function-clearslot) (DisplaySlot slot) = 0<br>_Clears any objective in the specified slot._  |
| virtual std::unordered\_set&lt; std::string &gt; | [**getEntries**](#function-getentries) () const = 0<br>_Gets all entries tracked by this_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual [**Objective**](classendstone_1_1Objective.md) \* | [**getObjective**](#function-getobjective-12) (std::string name) const = 0<br>_Gets an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _by name._ |
| virtual [**Objective**](classendstone_1_1Objective.md) \* | [**getObjective**](#function-getobjective-22) (DisplaySlot slot) const = 0<br>_Gets the_ [_**Objective**_](classendstone_1_1Objective.md) _currently displayed in a DisplaySlot on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual std::unordered\_set&lt; [**Objective**](classendstone_1_1Objective.md) \* &gt; | [**getObjectives**](#function-getobjectives) () const = 0<br>_Gets all Objectives on this_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual std::unordered\_set&lt; [**Objective**](classendstone_1_1Objective.md) \* &gt; | [**getObjectivesByCriteria**](#function-getobjectivesbycriteria) (Criteria criteria) const = 0<br>_Gets all Objectives of a Criteria on the_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._ |
| virtual  | [**~Scoreboard**](#function-scoreboard) () = default<br> |




























## Public Functions Documentation




### function addObjective [1/2]

_Registers an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual Objective & endstone::Scoreboard::addObjective (
    std::string name,
    Criteria criteria
) = 0
```





**Parameters:**


* `name` Name of the [**Objective**](classendstone_1_1Objective.md) 
* `criteria` Criteria for the [**Objective**](classendstone_1_1Objective.md) 



**Returns:**

A reference to the newly registered [**Objective**](classendstone_1_1Objective.md). If an [**Objective**](classendstone_1_1Objective.md) with the given name already exists, a reference to that existing [**Objective**](classendstone_1_1Objective.md) is returned instead. 





        

<hr>



### function addObjective [2/2]

_Registers an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual Objective & endstone::Scoreboard::addObjective (
    std::string name,
    Criteria criteria,
    std::string display_name
) = 0
```





**Parameters:**


* `name` Name of the [**Objective**](classendstone_1_1Objective.md) 
* `criteria` Criteria for the [**Objective**](classendstone_1_1Objective.md) 
* `display_name` Name displayed to players for the [**Objective**](classendstone_1_1Objective.md). 



**Returns:**

A reference to the newly registered [**Objective**](classendstone_1_1Objective.md). If an [**Objective**](classendstone_1_1Objective.md) with the given name already exists, a reference to that existing [**Objective**](classendstone_1_1Objective.md) is returned instead. 





        

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
virtual std::unordered_set< std::string > endstone::Scoreboard::getEntries () const = 0
```





**Returns:**

A copy of set of all tracked entries 





        

<hr>



### function getObjective [1/2]

_Gets an_ [_**Objective**_](classendstone_1_1Objective.md) _on this_[_**Scoreboard**_](classendstone_1_1Scoreboard.md) _by name._
```C++
virtual Objective * endstone::Scoreboard::getObjective (
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
virtual Objective * endstone::Scoreboard::getObjective (
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
virtual std::unordered_set< Objective * > endstone::Scoreboard::getObjectives () const = 0
```





**Returns:**

A copy of set of all Objectives on this [**Scoreboard**](classendstone_1_1Scoreboard.md) 





        

<hr>



### function getObjectivesByCriteria 

_Gets all Objectives of a Criteria on the_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _._
```C++
virtual std::unordered_set< Objective * > endstone::Scoreboard::getObjectivesByCriteria (
    Criteria criteria
) const = 0
```





**Parameters:**


* `criteria` Criteria to search by 



**Returns:**

A copy of set of Objectives using the specified Criteria 





        

<hr>



### function ~Scoreboard 

```C++
virtual endstone::Scoreboard::~Scoreboard () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/scoreboard/scoreboard.h`

