

# Class endstone::RecipeIngredient



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**RecipeIngredient**](classendstone_1_1RecipeIngredient.md)



_Represents a potential item match within a recipe._ [More...](#detailed-description)

* `#include <endstone/inventory/recipe_ingredient.h>`



Inherits the following classes: [endstone::Object](classendstone_1_1Object.md)


Inherited by the following classes: [endstone::ComplexAliasIngredient](classendstone_1_1ComplexAliasIngredient.md),  [endstone::ExactIngredient](classendstone_1_1ExactIngredient.md),  [endstone::ItemTagIngredient](classendstone_1_1ItemTagIngredient.md),  [endstone::ItemTypeIngredient](classendstone_1_1ItemTypeIngredient.md),  [endstone::MolangIngredient](classendstone_1_1MolangIngredient.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getCount**](#function-getcount) () const = 0<br>_Get how many items this ingredient consumes._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**test**](#function-test) ([**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br> |
|   | [**~RecipeIngredient**](#function-recipeingredient) () override<br> |


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






















































## Detailed Description


All choices within a recipe must be satisfied for it to be craftable. 


    
## Public Functions Documentation




### function getCount 

_Get how many items this ingredient consumes._ 
```C++
virtual int endstone::RecipeIngredient::getCount () const = 0
```



Bedrock records a count on each ingredient where Java repeats the ingredient instead.




**Returns:**

the number of items consumed 





        

<hr>



### function test 

```C++
virtual bool endstone::RecipeIngredient::test (
    const  ItemStack & item
) const = 0
```




<hr>



### function ~RecipeIngredient 

```C++
endstone::RecipeIngredient::~RecipeIngredient () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/recipe_ingredient.h`

