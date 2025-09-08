

# Class endstone::ChunkEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ChunkEvent**](classendstone_1_1ChunkEvent.md)



_Represents a_ [_**Chunk**_](classendstone_1_1Chunk.md) _related event._

* `#include <endstone/event/chunk/chunk_event.h>`



Inherits the following classes: [endstone::DimensionEvent](classendstone_1_1DimensionEvent.md)


Inherited by the following classes: [endstone::ChunkLoadEvent](classendstone_1_1ChunkLoadEvent.md),  [endstone::ChunkUnloadEvent](classendstone_1_1ChunkUnloadEvent.md)




























































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ChunkEvent**](#function-chunkevent) ([**Chunk**](classendstone_1_1Chunk.md) & chunk) <br> |
|  [**Chunk**](classendstone_1_1Chunk.md) & | [**getChunk**](#function-getchunk) () const<br> |


## Public Functions inherited from endstone::DimensionEvent

See [endstone::DimensionEvent](classendstone_1_1DimensionEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**DimensionEvent**](classendstone_1_1DimensionEvent.md#function-dimensionevent) ([**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |
|  [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](classendstone_1_1DimensionEvent.md#function-getdimension) () const<br> |


## Public Functions inherited from endstone::LevelEvent

See [endstone::LevelEvent](classendstone_1_1LevelEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**LevelEvent**](classendstone_1_1LevelEvent.md#function-levelevent) ([**Level**](classendstone_1_1Level.md) & level) <br> |
|  [**Level**](classendstone_1_1Level.md) & | [**getLevel**](classendstone_1_1LevelEvent.md#function-getlevel) () const<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |










































































































## Public Functions Documentation




### function ChunkEvent 

```C++
inline explicit endstone::ChunkEvent::ChunkEvent (
    Chunk & chunk
) 
```




<hr>



### function getChunk 

```C++
inline Chunk & endstone::ChunkEvent::getChunk () const
```



Gets the chunk being loaded/unloaded




**Returns:**

[**Chunk**](classendstone_1_1Chunk.md) that triggered this event 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/chunk/chunk_event.h`

