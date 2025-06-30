

# Class endstone::Level



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Level**](classendstone_1_1Level.md)



_Represents a level, which may contain actors, chunks and blocks._ 

* `#include <endstone/level/level.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::vector&lt; [**Actor**](classendstone_1_1Actor.md) \* &gt; | [**getActors**](#function-getactors) () const = 0<br>_Get a list of all actors in this level._  |
| virtual [**Dimension**](classendstone_1_1Dimension.md) \* | [**getDimension**](#function-getdimension) (std::string name) const = 0<br>_Gets the dimension with the given name._  |
| virtual std::vector&lt; [**Dimension**](classendstone_1_1Dimension.md) \* &gt; | [**getDimensions**](#function-getdimensions) () const = 0<br>_Gets a list of all dimensions within this level._  |
| virtual std::string | [**getName**](#function-getname) () const = 0<br>_Gets the unique name of this level._  |
| virtual int | [**getTime**](#function-gettime) () const = 0<br>_Gets the relative in-game time of this level._  |
| virtual void | [**setTime**](#function-settime) (int time) = 0<br>_Sets the relative in-game time on the server._  |
| virtual  | [**~Level**](#function-level) () = default<br> |




























## Public Functions Documentation




### function getActors 

_Get a list of all actors in this level._ 
```C++
virtual std::vector< Actor * > endstone::Level::getActors () const = 0
```





**Returns:**

A List of all actors currently residing in this level 





        

<hr>



### function getDimension 

_Gets the dimension with the given name._ 
```C++
virtual Dimension * endstone::Level::getDimension (
    std::string name
) const = 0
```





**Parameters:**


* `name` the name of the dimension to retrieve 



**Returns:**

The [**Dimension**](classendstone_1_1Dimension.md) with the given name, or nullptr if none exists 





        

<hr>



### function getDimensions 

_Gets a list of all dimensions within this level._ 
```C++
virtual std::vector< Dimension * > endstone::Level::getDimensions () const = 0
```





**Returns:**

a list of dimensions 





        

<hr>



### function getName 

_Gets the unique name of this level._ 
```C++
virtual std::string endstone::Level::getName () const = 0
```





**Returns:**

Name of this level 





        

<hr>



### function getTime 

_Gets the relative in-game time of this level._ 
```C++
virtual int endstone::Level::getTime () const = 0
```





**Returns:**

The current relative time 





        

<hr>



### function setTime 

_Sets the relative in-game time on the server._ 
```C++
virtual void endstone::Level::setTime (
    int time
) = 0
```





**Parameters:**


* `time` The new relative time to set the in-game time to 




        

<hr>



### function ~Level 

```C++
virtual endstone::Level::~Level () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/level/level.h`

