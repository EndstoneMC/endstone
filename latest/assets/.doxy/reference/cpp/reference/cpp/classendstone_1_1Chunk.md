

# Class endstone::Chunk



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Chunk**](classendstone_1_1Chunk.md)



_Represents a chunk of blocks._ 

* `#include <endstone/level/chunk.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](#function-getdimension) () const = 0<br>_Gets the dimension containing this chunk._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](#function-getlevel) () const = 0<br>_Gets the level containing this chunk._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getX**](#function-getx) () const = 0<br>_Gets the X-coordinate of this chunk._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getZ**](#function-getz) () const = 0<br>_Gets the Z-coordinate of this chunk._  |
| virtual  | [**~Chunk**](#function-chunk) () = default<br> |




























## Public Functions Documentation




### function getDimension 

_Gets the dimension containing this chunk._ 
```C++
virtual Dimension & endstone::Chunk::getDimension () const = 0
```





**Returns:**

Parent [**Dimension**](classendstone_1_1Dimension.md) 





        

<hr>



### function getLevel 

_Gets the level containing this chunk._ 
```C++
virtual Level & endstone::Chunk::getLevel () const = 0
```





**Returns:**

Parent [**Level**](classendstone_1_1Level.md) 





        

<hr>



### function getX 

_Gets the X-coordinate of this chunk._ 
```C++
virtual int endstone::Chunk::getX () const = 0
```





**Returns:**

X-coordinate 





        

<hr>



### function getZ 

_Gets the Z-coordinate of this chunk._ 
```C++
virtual int endstone::Chunk::getZ () const = 0
```





**Returns:**

Z-coordinate 





        

<hr>



### function ~Chunk 

```C++
virtual endstone::Chunk::~Chunk () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/level/chunk.h`

