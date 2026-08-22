

# Class endstone::Chunk



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Chunk**](classendstone_1_1Chunk.md)



_Represents a chunk of blocks._ 

* `#include <endstone/level/chunk.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**addPluginChunkTicket**](#function-addpluginchunkticket) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Adds a plugin ticket for this chunk, loading it if it is not already loaded._  |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Dimension**](classendstone_1_1Dimension.md) &gt; | [**getDimension**](#function-getdimension) () const = 0<br>_Gets the dimension containing this chunk._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](#function-getlevel) () const = 0<br>_Gets the level containing this chunk._  |
| virtual std::vector&lt; [**Plugin**](classendstone_1_1Plugin.md) \* &gt; | [**getPluginChunkTickets**](#function-getpluginchunktickets) () const = 0<br>_Gets which plugins hold a ticket for this chunk._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getX**](#function-getx) () const = 0<br>_Gets the X-coordinate of this chunk._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getZ**](#function-getz) () const = 0<br>_Gets the Z-coordinate of this chunk._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isLoaded**](#function-isloaded) () const = 0<br>_Checks if this chunk is loaded._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**load**](#function-load-12) () = 0<br>_Requests this chunk to be loaded, generating it if it does not exist yet._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**load**](#function-load-22) ([**bool**](classendstone_1_1Identifier.md) generate) = 0<br>_Requests this chunk to be loaded, and keeps it resident until it is released again._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**removePluginChunkTicket**](#function-removepluginchunkticket) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Removes the given plugin's ticket for this chunk._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**unload**](#function-unload) () = 0<br>_Releases the hold that_ `load()` _placed on this chunk, and unloads it if nothing else keeps it resident._ |
| virtual  | [**~Chunk**](#function-chunk) () = default<br> |




























## Public Functions Documentation




### function addPluginChunkTicket 

_Adds a plugin ticket for this chunk, loading it if it is not already loaded._ 
```C++
virtual bool endstone::Chunk::addPluginChunkTicket (
    Plugin & plugin
) = 0
```





**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) taking the ticket 



**Returns:**

`true` if a plugin ticket was added, `false` if the plugin already holds one for this chunk 





        

<hr>



### function getDimension 

_Gets the dimension containing this chunk._ 
```C++
virtual NotNull < Dimension > endstone::Chunk::getDimension () const = 0
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



### function getPluginChunkTickets 

_Gets which plugins hold a ticket for this chunk._ 
```C++
virtual std::vector< Plugin * > endstone::Chunk::getPluginChunkTickets () const = 0
```



The returned list is a snapshot; it does not track tickets added or removed afterwards.




**Returns:**

Plugins holding a ticket for this chunk 





        

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



### function isLoaded 

_Checks if this chunk is loaded._ 
```C++
virtual bool endstone::Chunk::isLoaded () const = 0
```





**Returns:**

`true` if the chunk is loaded, otherwise `false` 





        

<hr>



### function load [1/2]

_Requests this chunk to be loaded, generating it if it does not exist yet._ 
```C++
virtual bool endstone::Chunk::load () = 0
```



Equivalent to `load(true)`.




**Returns:**

`true` if the chunk is now held resident, otherwise `false` 





        

<hr>



### function load [2/2]

_Requests this chunk to be loaded, and keeps it resident until it is released again._ 
```C++
virtual bool endstone::Chunk::load (
    bool generate
) = 0
```





**See also:** [**Dimension::loadChunk()**](classendstone_1_1Dimension.md#function-loadchunk-12) for how the hold behaves.


**Parameters:**


* `generate` Whether to generate the chunk if it does not exist yet 



**Returns:**

`false` if `generate` is `false` and the chunk has not been generated, otherwise `true` 





        

<hr>



### function removePluginChunkTicket 

_Removes the given plugin's ticket for this chunk._ 
```C++
virtual bool endstone::Chunk::removePluginChunkTicket (
    Plugin & plugin
) = 0
```





**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) whose ticket to remove 



**Returns:**

`true` if a plugin ticket was removed, `false` if the plugin holds none for this chunk 





        

<hr>



### function unload 

_Releases the hold that_ `load()` _placed on this chunk, and unloads it if nothing else keeps it resident._
```C++
virtual bool endstone::Chunk::unload () = 0
```





**Returns:**

`true` if the chunk is no longer loaded, otherwise `false` 





        

<hr>



### function ~Chunk 

```C++
virtual endstone::Chunk::~Chunk () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/level/chunk.h`

