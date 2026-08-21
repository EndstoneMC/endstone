

# Class endstone::PlayerEditBookEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PlayerEditBookEvent**](classendstone_1_1PlayerEditBookEvent.md)



_Called when a player edits or signs a book._ 

* `#include <endstone/event/player/player_edit_book_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PlayerEditBookEvent**](classendstone_1_1PlayerEditBookEvent.md)) <br> |
|   | [**PlayerEditBookEvent**](#function-playereditbookevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & player, [**int**](classendstone_1_1Identifier.md) slot, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**BookMeta**](classendstone_1_1BookMeta.md) &gt; & previous\_book\_meta, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**BookMeta**](classendstone_1_1BookMeta.md) &gt; & new\_book\_meta, [**bool**](classendstone_1_1Identifier.md) signing) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**BookMeta**](classendstone_1_1BookMeta.md) &gt; & | [**getNewBookMeta**](#function-getnewbookmeta) () const<br>_Gets the metadata that will be applied after the edit._  |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**BookMeta**](classendstone_1_1BookMeta.md) &gt; & | [**getPreviousBookMeta**](#function-getpreviousbookmeta) () const<br>_Gets the metadata before the edit._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getSlot**](#function-getslot) () const<br>_Gets the inventory slot containing the book._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isSigning**](#function-issigning) () const<br>_Gets whether the book should be signed._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setNewBookMeta**](#function-setnewbookmeta) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**BookMeta**](classendstone_1_1BookMeta.md) &gt; & new\_book\_meta) <br>_Replaces the metadata that will be applied after the edit._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setSigning**](#function-setsigning) ([**bool**](classendstone_1_1Identifier.md) signing) <br>_Sets whether the book should be signed._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br>_Cancels this event._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br>_Gets the cancellation state of this event._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br>_Sets the cancellation state of this event._  |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PlayerEditBookEvent::ENDSTONE_EVENT (
    PlayerEditBookEvent
) 
```




<hr>



### function PlayerEditBookEvent 

```C++
inline endstone::PlayerEditBookEvent::PlayerEditBookEvent (
    const  NotNull < Player > & player,
    int slot,
    const  NotNull < BookMeta > & previous_book_meta,
    const  NotNull < BookMeta > & new_book_meta,
    bool signing
) 
```




<hr>



### function getNewBookMeta 

_Gets the metadata that will be applied after the edit._ 
```C++
inline const  NotNull < BookMeta > & endstone::PlayerEditBookEvent::getNewBookMeta () const
```



Changes made to it take effect.




**Returns:**

the book metadata after the edit 





        

<hr>



### function getPreviousBookMeta 

_Gets the metadata before the edit._ 
```C++
inline const  NotNull < BookMeta > & endstone::PlayerEditBookEvent::getPreviousBookMeta () const
```





**Returns:**

the book metadata before the edit 





        

<hr>



### function getSlot 

_Gets the inventory slot containing the book._ 
```C++
inline int endstone::PlayerEditBookEvent::getSlot () const
```





**Returns:**

the inventory slot containing the book 





        

<hr>



### function isSigning 

_Gets whether the book should be signed._ 
```C++
inline bool endstone::PlayerEditBookEvent::isSigning () const
```





**Returns:**

true if the book should be signed 





        

<hr>



### function setNewBookMeta 

_Replaces the metadata that will be applied after the edit._ 
```C++
inline void endstone::PlayerEditBookEvent::setNewBookMeta (
    const  NotNull < BookMeta > & new_book_meta
) 
```





**Parameters:**


* `new_book_meta` the metadata to apply after the edit 




        

<hr>



### function setSigning 

_Sets whether the book should be signed._ 
```C++
inline void endstone::PlayerEditBookEvent::setSigning (
    bool signing
) 
```





**Parameters:**


* `signing` true to sign the book, false to leave it unsigned 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/player/player_edit_book_event.h`

