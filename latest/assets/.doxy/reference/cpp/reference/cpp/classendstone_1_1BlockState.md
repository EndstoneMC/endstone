

# Class endstone::BlockState



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockState**](classendstone_1_1BlockState.md)



_Represents a captured state of a block, which will not update automatically._ [More...](#detailed-description)

* `#include <endstone/block/block_state.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlock**](#function-getblock) () const = 0<br>_Gets the block represented by this block state._  |
| virtual std::shared\_ptr&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; | [**getData**](#function-getdata) () const = 0<br>_Gets the data for this block state._  |
| virtual [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](#function-getdimension) () const = 0<br>_Gets the dimension which contains the block represented by this block state._  |
| virtual [**Location**](classendstone_1_1Location.md) | [**getLocation**](#function-getlocation) () const = 0<br>_Gets the location of this block state._  |
| virtual std::string | [**getType**](#function-gettype) () const = 0<br>_Gets the type of this block state._  |
| virtual int | [**getX**](#function-getx) () const = 0<br>_Gets the x-coordinate of this block state._  |
| virtual int | [**getY**](#function-gety) () const = 0<br>_Gets the y-coordinate of this block state._  |
| virtual int | [**getZ**](#function-getz) () const = 0<br>_Gets the z-coordinate of this block state._  |
| virtual void | [**setData**](#function-setdata) (std::shared\_ptr&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; data) = 0<br>_Sets the data for this block state._  |
| virtual void | [**setType**](#function-settype) (std::string type) = 0<br>_Sets the type of this block state._  |
| virtual bool | [**update**](#function-update-13) () = 0<br>_Attempts to update the block represented by this state, setting it to yhe new values as defined by this state._  |
| virtual bool | [**update**](#function-update-23) (bool force) = 0<br>_Attempts to update the block represented by this state, setting it to the new values as defined by this state._  |
| virtual bool | [**update**](#function-update-33) (bool force, bool apply\_physics) = 0<br>_Attempts to update the block represented by this state, setting it to the new values as defined by this state._  |
| virtual  | [**~BlockState**](#function-blockstate) () = default<br> |




























# Detailed Description


Unlike [**Block**](classendstone_1_1Block.md), which only one object can exist per coordinate, [**BlockState**](classendstone_1_1BlockState.md) can exist multiple times for any given [**Block**](classendstone_1_1Block.md). Note that another plugin may change the state of the block, and you will not know, or they may change the block to another type entirely, causing your [**BlockState**](classendstone_1_1BlockState.md) to become invalid. 


    
## Public Functions Documentation




### function getBlock 

_Gets the block represented by this block state._ 
```C++
virtual std::unique_ptr< Block > endstone::BlockState::getBlock () const = 0
```





**Returns:**

the block represented by this block state 




**Exception:**


* `IllegalStateException` if this block state is not placed 




        

<hr>



### function getData 

_Gets the data for this block state._ 
```C++
virtual std::shared_ptr< BlockData > endstone::BlockState::getData () const = 0
```





**Returns:**

block specific data 





        

<hr>



### function getDimension 

_Gets the dimension which contains the block represented by this block state._ 
```C++
virtual Dimension & endstone::BlockState::getDimension () const = 0
```





**Returns:**

the dimension containing the block represented by this block state 





        

<hr>



### function getLocation 

_Gets the location of this block state._ 
```C++
virtual Location endstone::BlockState::getLocation () const = 0
```





**Returns:**

the location 





        

<hr>



### function getType 

_Gets the type of this block state._ 
```C++
virtual std::string endstone::BlockState::getType () const = 0
```





**Returns:**

block type 





        

<hr>



### function getX 

_Gets the x-coordinate of this block state._ 
```C++
virtual int endstone::BlockState::getX () const = 0
```





**Returns:**

x-coordinate 





        

<hr>



### function getY 

_Gets the y-coordinate of this block state._ 
```C++
virtual int endstone::BlockState::getY () const = 0
```





**Returns:**

y-coordinate 





        

<hr>



### function getZ 

_Gets the z-coordinate of this block state._ 
```C++
virtual int endstone::BlockState::getZ () const = 0
```





**Returns:**

z-coordinate 





        

<hr>



### function setData 

_Sets the data for this block state._ 
```C++
virtual void endstone::BlockState::setData (
    std::shared_ptr< BlockData > data
) = 0
```





**Parameters:**


* `data` New block specific data 




        

<hr>



### function setType 

_Sets the type of this block state._ 
```C++
virtual void endstone::BlockState::setType (
    std::string type
) = 0
```





**Parameters:**


* `type` [**Block**](classendstone_1_1Block.md) type to change this block state to 




        

<hr>



### function update [1/3]

_Attempts to update the block represented by this state, setting it to yhe new values as defined by this state._ 
```C++
virtual bool endstone::BlockState::update () = 0
```



This has the same effect as calling update(false). That is to say, this will not modify the state of a block if it is no longer the same type as it was when this state was taken. It will return false in this eventuality.




**Returns:**

true if the update was successful, otherwise false 





        

<hr>



### function update [2/3]

_Attempts to update the block represented by this state, setting it to the new values as defined by this state._ 
```C++
virtual bool endstone::BlockState::update (
    bool force
) = 0
```



This has the same effect as calling update(force, true). That is to say, this will trigger a physics update to surrounding blocks.




**Parameters:**


* `force` true to forcefully set the state 



**Returns:**

true if the update was successful, otherwise false 





        

<hr>



### function update [3/3]

_Attempts to update the block represented by this state, setting it to the new values as defined by this state._ 
```C++
virtual bool endstone::BlockState::update (
    bool force,
    bool apply_physics
) = 0
```



Unless force is true, this will not modify the state of a block if it is no longer the same type as it was when this state was taken. It will return false in this eventuality. 


If force is true, it will set the type of the block to match the new state, set the state data and then return true. 


If apply\_physics is true, it will trigger a physics update on surrounding blocks which could cause them to update or disappear.




**Parameters:**


* `force` true to forcefully set the state 
* `apply_physics` false to cancel updating physics on surrounding blocks 



**Returns:**

true if the update was successful, otherwise false 





        

<hr>



### function ~BlockState 

```C++
virtual endstone::BlockState::~BlockState () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/block_state.h`

