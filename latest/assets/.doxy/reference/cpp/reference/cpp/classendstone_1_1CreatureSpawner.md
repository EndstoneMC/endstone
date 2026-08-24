

# Class endstone::CreatureSpawner



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CreatureSpawner**](classendstone_1_1CreatureSpawner.md)



_Represents a captured state of a creature spawner._ 

* `#include <endstone/block/creature_spawner.h>`



Inherits the following classes: [endstone::BlockState](classendstone_1_1BlockState.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getDelay**](#function-getdelay) () const = 0<br>_Gets the delay until the spawner spawns the next batch of actors._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getMaxNearbyEntities**](#function-getmaxnearbyentities) () const = 0<br>_Gets the maximum number of similar actors allowed nearby before the spawner stops spawning._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getMaxSpawnDelay**](#function-getmaxspawndelay) () const = 0<br>_Gets the maximum delay the spawner will wait between spawns._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getMinSpawnDelay**](#function-getminspawndelay) () const = 0<br>_Gets the minimum delay the spawner will wait between spawns._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getRequiredPlayerRange**](#function-getrequiredplayerrange) () const = 0<br>_Gets how far away a player must be for the spawner to be active._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getSpawnCount**](#function-getspawncount) () const = 0<br>_Gets how many actors the spawner attempts to spawn at a time._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getSpawnRange**](#function-getspawnrange) () const = 0<br>_Gets the radius around the spawner in which actors are spawned._  |
| virtual [**const**](classendstone_1_1Identifier.md) [**ActorType**](classendstone_1_1ActorType.md) & | [**getSpawnedType**](#function-getspawnedtype) () const = 0<br>_Gets the type of actor this spawner will spawn._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setDelay**](#function-setdelay) ([**int**](classendstone_1_1Identifier.md) delay) = 0<br>_Sets the delay until the spawner spawns the next batch of actors._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setMaxNearbyEntities**](#function-setmaxnearbyentities) ([**int**](classendstone_1_1Identifier.md) count) = 0<br>_Sets the maximum number of similar actors allowed nearby before the spawner stops spawning._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setMaxSpawnDelay**](#function-setmaxspawndelay) ([**int**](classendstone_1_1Identifier.md) delay) = 0<br>_Sets the maximum delay the spawner will wait between spawns._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setMinSpawnDelay**](#function-setminspawndelay) ([**int**](classendstone_1_1Identifier.md) delay) = 0<br>_Sets the minimum delay the spawner will wait between spawns._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setRequiredPlayerRange**](#function-setrequiredplayerrange) ([**int**](classendstone_1_1Identifier.md) range) = 0<br>_Sets how far away a player must be for the spawner to be active._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setSpawnCount**](#function-setspawncount) ([**int**](classendstone_1_1Identifier.md) count) = 0<br>_Sets how many actors the spawner attempts to spawn at a time._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setSpawnRange**](#function-setspawnrange) ([**int**](classendstone_1_1Identifier.md) range) = 0<br>_Sets the radius around the spawner in which actors are spawned._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setSpawnedType**](#function-setspawnedtype) ([**ActorTypeId**](classendstone_1_1Identifier.md) type) = 0<br>_Sets the type of actor this spawner will spawn._  |


## Public Functions inherited from endstone::BlockState

See [endstone::BlockState](classendstone_1_1BlockState.md)

| Type | Name |
| ---: | :--- |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlock**](classendstone_1_1BlockState.md#function-getblock) () const = 0<br>_Gets the block represented by this block state._  |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; | [**getData**](classendstone_1_1BlockState.md#function-getdata) () const = 0<br>_Gets the data for this block state._  |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; | [**getDimension**](classendstone_1_1BlockState.md#function-getdimension) () const = 0<br>_Gets the dimension which contains the block represented by this block state._  |
| virtual [**Location**](classendstone_1_1Location.md) | [**getLocation**](classendstone_1_1BlockState.md#function-getlocation) () const = 0<br>_Gets the location of this block state._  |
| virtual [**const**](classendstone_1_1Identifier.md) [**BlockType**](classendstone_1_1BlockType.md) & | [**getType**](classendstone_1_1BlockState.md#function-gettype) () const = 0<br>_Gets the type of this block state._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getX**](classendstone_1_1BlockState.md#function-getx) () const = 0<br>_Gets the x-coordinate of this block state._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getY**](classendstone_1_1BlockState.md#function-gety) () const = 0<br>_Gets the y-coordinate of this block state._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getZ**](classendstone_1_1BlockState.md#function-getz) () const = 0<br>_Gets the z-coordinate of this block state._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setData**](classendstone_1_1BlockState.md#function-setdata) ([**const**](classendstone_1_1Identifier.md) [**BlockData**](classendstone_1_1BlockData.md) & data) = 0<br>_Sets the data for this block state._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setType**](classendstone_1_1BlockState.md#function-settype) ([**BlockTypeId**](classendstone_1_1Identifier.md) type) = 0<br>_Sets the type of this block state._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**update**](classendstone_1_1BlockState.md#function-update-13) () = 0<br>_Attempts to update the block represented by this state, setting it to yhe new values as defined by this state._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**update**](classendstone_1_1BlockState.md#function-update-23) ([**bool**](classendstone_1_1Identifier.md) force) = 0<br>_Attempts to update the block represented by this state, setting it to the new values as defined by this state._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**update**](classendstone_1_1BlockState.md#function-update-33) ([**bool**](classendstone_1_1Identifier.md) force, [**bool**](classendstone_1_1Identifier.md) apply\_physics) = 0<br>_Attempts to update the block represented by this state, setting it to the new values as defined by this state._  |
|   | [**~BlockState**](classendstone_1_1BlockState.md#function-blockstate) () override<br> |


## Public Functions inherited from endstone::Object

See [endstone::Object](classendstone_1_1Object.md)

| Type | Name |
| ---: | :--- |
|  [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1Object.md#function-as-12) () <br>_Attempts to cast this object to the given type T._  |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1Object.md#function-as-22) () const<br>_Attempts to cast this object to the given type T._  |
| virtual [**ClassInfo**](classendstone_1_1ClassInfo.md) | [**getClassInfo**](classendstone_1_1Object.md#function-getclassinfo) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**is**](classendstone_1_1Object.md#function-is) () const<br>_Checks if this object is an instance of the given type T (or a subclass of T)._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof-12) ([**ClassInfo**](classendstone_1_1ClassInfo.md) target) const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof-22) () const<br> |
| virtual  | [**~Object**](classendstone_1_1Object.md#function-object) () = default<br> |
















































































## Public Functions Documentation




### function getDelay 

_Gets the delay until the spawner spawns the next batch of actors._ 
```C++
virtual int endstone::CreatureSpawner::getDelay () const = 0
```





**Returns:**

the delay, in ticks 





        

<hr>



### function getMaxNearbyEntities 

_Gets the maximum number of similar actors allowed nearby before the spawner stops spawning._ 
```C++
virtual int endstone::CreatureSpawner::getMaxNearbyEntities () const = 0
```





**Returns:**

the maximum number of similar actors 





        

<hr>



### function getMaxSpawnDelay 

_Gets the maximum delay the spawner will wait between spawns._ 
```C++
virtual int endstone::CreatureSpawner::getMaxSpawnDelay () const = 0
```





**Returns:**

the maximum delay, in ticks 





        

<hr>



### function getMinSpawnDelay 

_Gets the minimum delay the spawner will wait between spawns._ 
```C++
virtual int endstone::CreatureSpawner::getMinSpawnDelay () const = 0
```





**Returns:**

the minimum delay, in ticks 





        

<hr>



### function getRequiredPlayerRange 

_Gets how far away a player must be for the spawner to be active._ 
```C++
virtual int endstone::CreatureSpawner::getRequiredPlayerRange () const = 0
```





**Returns:**

the required player range, in blocks 





        

<hr>



### function getSpawnCount 

_Gets how many actors the spawner attempts to spawn at a time._ 
```C++
virtual int endstone::CreatureSpawner::getSpawnCount () const = 0
```





**Returns:**

the number of actors 





        

<hr>



### function getSpawnRange 

_Gets the radius around the spawner in which actors are spawned._ 
```C++
virtual int endstone::CreatureSpawner::getSpawnRange () const = 0
```





**Returns:**

the spawn range, in blocks 





        

<hr>



### function getSpawnedType 

_Gets the type of actor this spawner will spawn._ 
```C++
virtual const  ActorType & endstone::CreatureSpawner::getSpawnedType () const = 0
```





**Returns:**

the type of actor 





        

<hr>



### function setDelay 

_Sets the delay until the spawner spawns the next batch of actors._ 
```C++
virtual void endstone::CreatureSpawner::setDelay (
    int delay
) = 0
```





**Parameters:**


* `delay` the delay, in ticks 




        

<hr>



### function setMaxNearbyEntities 

_Sets the maximum number of similar actors allowed nearby before the spawner stops spawning._ 
```C++
virtual void endstone::CreatureSpawner::setMaxNearbyEntities (
    int count
) = 0
```





**Parameters:**


* `count` the maximum number of similar actors 




        

<hr>



### function setMaxSpawnDelay 

_Sets the maximum delay the spawner will wait between spawns._ 
```C++
virtual void endstone::CreatureSpawner::setMaxSpawnDelay (
    int delay
) = 0
```





**Parameters:**


* `delay` the maximum delay, in ticks 




        

<hr>



### function setMinSpawnDelay 

_Sets the minimum delay the spawner will wait between spawns._ 
```C++
virtual void endstone::CreatureSpawner::setMinSpawnDelay (
    int delay
) = 0
```





**Parameters:**


* `delay` the minimum delay, in ticks 




        

<hr>



### function setRequiredPlayerRange 

_Sets how far away a player must be for the spawner to be active._ 
```C++
virtual void endstone::CreatureSpawner::setRequiredPlayerRange (
    int range
) = 0
```





**Parameters:**


* `range` the required player range, in blocks 




        

<hr>



### function setSpawnCount 

_Sets how many actors the spawner attempts to spawn at a time._ 
```C++
virtual void endstone::CreatureSpawner::setSpawnCount (
    int count
) = 0
```





**Parameters:**


* `count` the number of actors 




        

<hr>



### function setSpawnRange 

_Sets the radius around the spawner in which actors are spawned._ 
```C++
virtual void endstone::CreatureSpawner::setSpawnRange (
    int range
) = 0
```





**Parameters:**


* `range` the spawn range, in blocks 




        

<hr>



### function setSpawnedType 

_Sets the type of actor this spawner will spawn._ 
```C++
virtual void endstone::CreatureSpawner::setSpawnedType (
    ActorTypeId type
) = 0
```





**Parameters:**


* `type` the type of actor (e.g. `minecraft:zombie`) 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/creature_spawner.h`

