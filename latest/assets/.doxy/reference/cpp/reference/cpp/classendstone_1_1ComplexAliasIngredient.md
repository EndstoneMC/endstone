

# Class endstone::ComplexAliasIngredient



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ComplexAliasIngredient**](classendstone_1_1ComplexAliasIngredient.md)



_Represents an ingredient that matches any item an id stands for._ 

* `#include <endstone/inventory/complex_alias_ingredient.h>`



Inherits the following classes: [endstone::RecipeIngredient](classendstone_1_1RecipeIngredient.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**const**](classendstone_1_1Identifier.md) std::string & | [**getAlias**](#function-getalias) () const = 0<br>_Gets the alias that this ingredient will match._  |
|   | [**~ComplexAliasIngredient**](#function-complexaliasingredient) () override<br> |


## Public Functions inherited from endstone::RecipeIngredient

See [endstone::RecipeIngredient](classendstone_1_1RecipeIngredient.md)

| Type | Name |
| ---: | :--- |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getCount**](classendstone_1_1RecipeIngredient.md#function-getcount) () const = 0<br>_Get how many items this ingredient consumes._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**test**](classendstone_1_1RecipeIngredient.md#function-test) ([**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br> |
|   | [**~RecipeIngredient**](classendstone_1_1RecipeIngredient.md#function-recipeingredient) () override<br> |


## Public Functions inherited from endstone::Object

See [endstone::Object](classendstone_1_1Object.md)

| Type | Name |
| ---: | :--- |
|  [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1Object.md#function-as-12) () <br>_Attempts to cast this object to the given type T._  |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1Object.md#function-as-22) () const<br>_Attempts to cast this object to the given type T._  |
| virtual [**ClassInfo**](classendstone_1_1ClassInfo.md) | [**getClassInfo**](classendstone_1_1Object.md#function-getclassinfo) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**is**](classendstone_1_1Object.md#function-is) () const<br>_Checks if this object is an instance of the given type T (or a subclass of T)._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof-12) ([**ClassInfo**](classendstone_1_1ClassInfo.md) target) const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof-22) () const<br> |
| virtual  | [**~Object**](classendstone_1_1Object.md#function-object) () = default<br> |
















































































## Public Functions Documentation




### function getAlias 

_Gets the alias that this ingredient will match._ 
```C++
virtual const std::string & endstone::ComplexAliasIngredient::getAlias () const = 0
```



The id predates the item flattening, such as `minecraft:planks`, and stands for every item it was split into. It is not an item tag, and the items are not reported.




**Returns:**

the alias 





        

<hr>



### function ~ComplexAliasIngredient 

```C++
endstone::ComplexAliasIngredient::~ComplexAliasIngredient () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/complex_alias_ingredient.h`

