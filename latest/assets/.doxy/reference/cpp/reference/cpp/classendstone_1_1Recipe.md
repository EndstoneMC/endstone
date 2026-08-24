

# Class endstone::Recipe



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Recipe**](classendstone_1_1Recipe.md)



_Represents some type of crafting recipe._ 

* `#include <endstone/inventory/recipe.h>`



Inherits the following classes: [endstone::Object](classendstone_1_1Object.md)


Inherited by the following classes: [endstone::ComplexRecipe](classendstone_1_1ComplexRecipe.md),  [endstone::ShapedRecipe](classendstone_1_1ShapedRecipe.md),  [endstone::ShapelessRecipe](classendstone_1_1ShapelessRecipe.md),  [endstone::SmithingRecipe](classendstone_1_1SmithingRecipe.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**const**](classendstone_1_1Identifier.md) std::vector&lt; [**Nullable**](classendstone_1_1Nullable.md)&lt; [**RecipeIngredient**](classendstone_1_1RecipeIngredient.md) &gt; &gt; & | [**getIngredients**](#function-getingredients) () const = 0<br> |
| virtual [**const**](classendstone_1_1Identifier.md) std::string & | [**getRecipeId**](#function-getrecipeid) () const = 0<br> |
| virtual [**ItemStack**](classendstone_1_1ItemStack.md) | [**getResult**](#function-getresult) () const = 0<br>_Get the result of this recipe._  |
| virtual [**const**](classendstone_1_1Identifier.md) std::string & | [**getTag**](#function-gettag) () const = 0<br>_Get the crafting station this recipe belongs to, such as_ `crafting_table` _or_`smithing_table` _._ |
|   | [**~Recipe**](#function-recipe) () override<br> |


## Public Functions inherited from endstone::Object

See [endstone::Object](classendstone_1_1Object.md)

| Type | Name |
| ---: | :--- |
|  [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1Object.md#function-as-12) () <br>_Attempts to cast this object to the given type T._  |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1Object.md#function-as-22) () const<br>_Attempts to cast this object to the given type T._  |
| virtual [**ClassInfo**](classendstone_1_1ClassInfo.md) | [**getClassInfo**](classendstone_1_1Object.md#function-getclassinfo) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**is**](classendstone_1_1Object.md#function-is) () const<br>_Checks if this object is an instance of the given type T (or a subclass of T)._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof) ([**ClassInfo**](classendstone_1_1ClassInfo.md) target) const = 0<br> |
| virtual  | [**~Object**](classendstone_1_1Object.md#function-object) () = default<br> |






















































## Public Functions Documentation




### function getIngredients 

```C++
virtual const std::vector< Nullable < RecipeIngredient > > & endstone::Recipe::getIngredients () const = 0
```




<hr>



### function getRecipeId 

```C++
virtual const std::string & endstone::Recipe::getRecipeId () const = 0
```




<hr>



### function getResult 

_Get the result of this recipe._ 
```C++
virtual ItemStack endstone::Recipe::getResult () const = 0
```





**Returns:**

The result stack 





        

<hr>



### function getTag 

_Get the crafting station this recipe belongs to, such as_ `crafting_table` _or_`smithing_table` _._
```C++
virtual const std::string & endstone::Recipe::getTag () const = 0
```





**Returns:**

the crafting tag 





        

<hr>



### function ~Recipe 

```C++
endstone::Recipe::~Recipe () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/recipe.h`

