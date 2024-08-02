

# Class endstone::Block



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Block**](classendstone_1_1Block.md)



_Represents a block._ [More...](#detailed-description)

* `#include <endstone/block/block.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::unique\_ptr&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; | [**getData**](#function-getdata) () = 0<br>_Gets the complete block data for this block._  |
| virtual  | [**~Block**](#function-block) () = default<br> |




























# Detailed Description


This is a live object, and only one [**Block**](classendstone_1_1Block.md) may exist for any given location in a world. The state of the block may change concurrently to your own handling of it. Use block.getState() to get a snapshot state of a block which will not be modified. 


    
## Public Functions Documentation




### function getData 

_Gets the complete block data for this block._ 
```C++
virtual std::unique_ptr< BlockData > endstone::Block::getData () = 0
```





**Returns:**

block specific data 





        

<hr>



### function ~Block 

```C++
virtual endstone::Block::~Block () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/block.h`

