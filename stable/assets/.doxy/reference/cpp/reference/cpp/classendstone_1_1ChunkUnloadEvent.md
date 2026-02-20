

# Class endstone::ChunkUnloadEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ChunkUnloadEvent**](classendstone_1_1ChunkUnloadEvent.md)



_Called when a chunk is unloaded._ 

* `#include <endstone/event/chunk/chunk_unload_event.h>`



Inherits the following classes: [endstone::ChunkEvent](classendstone_1_1ChunkEvent.md)


















































































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ChunkUnloadEvent**](#function-chunkunloadevent) ([**Chunk**](classendstone_1_1Chunk.md) & chunk) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ChunkUnloadEvent**](classendstone_1_1ChunkUnloadEvent.md)) <br> |
|   | [**~ChunkUnloadEvent**](#function-chunkunloadevent) () override<br> |


## Public Functions inherited from endstone::ChunkEvent

See [endstone::ChunkEvent](classendstone_1_1ChunkEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**ChunkEvent**](classendstone_1_1ChunkEvent.md#function-chunkevent) ([**Chunk**](classendstone_1_1Chunk.md) & chunk) <br> |
|  [**Chunk**](classendstone_1_1Chunk.md) & | [**getChunk**](classendstone_1_1ChunkEvent.md#function-getchunk) () const<br> |


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
|   | [**Event**](classendstone_1_1Event.md#function-event-13) ([**bool**](classendstone_1_1Identifier.md) async=[**false**](classendstone_1_1Identifier.md)) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-23) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-33) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator_1) ([**Event**](classendstone_1_1Event.md) &&) = default<br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |




































































































































## Public Functions Documentation




### function ChunkUnloadEvent 

```C++
inline explicit endstone::ChunkUnloadEvent::ChunkUnloadEvent (
    Chunk & chunk
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::ChunkUnloadEvent::ENDSTONE_EVENT (
    ChunkUnloadEvent
) 
```




<hr>



### function ~ChunkUnloadEvent 

```C++
endstone::ChunkUnloadEvent::~ChunkUnloadEvent () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/chunk/chunk_unload_event.h`

