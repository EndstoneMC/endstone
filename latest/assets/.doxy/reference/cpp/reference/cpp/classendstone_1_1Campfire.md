

# Class endstone::Campfire



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Campfire**](classendstone_1_1Campfire.md)



_Represents a captured state of a campfire._ 

* `#include <endstone/block/campfire.h>`



Inherits the following classes: [endstone::BlockState](classendstone_1_1BlockState.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getCookTime**](#function-getcooktime) ([**int**](classendstone_1_1Identifier.md) index) const = 0<br>_Gets how long the item in the given slot has been cooking for._  |
| virtual std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getItem**](#function-getitem) ([**int**](classendstone_1_1Identifier.md) index) const = 0<br>_Gets the item currently cooking in the given slot._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getSize**](#function-getsize) () const = 0<br>_Gets the number of items this campfire can cook at once._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCookTime**](#function-setcooktime) ([**int**](classendstone_1_1Identifier.md) index, [**int**](classendstone_1_1Identifier.md) cook\_time) = 0<br>_Sets how long the item in the given slot has been cooking for._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setItem**](#function-setitem) ([**int**](classendstone_1_1Identifier.md) index, [**const**](classendstone_1_1Identifier.md) std::optional&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; & item) = 0<br>_Sets the item currently cooking in the given slot._  |


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




### function getCookTime 

_Gets how long the item in the given slot has been cooking for._ 
```C++
virtual int endstone::Campfire::getCookTime (
    int index
) const = 0
```





**Parameters:**


* `index` the slot, between 0 and `getSize()` - 1 



**Returns:**

the cook time, in ticks 





        

<hr>



### function getItem 

_Gets the item currently cooking in the given slot._ 
```C++
virtual std::optional< ItemStack > endstone::Campfire::getItem (
    int index
) const = 0
```





**Parameters:**


* `index` the slot, between 0 and `getSize()` - 1 



**Returns:**

the item, or `std::nullopt` if the slot is empty 





        

<hr>



### function getSize 

_Gets the number of items this campfire can cook at once._ 
```C++
virtual int endstone::Campfire::getSize () const = 0
```





**Returns:**

the number of slots 





        

<hr>



### function setCookTime 

_Sets how long the item in the given slot has been cooking for._ 
```C++
virtual void endstone::Campfire::setCookTime (
    int index,
    int cook_time
) = 0
```





**Parameters:**


* `index` the slot, between 0 and `getSize()` - 1 
* `cook_time` the cook time, in ticks 




        

<hr>



### function setItem 

_Sets the item currently cooking in the given slot._ 
```C++
virtual void endstone::Campfire::setItem (
    int index,
    const std::optional< ItemStack > & item
) = 0
```





**Parameters:**


* `index` the slot, between 0 and `getSize()` - 1 
* `item` the item, or `std::nullopt` to empty the slot 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/campfire.h`

