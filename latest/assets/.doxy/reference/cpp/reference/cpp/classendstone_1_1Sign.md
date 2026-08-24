

# Class endstone::Sign



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Sign**](classendstone_1_1Sign.md)



_Represents a captured state of a sign._ 

* `#include <endstone/block/sign.h>`



Inherits the following classes: [endstone::BlockState](classendstone_1_1BlockState.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Side**](#enum-side)  <br>_Represents a side of a sign._  |




























































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**SignSide**](classendstone_1_1SignSide.md) & | [**getSide**](#function-getside) ([**Side**](classendstone_1_1Sign.md#enum-side) side) const = 0<br>_Gets the side of this sign._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isWaxed**](#function-iswaxed) () const = 0<br>_Gets whether this sign is waxed._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setWaxed**](#function-setwaxed) ([**bool**](classendstone_1_1Identifier.md) waxed) = 0<br>_Sets whether this sign is waxed._  |


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
















































































## Public Types Documentation




### enum Side 

_Represents a side of a sign._ 
```C++
enum endstone::Sign::Side {
    Front = 0,
    Back = 1
};
```




<hr>
## Public Functions Documentation




### function getSide 

_Gets the side of this sign._ 
```C++
virtual SignSide & endstone::Sign::getSide (
    Side side
) const = 0
```





**Parameters:**


* `side` the side of the sign 



**Returns:**

the given side of the sign 





        

<hr>



### function isWaxed 

_Gets whether this sign is waxed._ 
```C++
virtual bool endstone::Sign::isWaxed () const = 0
```



A waxed sign cannot be edited by players.




**Returns:**

`true` if this sign is waxed 





        

<hr>



### function setWaxed 

_Sets whether this sign is waxed._ 
```C++
virtual void endstone::Sign::setWaxed (
    bool waxed
) = 0
```





**Parameters:**


* `waxed` whether this sign is waxed 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/sign.h`

