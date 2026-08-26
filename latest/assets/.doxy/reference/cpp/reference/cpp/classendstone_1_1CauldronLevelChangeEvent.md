

# Class endstone::CauldronLevelChangeEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CauldronLevelChangeEvent**](classendstone_1_1CauldronLevelChangeEvent.md)



_Called when a cauldron's level or contents change._ [More...](#detailed-description)

* `#include <endstone/event/block/cauldron_level_change_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ChangeReason**](#enum-changereason)  <br>_Describes what caused the cauldron to change._  |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CauldronLevelChangeEvent**](#function-cauldronlevelchangeevent) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & block, [**const**](classendstone_1_1Identifier.md) [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & actor, [**ChangeReason**](classendstone_1_1CauldronLevelChangeEvent.md#enum-changereason) reason, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**BlockState**](classendstone_1_1BlockState.md) &gt; & new\_state) <br> |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**CauldronLevelChangeEvent**](classendstone_1_1CauldronLevelChangeEvent.md)) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**Nullable**](classendstone_1_1Nullable.md)&lt; [**Actor**](classendstone_1_1Actor.md) &gt; & | [**getActor**](#function-getactor) () const<br>_Gets the actor which did this._  |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**BlockState**](classendstone_1_1BlockState.md) &gt; & | [**getNewState**](#function-getnewstate) () const<br>_Gets the state the cauldron will take._  |
|  [**ChangeReason**](classendstone_1_1CauldronLevelChangeEvent.md#enum-changereason) | [**getReason**](#function-getreason) () const<br>_Gets the reason for the change._  |


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
















































































## Detailed Description


If this event is cancelled, the cauldron will not change. 


    
## Public Types Documentation




### enum ChangeReason 

_Describes what caused the cauldron to change._ 
```C++
enum endstone::CauldronLevelChangeEvent::ChangeReason {
    BucketFill,
    BucketEmpty,
    BottleFill,
    BottleEmpty,
    BannerWash,
    ArmorWash,
    ShulkerWash,
    Extinguish,
    Evaporate,
    NaturalFill,
    Unknown
};
```




<hr>
## Public Functions Documentation




### function CauldronLevelChangeEvent 

```C++
inline endstone::CauldronLevelChangeEvent::CauldronLevelChangeEvent (
    const  NotNull < Block > & block,
    const  Nullable < Actor > & actor,
    ChangeReason reason,
    const  NotNull < BlockState > & new_state
) 
```




<hr>



### function ENDSTONE\_EVENT 

```C++
endstone::CauldronLevelChangeEvent::ENDSTONE_EVENT (
    CauldronLevelChangeEvent
) 
```




<hr>



### function getActor 

_Gets the actor which did this._ 
```C++
inline const  Nullable < Actor > & endstone::CauldronLevelChangeEvent::getActor () const
```



Only a player interacting with the cauldron is reported. Every other change reports nullptr.




**Returns:**

the actor which did this, or nullptr if there is none 





        

<hr>



### function getNewState 

_Gets the state the cauldron will take._ 
```C++
inline const  NotNull < BlockState > & endstone::CauldronLevelChangeEvent::getNewState () const
```



Modifying the returned state changes what the cauldron becomes.




**Returns:**

the new state of the cauldron 





        

<hr>



### function getReason 

_Gets the reason for the change._ 
```C++
inline ChangeReason endstone::CauldronLevelChangeEvent::getReason () const
```



Only a player interacting with the cauldron is attributed. Every other change reports ChangeReason::Unknown.




**Returns:**

the reason for the change 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/block/cauldron_level_change_event.h`

