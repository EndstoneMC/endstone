

# Class endstone::Level



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Level**](classendstone_1_1Level.md)



_Represents a level, which may contain actors, chunks and blocks._ 

* `#include <endstone/level/level.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual GameRuleValue | [**\_getGameRule**](#function-_getgamerule) ([**Identifier**](classendstone_1_1Identifier.md)&lt; [**GameRule**](classendstone_1_1GameRule.md) &gt; rule) const = 0<br>_Gets the value of a game rule._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**\_hasGameRule**](#function-_hasgamerule) ([**Identifier**](classendstone_1_1Identifier.md)&lt; [**GameRule**](classendstone_1_1GameRule.md) &gt; rule) const = 0<br>_Checks if a game rule exists._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**\_setGameRule**](#function-_setgamerule) ([**Identifier**](classendstone_1_1Identifier.md)&lt; [**GameRule**](classendstone_1_1GameRule.md) &gt; rule, GameRuleValue value) = 0<br>_Sets the value of a game rule._  |
| virtual [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; | [**createDimension**](#function-createdimension) ([**const**](classendstone_1_1Identifier.md) [**DimensionCreator**](classendstone_1_1DimensionCreator.md) & creator) = 0<br>_Creates a new custom dimension within this level._  |
| virtual std::vector&lt; [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; &gt; | [**getActors**](#function-getactors) () const = 0<br>_Get a list of all actors in this level._  |
| virtual [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; | [**getDimension**](#function-getdimension) ([**DimensionId**](classendstone_1_1Identifier.md) id) const = 0<br>_Gets the dimension with the given id._  |
| virtual std::vector&lt; [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; &gt; | [**getDimensions**](#function-getdimensions) () const = 0<br>_Gets a list of all dimensions within this level._  |
|  [**T**](classendstone_1_1Identifier.md) | [**getGameRule**](#function-getgamerule) ([**GameRuleId**](classendstone_1_1GameRuleId.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; rule) const<br>_Gets the value of a game rule._  |
| virtual std::string | [**getName**](#function-getname) () const = 0<br>_Gets the unique name of this level._  |
| virtual std::int64\_t | [**getSeed**](#function-getseed) () const = 0<br>_Gets the Seed for this level._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getTime**](#function-gettime) () const = 0<br>_Gets the relative in-game time of this level._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**hasGameRule**](#function-hasgamerule) ([**GameRuleId**](classendstone_1_1GameRuleId.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; rule) const<br>_Checks if a game rule exists._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**setGameRule**](#function-setgamerule) ([**GameRuleId**](classendstone_1_1GameRuleId.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; rule, [**T**](classendstone_1_1Identifier.md) value) <br>_Sets the value of a game rule._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setTime**](#function-settime) ([**int**](classendstone_1_1Identifier.md) time) = 0<br>_Sets the relative in-game time on the server._  |
| virtual  | [**~Level**](#function-level) () = default<br> |




























## Public Functions Documentation




### function \_getGameRule 

_Gets the value of a game rule._ 
```C++
virtual GameRuleValue endstone::Level::_getGameRule (
    Identifier < GameRule > rule
) const = 0
```





**Parameters:**


* `rule` The Minecraft game rule to get 



**Returns:**

The current game rule value 





        

<hr>



### function \_hasGameRule 

_Checks if a game rule exists._ 
```C++
virtual bool endstone::Level::_hasGameRule (
    Identifier < GameRule > rule
) const = 0
```





**Parameters:**


* `rule` The Minecraft game rule to check 



**Returns:**

True if the game rule exists 





        

<hr>



### function \_setGameRule 

_Sets the value of a game rule._ 
```C++
virtual bool endstone::Level::_setGameRule (
    Identifier < GameRule > rule,
    GameRuleValue value
) = 0
```





**Parameters:**


* `rule` The Minecraft game rule to set 
* `value` The new value 



**Returns:**

True if the value was accepted 





        

<hr>



### function createDimension 

_Creates a new custom dimension within this level._ 
```C++
virtual Nullable < Dimension > endstone::Level::createDimension (
    const  DimensionCreator & creator
) = 0
```



The dimension is created from the options described by the given [**DimensionCreator**](classendstone_1_1DimensionCreator.md). Custom dimensions are empty (void) dimensions identified by a namespaced id, e.g. `myplugin:void_realm`; populate them with blocks, structures or actors afterward. Custom dimensions persist across server restarts. 


If a dimension with the requested name already exists, that existing dimension is returned instead.




**Parameters:**


* `creator` the options to use when creating the dimension 



**Returns:**

the newly created (or existing) [**Dimension**](classendstone_1_1Dimension.md), or null if it could not be created 





        

<hr>



### function getActors 

_Get a list of all actors in this level._ 
```C++
virtual std::vector< NotNull < Actor > > endstone::Level::getActors () const = 0
```





**Returns:**

A List of all actors currently residing in this level 





        

<hr>



### function getDimension 

_Gets the dimension with the given id._ 
```C++
virtual Nullable < Dimension > endstone::Level::getDimension (
    DimensionId id
) const = 0
```





**Parameters:**


* `id` the id of the dimension to retrieve.



**Returns:**

The [**Dimension**](classendstone_1_1Dimension.md) with the given id, or null if none exists 





        

<hr>



### function getDimensions 

_Gets a list of all dimensions within this level._ 
```C++
virtual std::vector< NotNull < Dimension > > endstone::Level::getDimensions () const = 0
```





**Returns:**

a list of dimensions 





        

<hr>



### function getGameRule 

_Gets the value of a game rule._ 
```C++
template<typename  T>
inline T endstone::Level::getGameRule (
    GameRuleId < T > rule
) const
```





**Template parameters:**


* `T` The type of the game rule's value. 



**Parameters:**


* `rule` The Minecraft game rule to get 



**Returns:**

The current game rule value 





        

<hr>



### function getName 

_Gets the unique name of this level._ 
```C++
virtual std::string endstone::Level::getName () const = 0
```





**Returns:**

Name of this level 





        

<hr>



### function getSeed 

_Gets the Seed for this level._ 
```C++
virtual std::int64_t endstone::Level::getSeed () const = 0
```





**Returns:**

This level's Seed 





        

<hr>



### function getTime 

_Gets the relative in-game time of this level._ 
```C++
virtual int endstone::Level::getTime () const = 0
```





**Returns:**

The current relative time 





        

<hr>



### function hasGameRule 

_Checks if a game rule exists._ 
```C++
template<typename  T>
inline bool endstone::Level::hasGameRule (
    GameRuleId < T > rule
) const
```





**Template parameters:**


* `T` The type of the game rule's value. 



**Parameters:**


* `rule` The Minecraft game rule to check 



**Returns:**

True if the game rule exists 





        

<hr>



### function setGameRule 

_Sets the value of a game rule._ 
```C++
template<typename  T>
inline bool endstone::Level::setGameRule (
    GameRuleId < T > rule,
    T value
) 
```





**Template parameters:**


* `T` The type of the game rule's value. 



**Parameters:**


* `rule` The Minecraft game rule to set 
* `value` The new value 



**Returns:**

True if the value was accepted 





        

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

