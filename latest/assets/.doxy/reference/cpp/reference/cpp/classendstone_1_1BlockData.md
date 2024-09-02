

# Class endstone::BlockData



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**BlockData**](classendstone_1_1BlockData.md)



_Represents the data related to a live block._ 

* `#include <endstone/block/block_data.h>`



Inherits the following classes: std::enable_shared_from_this< BlockData >


































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual BlockStates | [**getBlockStates**](#function-getblockstates) () const = 0<br>_Gets the block states, which when passed into a method such as Server::createBlockData(type, block\_states) will unambiguously recreate this instance._  |
| virtual std::string | [**getType**](#function-gettype) () const = 0<br>_Get the block type represented by this block data._  |
| virtual  | [**~BlockData**](#function-blockdata) () = default<br> |




























## Public Functions Documentation




### function getBlockStates 

_Gets the block states, which when passed into a method such as Server::createBlockData(type, block\_states) will unambiguously recreate this instance._ 
```C++
virtual BlockStates endstone::BlockData::getBlockStates () const = 0
```





**Returns:**

the block states for this block 





        

<hr>



### function getType 

_Get the block type represented by this block data._ 
```C++
virtual std::string endstone::BlockData::getType () const = 0
```





**Returns:**

the block type 





        

<hr>



### function ~BlockData 

```C++
virtual endstone::BlockData::~BlockData () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/block_data.h`

