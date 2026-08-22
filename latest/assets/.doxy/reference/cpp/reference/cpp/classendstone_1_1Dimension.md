

# Class endstone::Dimension



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Dimension**](classendstone_1_1Dimension.md)



_Represents a dimension within a_ [_**Level**_](classendstone_1_1Level.md) _._

* `#include <endstone/level/dimension.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Nether**](#variable-nether)   = `[**DimensionId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("nether")`<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Overworld**](#variable-overworld)   = `[**DimensionId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("overworld")`<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**TheEnd**](#variable-theend)   = `[**DimensionId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("the\_end")`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**addPluginChunkTicket**](#function-addpluginchunkticket) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z, [**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Adds a plugin ticket for the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates, loading it if it is not already loaded._ |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Item**](classendstone_1_1Item.md) &gt; | [**dropItem**](#function-dropitem) ([**Location**](classendstone_1_1Location.md) location, [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & item) = 0<br>_Drops an item at the specified_ [_**Location**_](classendstone_1_1Location.md) _._ |
| virtual std::vector&lt; [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; &gt; | [**getActors**](#function-getactors) () const = 0<br>_Get a list of all actors in this dimension._  |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlockAt**](#function-getblockat-12) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) y, [**int**](classendstone_1_1Identifier.md) z) const = 0<br>_Gets the_ [_**Block**_](classendstone_1_1Block.md) _at the given coordinates._ |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlockAt**](#function-getblockat-22) ([**Location**](classendstone_1_1Location.md) location) const = 0<br>_Gets the_ [_**Block**_](classendstone_1_1Block.md) _at the given_[_**Location**_](classendstone_1_1Location.md) _._ |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getHighestBlockAt**](#function-gethighestblockat-12) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z) const = 0<br>_Gets the highest non-empty (impassable) block at the given coordinates._  |
| virtual [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getHighestBlockAt**](#function-gethighestblockat-22) ([**Location**](classendstone_1_1Location.md) location) const = 0<br>_Gets the highest non-empty (impassable) block at the given_ [_**Location**_](classendstone_1_1Location.md) _._ |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getHighestBlockYAt**](#function-gethighestblockyat) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z) const = 0<br>_Gets the highest non-empty (impassable) coordinate at the given coordinates._  |
| virtual [**DimensionId**](classendstone_1_1Identifier.md) | [**getId**](#function-getid) () const = 0<br>_Return the identifier of this dimension._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](#function-getlevel) () const = 0<br>_Gets the level to which this dimension belongs._  |
| virtual std::vector&lt; [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Chunk**](classendstone_1_1Chunk.md) &gt; &gt; | [**getLoadedChunks**](#function-getloadedchunks) () = 0<br>_Gets a list of all loaded Chunks._  |
| virtual std::vector&lt; [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Mob**](classendstone_1_1Mob.md) &gt; &gt; | [**getMobs**](#function-getmobs) () const = 0<br>_Get a list of all mobs in this dimension._  |
| virtual std::vector&lt; [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; &gt; | [**getPlayers**](#function-getplayers) () const = 0<br>_Get a list of all players in this dimension._  |
| virtual std::vector&lt; [**Plugin**](classendstone_1_1Plugin.md) \* &gt; | [**getPluginChunkTickets**](#function-getpluginchunktickets-12) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z) const = 0<br>_Gets which plugins hold a ticket for the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates._ |
| virtual std::unordered\_map&lt; [**Plugin**](classendstone_1_1Plugin.md) \*, std::vector&lt; [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Chunk**](classendstone_1_1Chunk.md) &gt; &gt; &gt; | [**getPluginChunkTickets**](#function-getpluginchunktickets-22) () const = 0<br>_Gets which plugins hold tickets for which Chunks in this dimension._  |
| virtual std::string | [**getTranslationKey**](#function-gettranslationkey) () const = 0<br>_Get the translation key, suitable for use in a translation component._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isChunkGenerated**](#function-ischunkgenerated) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z) const = 0<br>_Checks if the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates has been generated._ |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isChunkLoaded**](#function-ischunkloaded) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z) const = 0<br>_Checks if the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates is loaded._ |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isValid**](#function-isvalid) () const = 0<br>_Checks whether this dimension is still valid (loaded)._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**loadChunk**](#function-loadchunk-12) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z) = 0<br>_Requests the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates to be loaded, generating it if it does not exist yet._ |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**loadChunk**](#function-loadchunk-22) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z, [**bool**](classendstone_1_1Identifier.md) generate) = 0<br>_Requests the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates to be loaded, and keeps it resident until it is released again._ |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**removePluginChunkTicket**](#function-removepluginchunkticket) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z, [**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Removes the given plugin's ticket for the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**removePluginChunkTickets**](#function-removepluginchunktickets) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Removes every ticket the given plugin holds in this dimension._  |
| virtual [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; | [**spawnActor**](#function-spawnactor) ([**Location**](classendstone_1_1Location.md) location, [**ActorTypeId**](classendstone_1_1Identifier.md) type) = 0<br>_Creates an actor at the given_ [_**Location**_](classendstone_1_1Location.md) _._ |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**unloadChunk**](#function-unloadchunk) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z) = 0<br>_Releases the hold that_ `loadChunk()` _placed on the_[_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates, and unloads it if nothing else keeps it resident._ |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**unloadChunkRequest**](#function-unloadchunkrequest) ([**int**](classendstone_1_1Identifier.md) x, [**int**](classendstone_1_1Identifier.md) z) = 0<br>_Releases the hold that_ `loadChunk()` _placed on the_[_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates, without unloading it now._ |
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




### function addPluginChunkTicket 

_Adds a plugin ticket for the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates, loading it if it is not already loaded._
```C++
virtual bool endstone::Dimension::addPluginChunkTicket (
    int x,
    int z,
    Plugin & plugin
) = 0
```



A plugin ticket keeps the chunk resident until it is explicitly removed or the owning plugin is disabled. A plugin may only have one ticket per chunk, but each chunk can have multiple plugin tickets. `unloadChunk()` does not remove plugin tickets.




**Parameters:**


* `x` X-coordinate of the chunk 
* `z` Z-coordinate of the chunk 
* `plugin` [**Plugin**](classendstone_1_1Plugin.md) taking the ticket 



**Returns:**

`true` if a plugin ticket was added, `false` if the plugin already holds one for this chunk 





        

<hr>



### function dropItem 

_Drops an item at the specified_ [_**Location**_](classendstone_1_1Location.md) _._
```C++
virtual NotNull < Item > endstone::Dimension::dropItem (
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
virtual std::vector< NotNull < Actor > > endstone::Dimension::getActors () const = 0
```





**Returns:**

A List of all actors currently residing in this dimension 





        

<hr>



### function getBlockAt [1/2]

_Gets the_ [_**Block**_](classendstone_1_1Block.md) _at the given coordinates._
```C++
virtual NotNull < Block > endstone::Dimension::getBlockAt (
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
virtual NotNull < Block > endstone::Dimension::getBlockAt (
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
virtual NotNull < Block > endstone::Dimension::getHighestBlockAt (
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
virtual NotNull < Block > endstone::Dimension::getHighestBlockAt (
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
virtual std::vector< NotNull < Chunk > > endstone::Dimension::getLoadedChunks () = 0
```





**Returns:**

All loaded chunks 





        

<hr>



### function getMobs 

_Get a list of all mobs in this dimension._ 
```C++
virtual std::vector< NotNull < Mob > > endstone::Dimension::getMobs () const = 0
```





**Returns:**

A List of all mobs currently residing in this dimension 





        

<hr>



### function getPlayers 

_Get a list of all players in this dimension._ 
```C++
virtual std::vector< NotNull < Player > > endstone::Dimension::getPlayers () const = 0
```





**Returns:**

A List of all players currently residing in this dimension 





        

<hr>



### function getPluginChunkTickets [1/2]

_Gets which plugins hold a ticket for the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates._
```C++
virtual std::vector< Plugin * > endstone::Dimension::getPluginChunkTickets (
    int x,
    int z
) const = 0
```



The returned list is a snapshot; it does not track tickets added or removed afterwards.




**Parameters:**


* `x` X-coordinate of the chunk 
* `z` Z-coordinate of the chunk 



**Returns:**

Plugins holding a ticket for the chunk 





        

<hr>



### function getPluginChunkTickets [2/2]

_Gets which plugins hold tickets for which Chunks in this dimension._ 
```C++
virtual std::unordered_map< Plugin *, std::vector< NotNull < Chunk > > > endstone::Dimension::getPluginChunkTickets () const = 0
```



The returned map is a snapshot; it does not track tickets added or removed afterwards. A plugin holding no tickets is absent from it.




**Returns:**

Chunks each plugin holds a ticket for 





        

<hr>



### function getTranslationKey 

_Get the translation key, suitable for use in a translation component._ 
```C++
virtual std::string endstone::Dimension::getTranslationKey () const = 0
```





**Returns:**

the translation key 





        

<hr>



### function isChunkGenerated 

_Checks if the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates has been generated._
```C++
virtual bool endstone::Dimension::isChunkGenerated (
    int x,
    int z
) const = 0
```



A chunk counts as generated once it is loaded or has been written to the level's chunk storage.




**Parameters:**


* `x` X-coordinate of the chunk 
* `z` Z-coordinate of the chunk 



**Returns:**

`true` if the chunk has been generated, otherwise `false` 





        

<hr>



### function isChunkLoaded 

_Checks if the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates is loaded._
```C++
virtual bool endstone::Dimension::isChunkLoaded (
    int x,
    int z
) const = 0
```





**Parameters:**


* `x` X-coordinate of the chunk 
* `z` Z-coordinate of the chunk 



**Returns:**

`true` if the chunk is loaded, otherwise `false` 





        

<hr>



### function isValid 

_Checks whether this dimension is still valid (loaded)._ 
```C++
virtual bool endstone::Dimension::isValid () const = 0
```



A [**Dimension**](classendstone_1_1Dimension.md) handle can outlive the underlying dimension if it is unloaded; this returns false once that happens, after which accessing the dimension throws.




**Returns:**

`true` if the dimension is still loaded, `false` otherwise 





        

<hr>



### function loadChunk [1/2]

_Requests the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates to be loaded, generating it if it does not exist yet._
```C++
virtual bool endstone::Dimension::loadChunk (
    int x,
    int z
) = 0
```



Equivalent to `loadChunk(x, z, true)`.




**Parameters:**


* `x` X-coordinate of the chunk 
* `z` Z-coordinate of the chunk 



**Returns:**

`true` if the chunk is now held resident, otherwise `false` 





        

<hr>



### function loadChunk [2/2]

_Requests the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates to be loaded, and keeps it resident until it is released again._
```C++
virtual bool endstone::Dimension::loadChunk (
    int x,
    int z,
    bool generate
) = 0
```



The chunk is held from the moment this returns until `unloadChunk()` or `unloadChunkRequest()` releases it, or the server restarts. Bedrock has no synchronous chunk load, so unless the chunk was already resident the load finishes on a later tick and `isChunkLoaded()` may still report `false` right afterwards. A chunk held this way stays in memory but is not ticked, and the hold never expires on its own. Intended for keeping a handful of chunks resident; it is not suited to loading large regions.


The hold is not attributed to any plugin and survives that plugin being disabled. Use `addPluginChunkTicket()` for a hold that is released automatically.




**Parameters:**


* `x` X-coordinate of the chunk 
* `z` Z-coordinate of the chunk 
* `generate` Whether to generate the chunk if it does not exist yet 



**Returns:**

`false` if `generate` is `false` and the chunk has not been generated, or if the coordinates lie outside the world limit, otherwise `true` 





        

<hr>



### function removePluginChunkTicket 

_Removes the given plugin's ticket for the_ [_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates._
```C++
virtual bool endstone::Dimension::removePluginChunkTicket (
    int x,
    int z,
    Plugin & plugin
) = 0
```





**Parameters:**


* `x` X-coordinate of the chunk 
* `z` Z-coordinate of the chunk 
* `plugin` [**Plugin**](classendstone_1_1Plugin.md) whose ticket to remove 



**Returns:**

`true` if a plugin ticket was removed, `false` if the plugin holds none for this chunk 





        

<hr>



### function removePluginChunkTickets 

_Removes every ticket the given plugin holds in this dimension._ 
```C++
virtual void endstone::Dimension::removePluginChunkTickets (
    Plugin & plugin
) = 0
```





**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) whose tickets to remove 




        

<hr>



### function spawnActor 

_Creates an actor at the given_ [_**Location**_](classendstone_1_1Location.md) _._
```C++
virtual Nullable < Actor > endstone::Dimension::spawnActor (
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



### function unloadChunk 

_Releases the hold that_ `loadChunk()` _placed on the_[_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates, and unloads it if nothing else keeps it resident._
```C++
virtual bool endstone::Dimension::unloadChunk (
    int x,
    int z
) = 0
```



A chunk kept alive by a nearby player, the spawn area, a `/tickingarea` or a plugin chunk ticket stays loaded, and this reports `false`. Unloading a chunk saves it and fires a [**ChunkUnloadEvent**](classendstone_1_1ChunkUnloadEvent.md), which handlers observe before this returns.




**Note:**

This also completes any chunk unloads the dimension had pending, so calling it once per chunk over a large area is expensive. Use `unloadChunkRequest()` when releasing many chunks at once.




**Parameters:**


* `x` X-coordinate of the chunk 
* `z` Z-coordinate of the chunk 



**Returns:**

`true` if the chunk is no longer loaded, otherwise `false` 





        

<hr>



### function unloadChunkRequest 

_Releases the hold that_ `loadChunk()` _placed on the_[_**Chunk**_](classendstone_1_1Chunk.md) _at the given coordinates, without unloading it now._
```C++
virtual bool endstone::Dimension::unloadChunkRequest (
    int x,
    int z
) = 0
```



The chunk is unloaded on a later tick once nothing else keeps it resident.




**Parameters:**


* `x` X-coordinate of the chunk 
* `z` Z-coordinate of the chunk 



**Returns:**

`true` 





        

<hr>



### function ~Dimension 

```C++
virtual endstone::Dimension::~Dimension () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/level/dimension.h`

