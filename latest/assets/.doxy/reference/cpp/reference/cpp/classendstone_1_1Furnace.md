

# Class endstone::Furnace



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Furnace**](classendstone_1_1Furnace.md)



_Represents a captured state of a furnace._ 

* `#include <endstone/block/furnace.h>`



Inherits the following classes: [endstone::Container](classendstone_1_1Container.md)






























































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getBurnTime**](#function-getburntime) () const = 0<br>_Gets the burn time._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getCookTime**](#function-getcooktime) () const = 0<br>_Gets the cook time._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setBurnTime**](#function-setburntime) ([**int**](classendstone_1_1Identifier.md) burn\_time) = 0<br>_Sets the burn time._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCookTime**](#function-setcooktime) ([**int**](classendstone_1_1Identifier.md) cook\_time) = 0<br>_Sets the cook time._  |


## Public Functions inherited from endstone::Container

See [endstone::Container](classendstone_1_1Container.md)

| Type | Name |
| ---: | :--- |
| virtual [**Inventory**](classendstone_1_1Inventory.md) & | [**getInventory**](classendstone_1_1Container.md#function-getinventory) () const = 0<br>_Gets the inventory of the block represented by this block state._  |


## Public Functions inherited from endstone::BlockState

See [endstone::BlockState](classendstone_1_1BlockState.md)

| Type | Name |
| ---: | :--- |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlock**](classendstone_1_1BlockState.md#function-getblock) () const = 0<br>_Gets the block represented by this block state._  |
| virtual std::unique\_ptr&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; | [**getData**](classendstone_1_1BlockState.md#function-getdata) () const = 0<br>_Gets the data for this block state._  |
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
| virtual [**const**](classendstone_1_1Identifier.md) std::type\_info & | [**getClassTypeId**](classendstone_1_1Object.md#function-getclasstypeid) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**is**](classendstone_1_1Object.md#function-is) () const<br>_Checks if this object is an instance of the given type T (or a subclass of T)._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof) ([**const**](classendstone_1_1Identifier.md) std::type\_info & target) const = 0<br> |
| virtual  | [**~Object**](classendstone_1_1Object.md#function-object) () = default<br> |










































































































## Public Functions Documentation




### function getBurnTime 

_Gets the burn time._ 
```C++
virtual int endstone::Furnace::getBurnTime () const = 0
```



This is the number of ticks the current fuel keeps the furnace lit for.




**Returns:**

the burn time 





        

<hr>



### function getCookTime 

_Gets the cook time._ 
```C++
virtual int endstone::Furnace::getCookTime () const = 0
```



This is the amount of time the item has been cooking for.




**Returns:**

the cook time 





        

<hr>



### function setBurnTime 

_Sets the burn time._ 
```C++
virtual void endstone::Furnace::setBurnTime (
    int burn_time
) = 0
```



A burn time greater than 0 will cause this block to be lit, whilst a time less than 0 will extinguish it.




**Parameters:**


* `burn_time` the burn time 




        

<hr>



### function setCookTime 

_Sets the cook time._ 
```C++
virtual void endstone::Furnace::setCookTime (
    int cook_time
) = 0
```



This is the amount of time the item has been cooking for.




**Parameters:**


* `cook_time` the cook time 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/furnace.h`

