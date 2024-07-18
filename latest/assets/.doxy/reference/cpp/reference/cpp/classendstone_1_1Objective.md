

# Class endstone::Objective



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Objective**](classendstone_1_1Objective.md)



_Represents an objective on a scoreboard that can show scores specific to entries._ 

* `#include <endstone/scoreboard/objective.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::string | [**getDisplayName**](#function-getdisplayname) () const = 0<br>_Gets the name displayed to players for this objective._  |
| virtual std::string | [**getName**](#function-getname) () const = 0<br>_Gets the name of this_ [_**Objective**_](classendstone_1_1Objective.md) _._ |
| virtual bool | [**isValid**](#function-isvalid) () const = 0<br>_Determines if the_ [_**Objective**_](classendstone_1_1Objective.md) _is valid._ |
| virtual  | [**~Objective**](#function-objective) () = default<br> |




























## Public Functions Documentation




### function getDisplayName 

_Gets the name displayed to players for this objective._ 
```C++
virtual std::string endstone::Objective::getDisplayName () const = 0
```





**Returns:**

this objective's display name 





        

<hr>



### function getName 

_Gets the name of this_ [_**Objective**_](classendstone_1_1Objective.md) _._
```C++
virtual std::string endstone::Objective::getName () const = 0
```





**Returns:**

this objective's name 





        

<hr>



### function isValid 

_Determines if the_ [_**Objective**_](classendstone_1_1Objective.md) _is valid._
```C++
virtual bool endstone::Objective::isValid () const = 0
```





**Returns:**

true if the [**Objective**](classendstone_1_1Objective.md) is valid, false otherwise 





        

<hr>



### function ~Objective 

```C++
virtual endstone::Objective::~Objective () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/scoreboard/objective.h`

