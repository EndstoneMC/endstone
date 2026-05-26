

# Class endstone::Dimension



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Dimension**](classendstone_1_1Dimension.md)



_Represents a dimension within a_ [_**Level**_](classendstone_1_1Level.md) _._

* `#include <endstone/level/dimension.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Nether**](#variable-nether)   = `DimensionId::minecraft("nether")`<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Overworld**](#variable-overworld)   = `DimensionId::minecraft("overworld")`<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**TheEnd**](#variable-theend)   = `DimensionId::minecraft("the\_end")`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Item**](classendstone_1_1Item.md) & | [**dropItem**](#function-dropitem) ([**Location**](classendstone_1_1Location.md) location, [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & item) = 0<br>_Drops an item at the specified_ [_**Location**_](classendstone_1_1Location.md) _._ |
| virtual std::vector&lt; [**Actor**](classendstone_1_1Actor.md) \* &gt; | [**getActors**](#function-getactors) () const = 0<br>_Get a list of all actors in this dimension._  |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlockAt**](#function-getblockat-12) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) y, [**int**](classendstone_1_1Identifier.md) z) const = 0<br>_Gets the_ [_**Block**_](classendstone_1_1Block.md) _at the given coordinates._ |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlockAt**](#function-getblockat-22) ([**Location**](classendstone_1_1Location.md) location) const = 0<br>_Gets the_ [_**Block**_](classendstone_1_1Block.md) _at the given_[_**Location**_](classendstone_1_1Location.md) _._ |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getHighestBlockAt**](#function-gethighestblockat-12) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z) const = 0<br>_Gets the highest non-empty (impassable) block at the given coordinates._  |
| virtual std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getHighestBlockAt**](#function-gethighestblockat-22) ([**Location**](classendstone_1_1Location.md) location) const = 0<br>_Gets the highest non-empty (impassable) block at the given_ [_**Location**_](classendstone_1_1Location.md) _._ |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getHighestBlockYAt**](#function-gethighestblockyat) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z) const = 0<br>_Gets the highest non-empty (impassable) coordinate at the given coordinates._  |
| virtual [**DimensionId**](classendstone_1_1Identifier.md) | [**getId**](#function-getid) () const = 0<br>_Return the identifier of this dimension._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](#function-getlevel) () const = 0<br>_Gets the level to which this dimension belongs._  |
| virtual std::vector&lt; std::unique\_ptr&lt; [**Chunk**](classendstone_1_1Chunk.md) &gt; &gt; | [**getLoadedChunks**](#function-getloadedchunks) () = 0<br>_Gets a list of all loaded Chunks._  |
| virtual std::string | [**getTranslationKey**](#function-gettranslationkey) () const = 0<br>_Get the translation key, suitable for use in a translation component._  |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**spawnActor**](#function-spawnactor) ([**Location**](classendstone_1_1Location.md) location, [**ActorTypeId**](classendstone_1_1Identifier.md) type) = 0<br>_Creates an actor at the given_ [_**Location**_](classendstone_1_1Location.md) _._ |
| virtual  | [**~Dimension**](#function-dimension) () = default<br> |




























## Public Static Attributes Documentation




### variable Nether 

```C++
constexpr auto endstone::Dimension::Nether;
```




<hr>



### variable Overworld 

```C++
constexpr auto endstone::Dimension::Overworld;
```




<hr>



### variable TheEnd 

```C++
constexpr auto endstone::Dimension::TheEnd;
```




<hr>
## Public Functions Documentation




### function dropItem 

_Drops an item at the specified_ [_**Location**_](classendstone_1_1Location.md) _._
```C++
virtual Item & endstone::Dimension::dropItem (
    Location location,
    const  ItemStack & item
) = 0
```





**Parameters:**


* `location` [**Location**](classendstone_1_1Location.md) to drop the item 
* `item` [**ItemStack**](classendstone_1_1ItemStack.md) to drop



**Returns:**

[**Item**](classendstone_1_1Item.md) entity created as a result of this method 





        

<hr>



### function getActors 

_Get a list of all actors in this dimension._ 
```C++
virtual std::vector< Actor * > endstone::Dimension::getActors () const = 0
```





**Returns:**

A List of all actors currently residing in this dimension 





        

<hr>



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



### function getId 

_Return the identifier of this dimension._ 
```C++
virtual DimensionId endstone::Dimension::getId () const = 0
```





**Returns:**

this dimension's identifier 





        

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



### function getTranslationKey 

_Get the translation key, suitable for use in a translation component._ 
```C++
virtual std::string endstone::Dimension::getTranslationKey () const = 0
```





**Returns:**

the translation key 





        

<hr>



### function spawnActor 

_Creates an actor at the given_ [_**Location**_](classendstone_1_1Location.md) _._
```C++
virtual Actor * endstone::Dimension::spawnActor (
    Location location,
    ActorTypeId type
) = 0
```





**Parameters:**


* `location` The location to spawn the actor 
* `type` The actor type to spawn 



**Returns:**

Resulting [**Actor**](classendstone_1_1Actor.md) of this method 





        

<hr>



### function ~Dimension 

```C++
virtual endstone::Dimension::~Dimension () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/level/dimension.h`

