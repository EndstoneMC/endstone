

# Class endstone::Block



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Block**](classendstone_1_1Block.md)



_Represents a block._ [More...](#detailed-description)

* `#include <endstone/block/block.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::shared\_ptr&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; | [**getData**](#function-getdata) () const = 0<br>_Gets the complete block data for this block._  |
| virtual [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](#function-getdimension) () const = 0<br>_Gets the dimension which contains this_ [_**Block**_](classendstone_1_1Block.md) _._ |
| virtual [**Location**](classendstone_1_1Location.md) | [**getLocation**](#function-getlocation) () const = 0<br>_Gets the_ [_**Location**_](classendstone_1_1Location.md) _of the block._ |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getRelative**](#function-getrelative-13) (int offset\_x, int offset\_y, int offset\_z) = 0<br>_Gets the block at the given offsets._  |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getRelative**](#function-getrelative-23) (BlockFace face) = 0<br>_Gets the block at the given face._  |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getRelative**](#function-getrelative-33) (BlockFace face, int distance) = 0<br>_Gets the block at the given distance of the given face._  |
| virtual std::string | [**getType**](#function-gettype) () const = 0<br>_Get the type of the block._  |
| virtual int | [**getX**](#function-getx) () const = 0<br>_Gets the x-coordinate of this block._  |
| virtual int | [**getY**](#function-gety) () const = 0<br>_Gets the y-coordinate of this block._  |
| virtual int | [**getZ**](#function-getz) () const = 0<br>_Gets the z-coordinate of this block._  |
| virtual bool | [**isValid**](#function-isvalid) () const = 0<br>_Checks if the block is valid._  |
| virtual void | [**setData**](#function-setdata-12) (std::shared\_ptr&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; data) = 0<br>_Sets the complete data for this block._  |
| virtual void | [**setData**](#function-setdata-22) (std::shared\_ptr&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; data, bool apply\_physics) = 0<br>_Sets the complete data for this block._  |
| virtual void | [**setType**](#function-settype-12) (std::string type) = 0<br>_Sets the type of this block._  |
| virtual void | [**setType**](#function-settype-22) (std::string type, bool apply\_physics) = 0<br>_Sets the type of this block._  |
| virtual  | [**~Block**](#function-block) () = default<br> |




























# Detailed Description


This is a live object, and only one [**Block**](classendstone_1_1Block.md) may exist for any given location in a dimension. 


    
## Public Functions Documentation




### function getData 

_Gets the complete block data for this block._ 
```C++
virtual std::shared_ptr< BlockData > endstone::Block::getData () const = 0
```





**Returns:**

block specific data 





        

<hr>



### function getDimension 

_Gets the dimension which contains this_ [_**Block**_](classendstone_1_1Block.md) _._
```C++
virtual Dimension & endstone::Block::getDimension () const = 0
```





**Returns:**

[**Dimension**](classendstone_1_1Dimension.md) containing this block 





        

<hr>



### function getLocation 

_Gets the_ [_**Location**_](classendstone_1_1Location.md) _of the block._
```C++
virtual Location endstone::Block::getLocation () const = 0
```





**Returns:**

[**Location**](classendstone_1_1Location.md) of block 





        

<hr>



### function getRelative [1/3]

_Gets the block at the given offsets._ 
```C++
virtual std::unique_ptr< Block > endstone::Block::getRelative (
    int offset_x,
    int offset_y,
    int offset_z
) = 0
```





**Parameters:**


* `offset_x` X-coordinate offset 
* `offset_y` Y-coordinate offset 
* `offset_z` Z-coordinate offset 



**Returns:**

[**Block**](classendstone_1_1Block.md) at the given offsets 





        

<hr>



### function getRelative [2/3]

_Gets the block at the given face._ 
```C++
virtual std::unique_ptr< Block > endstone::Block::getRelative (
    BlockFace face
) = 0
```



This method is equal to getRelative(face, 1)




**Parameters:**


* `face` Face of this block to return 



**Returns:**

[**Block**](classendstone_1_1Block.md) at the given face 





        

<hr>



### function getRelative [3/3]

_Gets the block at the given distance of the given face._ 
```C++
virtual std::unique_ptr< Block > endstone::Block::getRelative (
    BlockFace face,
    int distance
) = 0
```





**Parameters:**


* `face` Face of this block to return 
* `distance` Distance to get the block at 



**Returns:**

[**Block**](classendstone_1_1Block.md) at the given face 





        

<hr>



### function getType 

_Get the type of the block._ 
```C++
virtual std::string endstone::Block::getType () const = 0
```



This method returns the type of the block as a string, for example, minecraft:acacia\_stairs.




**Returns:**

The type of the block. 





        

<hr>



### function getX 

_Gets the x-coordinate of this block._ 
```C++
virtual int endstone::Block::getX () const = 0
```





**Returns:**

x-coordinate 





        

<hr>



### function getY 

_Gets the y-coordinate of this block._ 
```C++
virtual int endstone::Block::getY () const = 0
```





**Returns:**

x-coordinate 





        

<hr>



### function getZ 

_Gets the z-coordinate of this block._ 
```C++
virtual int endstone::Block::getZ () const = 0
```





**Returns:**

x-coordinate 





        

<hr>



### function isValid 

_Checks if the block is valid._ 
```C++
virtual bool endstone::Block::isValid () const = 0
```




 This method verifies the validity of the block. A block is considered valid if and only if the chunk containing this block is loaded and ticking. If the chunk is not loaded or not in the ticking state, this method will return false.




**Returns:**

true if the block is valid (i.e., the containing chunk is loaded and ticking), otherwise false. 





        

<hr>



### function setData [1/2]

_Sets the complete data for this block._ 
```C++
virtual void endstone::Block::setData (
    std::shared_ptr< BlockData > data
) = 0
```





**Parameters:**


* `data` new block specific data 




        

<hr>



### function setData [2/2]

_Sets the complete data for this block._ 
```C++
virtual void endstone::Block::setData (
    std::shared_ptr< BlockData > data,
    bool apply_physics
) = 0
```





**Parameters:**


* `data` new block specific data 
* `apply_physics` False to cancel physics on the changed block. 




        

<hr>



### function setType [1/2]

_Sets the type of this block._ 
```C++
virtual void endstone::Block::setType (
    std::string type
) = 0
```





**Parameters:**


* `type` Material to change this block to 




        

<hr>



### function setType [2/2]

_Sets the type of this block._ 
```C++
virtual void endstone::Block::setType (
    std::string type,
    bool apply_physics
) = 0
```





**Parameters:**


* `type` Material to change this block to 
* `apply_physics` False to cancel physics on the changed block. 




        

<hr>



### function ~Block 

```C++
virtual endstone::Block::~Block () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/block.h`

