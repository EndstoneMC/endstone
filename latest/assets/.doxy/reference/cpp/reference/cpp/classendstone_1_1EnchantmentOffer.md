

# Class endstone::EnchantmentOffer



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**EnchantmentOffer**](classendstone_1_1EnchantmentOffer.md)



_Represents an offer made by an enchanting table._ [More...](#detailed-description)

* `#include <endstone/enchantments/enchantment_offer.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::unordered\_map&lt; [**const**](classendstone_1_1Identifier.md) [**Enchantment**](classendstone_1_1Enchantment.md) \*, [**int**](classendstone_1_1Identifier.md) &gt; | [**Enchantments**](#typedef-enchantments)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EnchantmentOffer**](#function-enchantmentoffer) (Enchantments enchants, [**int**](classendstone_1_1Identifier.md) cost) <br>_Creates an enchanting offer._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getCost**](#function-getcost) () const<br>_Gets the minimum player level required to select this offer._  |
|  [**const**](classendstone_1_1Identifier.md) Enchantments & | [**getEnchants**](#function-getenchants) () const<br>_Gets the enchantments applied by this offer._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setCost**](#function-setcost) ([**int**](classendstone_1_1Identifier.md) cost) <br>_Sets the minimum player level required to select this offer._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setEnchants**](#function-setenchants) (Enchantments enchants) <br>_Sets the enchantments applied by this offer._  |




























## Detailed Description


Bedrock stores every enchantment that selecting an offer applies rather than a single hinted enchantment. Enchantments in an offer must be entries obtained from Endstone's [**Enchantment**](classendstone_1_1Enchantment.md) registry. 


    
## Public Types Documentation




### typedef Enchantments 

```C++
using endstone::EnchantmentOffer::Enchantments =  std::unordered_map<const Enchantment *, int>;
```




<hr>
## Public Functions Documentation




### function EnchantmentOffer 

_Creates an enchanting offer._ 
```C++
inline endstone::EnchantmentOffer::EnchantmentOffer (
    Enchantments enchants,
    int cost
) 
```





**Parameters:**


* `enchants` the enchantments and levels applied by the offer 
* `cost` the minimum player level required to select the offer, between 1 and 255 




        

<hr>



### function getCost 

_Gets the minimum player level required to select this offer._ 
```C++
inline int endstone::EnchantmentOffer::getCost () const
```





**Returns:**

the minimum required player level 





        

<hr>



### function getEnchants 

_Gets the enchantments applied by this offer._ 
```C++
inline const Enchantments & endstone::EnchantmentOffer::getEnchants () const
```





**Returns:**

the enchantments and their levels 





        

<hr>



### function setCost 

_Sets the minimum player level required to select this offer._ 
```C++
inline void endstone::EnchantmentOffer::setCost (
    int cost
) 
```





**Parameters:**


* `cost` the minimum required player level, between 1 and 255 




        

<hr>



### function setEnchants 

_Sets the enchantments applied by this offer._ 
```C++
inline void endstone::EnchantmentOffer::setEnchants (
    Enchantments enchants
) 
```





**Parameters:**


* `enchants` the enchantments and their levels 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/enchantments/enchantment_offer.h`

