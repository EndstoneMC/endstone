

# Class endstone::Dimension



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Dimension**](classendstone_1_1Dimension.md)



_Represents a dimension within a_ [_**Level**_](classendstone_1_1Level.md) _._

* `#include <endstone/level/dimension.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br>_Represents various dimension types._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlockAt**](#function-getblockat-12) (int x, int y, int z) const = 0<br>_Gets the_ [_**Block**_](classendstone_1_1Block.md) _at the given coordinates._ |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlockAt**](#function-getblockat-22) ([**Location**](classendstone_1_1Location.md) location) const = 0<br>_Gets the_ [_**Block**_](classendstone_1_1Block.md) _at the given_[_**Location**_](classendstone_1_1Location.md) _._ |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getHighestBlockAt**](#function-gethighestblockat-12) (int x, int z) const = 0<br>_Gets the highest non-empty (impassable) block at the given coordinates._  |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getHighestBlockAt**](#function-gethighestblockat-22) ([**Location**](classendstone_1_1Location.md) location) const = 0<br>_Gets the highest non-empty (impassable) block at the given_ [_**Location**_](classendstone_1_1Location.md) _._ |
| virtual int | [**getHighestBlockYAt**](#function-gethighestblockyat) (int x, int z) const = 0<br>_Gets the highest non-empty (impassable) coordinate at the given coordinates._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](#function-getlevel) () const = 0<br>_Gets the level to which this dimension belongs._  |
| virtual std::vector&lt; std::unique\_ptr&lt; [**Chunk**](classendstone_1_1Chunk.md) &gt; &gt; | [**getLoadedChunks**](#function-getloadedchunks) () = 0<br>_Gets a list of all loaded Chunks._  |
| virtual std::string | [**getName**](#function-getname) () const = 0<br>_Gets the name of this dimension._  |
| virtual [**Type**](classendstone_1_1Dimension.md#enum-type) | [**getType**](#function-gettype) () const = 0<br>_Gets the type of this dimension._  |
| virtual  | [**~Dimension**](#function-dimension) () = default<br> |




























## Public Types Documentation




### enum Type 

_Represents various dimension types._ 
```C++
enum endstone::Dimension::Type {
    Overworld = 0,
    Nether = 1,
    TheEnd = 2,
    Custom = 999
};
```




<hr>
## Public Functions Documentation




### function getBlockAt [1/2]

_Gets the_ [_**Block**_](classendstone_1_1Block.md) _at the given coordinates._
```C++
virtual std::unique_ptr< Block > endstone::Dimension::getBlockAt (
    int x,
    int y,
    int z
) const = 0
```





**Parameters:**


* `x` X-coordinate of the block 
* `y` Y-coordinate of the block 
* `z` Z-coordinate of the block 



**Returns:**

[**Block**](classendstone_1_1Block.md) at the given coordinates 





        

<hr>



### function getBlockAt [2/2]

_Gets the_ [_**Block**_](classendstone_1_1Block.md) _at the given_[_**Location**_](classendstone_1_1Location.md) _._
```C++
virtual std::unique_ptr< Block > endstone::Dimension::getBlockAt (
    Location location
) const = 0
```





**Parameters:**


* `location` [**Location**](classendstone_1_1Location.md) of the block 



**Returns:**

[**Block**](classendstone_1_1Block.md) at the given coordinates 





        

<hr>



### function getHighestBlockAt [1/2]

_Gets the highest non-empty (impassable) block at the given coordinates._ 
```C++
virtual std::unique_ptr< Block > endstone::Dimension::getHighestBlockAt (
    int x,
    int z
) const = 0
```





**Parameters:**


* `x` X-coordinate of the block 
* `z` Z-coordinate of the block 



**Returns:**

Highest non-empty block 





        

<hr>



### function getHighestBlockAt [2/2]

_Gets the highest non-empty (impassable) block at the given_ [_**Location**_](classendstone_1_1Location.md) _._
```C++
virtual std::unique_ptr< Block > endstone::Dimension::getHighestBlockAt (
    Location location
) const = 0
```





**Parameters:**


* `location` Coordinates to get the highest block 



**Returns:**

Highest non-empty block 





        

<hr>



### function getHighestBlockYAt 

_Gets the highest non-empty (impassable) coordinate at the given coordinates._ 
```C++
virtual int endstone::Dimension::getHighestBlockYAt (
    int x,
    int z
) const = 0
```





**Parameters:**


* `x` X-coordinate of the blocks 
* `z` Z-coordinate of the blocks 



**Returns:**

Y-coordinate of the highest non-empty block 





        

<hr>



### function getLevel 

_Gets the level to which this dimension belongs._ 
```C++
virtual Level & endstone::Dimension::getLevel () const = 0
```





**Returns:**

[**Level**](classendstone_1_1Level.md) containing this dimension. 





        

<hr>



### function getLoadedChunks 

_Gets a list of all loaded Chunks._ 
```C++
virtual std::vector< std::unique_ptr< Chunk > > endstone::Dimension::getLoadedChunks () = 0
```





**Returns:**

All loaded chunks 





        

<hr>



### function getName 

_Gets the name of this dimension._ 
```C++
virtual std::string endstone::Dimension::getName () const = 0
```





**Returns:**

Name of this dimension 





        

<hr>



### function getType 

_Gets the type of this dimension._ 
```C++
virtual Type endstone::Dimension::getType () const = 0
```





**Returns:**

Type of this dimension 





        

<hr>



### function ~Dimension 

```C++
virtual endstone::Dimension::~Dimension () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/level/dimension.h`

