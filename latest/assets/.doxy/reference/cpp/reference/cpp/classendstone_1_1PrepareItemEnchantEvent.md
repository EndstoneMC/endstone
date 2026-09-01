

# Class endstone::PrepareItemEnchantEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**PrepareItemEnchantEvent**](classendstone_1_1PrepareItemEnchantEvent.md)



_Called when an enchanting table prepares offers for an item._ [More...](#detailed-description)

* `#include <endstone/event/enchantment/prepare_item_enchant_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::array&lt; std::optional&lt; [**EnchantmentOffer**](classendstone_1_1EnchantmentOffer.md) &gt;, 3 &gt; | [**Offers**](#typedef-offers)  <br> |




























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**PrepareItemEnchantEvent**](classendstone_1_1PrepareItemEnchantEvent.md)) <br> |
|   | [**PrepareItemEnchantEvent**](#function-prepareitemenchantevent) ([**Inventory**](classendstone_1_1Inventory.md) & inventory, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & enchanter, [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & enchant\_block, [**ItemStack**](classendstone_1_1ItemStack.md) item, Offers offers, [**int**](classendstone_1_1Identifier.md) enchantment\_bonus) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Block**](classendstone_1_1Block.md) &gt; & | [**getEnchantBlock**](#function-getenchantblock) () const<br>_Gets the enchanting table involved in this event._  |
|  [**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**Player**](classendstone_1_1Player.md) &gt; & | [**getEnchanter**](#function-getenchanter) () const<br>_Gets the player for whom the offers are being prepared._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getEnchantmentBonus**](#function-getenchantmentbonus) () const<br>_Gets the enchanting bonus provided by nearby bookshelves._  |
|  [**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & | [**getItem**](#function-getitem) () const<br>_Gets the item for which offers are being prepared._  |
|  [**const**](classendstone_1_1Identifier.md) Offers & | [**getOffers**](#function-getoffers-12) () const<br>_Gets the three enchanting offers._  |
|  Offers & | [**getOffers**](#function-getoffers-22) () <br>_Gets the three enchanting offers._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setOffers**](#function-setoffers) (Offers offers) <br>_Sets the three enchanting offers._  |


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


Set an offer to std::nullopt to hide it, or assign a new [**EnchantmentOffer**](classendstone_1_1EnchantmentOffer.md) to replace it. Each offer contains every enchantment that Bedrock applies when its corresponding button is selected.




**Note:**

This event can be called multiple times while the enchanting table is open. 




**Note:**

Cancelling the event removes all enchanting offers. 





    
## Public Types Documentation




### typedef Offers 

```C++
using endstone::PrepareItemEnchantEvent::Offers =  std::array<std::optional<EnchantmentOffer>, 3>;
```




<hr>
## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::PrepareItemEnchantEvent::ENDSTONE_EVENT (
    PrepareItemEnchantEvent
) 
```




<hr>



### function PrepareItemEnchantEvent 

```C++
inline endstone::PrepareItemEnchantEvent::PrepareItemEnchantEvent (
    Inventory & inventory,
    const  NotNull < Player > & enchanter,
    const  NotNull < Block > & enchant_block,
    ItemStack item,
    Offers offers,
    int enchantment_bonus
) 
```




<hr>



### function getEnchantBlock 

_Gets the enchanting table involved in this event._ 
```C++
inline const  NotNull < Block > & endstone::PrepareItemEnchantEvent::getEnchantBlock () const
```





**Returns:**

the enchanting table 





        

<hr>



### function getEnchanter 

_Gets the player for whom the offers are being prepared._ 
```C++
inline const  NotNull < Player > & endstone::PrepareItemEnchantEvent::getEnchanter () const
```





**Returns:**

the player viewing the enchanting table 





        

<hr>



### function getEnchantmentBonus 

_Gets the enchanting bonus provided by nearby bookshelves._ 
```C++
inline int endstone::PrepareItemEnchantEvent::getEnchantmentBonus () const
```





**Returns:**

the number of contributing bookshelves 





        

<hr>



### function getItem 

_Gets the item for which offers are being prepared._ 
```C++
inline const  ItemStack & endstone::PrepareItemEnchantEvent::getItem () const
```





**Returns:**

the item being enchanted 





        

<hr>



### function getOffers [1/2]

_Gets the three enchanting offers._ 
```C++
inline const Offers & endstone::PrepareItemEnchantEvent::getOffers () const
```



A missing value indicates that the corresponding slot has no offer.




**Returns:**

the enchanting offers 





        

<hr>



### function getOffers [2/2]

_Gets the three enchanting offers._ 
```C++
inline Offers & endstone::PrepareItemEnchantEvent::getOffers () 
```



A missing value indicates that the corresponding slot has no offer.




**Returns:**

the enchanting offers 





        

<hr>



### function setOffers 

_Sets the three enchanting offers._ 
```C++
inline void endstone::PrepareItemEnchantEvent::setOffers (
    Offers offers
) 
```





**Parameters:**


* `offers` the enchanting offers 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/enchantment/prepare_item_enchant_event.h`

