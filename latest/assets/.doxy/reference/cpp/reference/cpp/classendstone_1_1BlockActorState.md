

# Class endstone::BlockActorState



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockActorState**](classendstone_1_1BlockActorState.md)



_Represents a captured state of a block entity._ 

* `#include <endstone/block/block_actor_state.h>`



Inherits the following classes: [endstone::BlockState](classendstone_1_1BlockState.md)


Inherited by the following classes: [endstone::Campfire](classendstone_1_1Campfire.md),  [endstone::Container](classendstone_1_1Container.md),  [endstone::CreatureSpawner](classendstone_1_1CreatureSpawner.md),  [endstone::ItemFrame](classendstone_1_1ItemFrame.md),  [endstone::Sign](classendstone_1_1Sign.md)








































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isSnapshot**](#function-issnapshot) () const = 0<br>_Gets whether this state is backed by an independent block entity snapshot._  |
|   | [**~BlockActorState**](#function-blockactorstate) () override<br> |


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
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof) ([**ClassInfo**](classendstone_1_1ClassInfo.md) target) const = 0<br> |
| virtual  | [**~Object**](classendstone_1_1Object.md#function-object) () = default<br> |
















































































## Public Functions Documentation




### function isSnapshot 

_Gets whether this state is backed by an independent block entity snapshot._ 
```C++
virtual bool endstone::BlockActorState::isSnapshot () const = 0
```





**Returns:**

`true` if this state is a snapshot, otherwise `false`. 





        

<hr>



### function ~BlockActorState 

```C++
endstone::BlockActorState::~BlockActorState () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/block_actor_state.h`

