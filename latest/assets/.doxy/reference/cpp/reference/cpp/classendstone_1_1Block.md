

# Class endstone::Block



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Block**](classendstone_1_1Block.md)



_Represents a block._ [More...](#detailed-description)

* `#include <endstone/block/block.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::string | [**getType**](#function-gettype) () const = 0<br>_Get the type of the block._  |
| virtual  | [**~Block**](#function-block) () = default<br> |




























# Detailed Description


This is a live object, and only one [**Block**](classendstone_1_1Block.md) may exist for any given location in a world. 


    
## Public Functions Documentation




### function getType 

_Get the type of the block._ 
```C++
virtual std::string endstone::Block::getType () const = 0
```



This method returns the type of the block as a string, for example, minecraft:acacia\_stairs.




**Returns:**

The type of the block. 





        

<hr>



### function ~Block 

```C++
virtual endstone::Block::~Block () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/block.h`

