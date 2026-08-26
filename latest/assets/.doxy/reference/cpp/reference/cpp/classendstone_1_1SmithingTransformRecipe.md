

# Class endstone::SmithingTransformRecipe



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**SmithingTransformRecipe**](classendstone_1_1SmithingTransformRecipe.md)



_Represents a smithing transform recipe._ 

* `#include <endstone/inventory/smithing_transform_recipe.h>`



Inherits the following classes: [endstone::SmithingRecipe](classendstone_1_1SmithingRecipe.md)






























































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Nullable**](classendstone_1_1Nullable.md)&lt; [**RecipeIngredient**](classendstone_1_1RecipeIngredient.md) &gt; | [**getTemplate**](#function-gettemplate) () const = 0<br>_Get the template recipe item._  |
|   | [**~SmithingTransformRecipe**](#function-smithingtransformrecipe) () override<br> |


## Public Functions inherited from endstone::SmithingRecipe

See [endstone::SmithingRecipe](classendstone_1_1SmithingRecipe.md)

| Type | Name |
| ---: | :--- |
| virtual [**Nullable**](classendstone_1_1Nullable.md)&lt; [**RecipeIngredient**](classendstone_1_1RecipeIngredient.md) &gt; | [**getAddition**](classendstone_1_1SmithingRecipe.md#function-getaddition) () const = 0<br>_Get the addition recipe item._  |
| virtual [**Nullable**](classendstone_1_1Nullable.md)&lt; [**RecipeIngredient**](classendstone_1_1RecipeIngredient.md) &gt; | [**getBase**](classendstone_1_1SmithingRecipe.md#function-getbase) () const = 0<br>_Get the base recipe item._  |
|   | [**~SmithingRecipe**](classendstone_1_1SmithingRecipe.md#function-smithingrecipe) () override<br> |


## Public Functions inherited from endstone::Recipe

See [endstone::Recipe](classendstone_1_1Recipe.md)

| Type | Name |
| ---: | :--- |
| virtual [**const**](classendstone_1_1Identifier.md) std::string & | [**getId**](classendstone_1_1Recipe.md#function-getid) () const = 0<br>_Get the identifier of this recipe, such as_ `minecraft:crafting_table` _._ |
| virtual [**const**](classendstone_1_1Identifier.md) std::vector&lt; [**Nullable**](classendstone_1_1Nullable.md)&lt; [**RecipeIngredient**](classendstone_1_1RecipeIngredient.md) &gt; &gt; & | [**getIngredients**](classendstone_1_1Recipe.md#function-getingredients) () const = 0<br>_Gets the ingredients consumed by this recipe._  |
| virtual [**ItemStack**](classendstone_1_1ItemStack.md) | [**getResult**](classendstone_1_1Recipe.md#function-getresult) () const = 0<br>_Get the result of this recipe._  |
| virtual [**const**](classendstone_1_1Identifier.md) std::string & | [**getTag**](classendstone_1_1Recipe.md#function-gettag) () const = 0<br>_Gets the station this recipe belongs to, such as_ `crafting_table` _,_`smithing_table` _or_`furnace` _._ |
|   | [**~Recipe**](classendstone_1_1Recipe.md#function-recipe) () override<br> |


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




### function getTemplate 

_Get the template recipe item._ 
```C++
virtual Nullable < RecipeIngredient > endstone::SmithingTransformRecipe::getTemplate () const = 0
```





**Returns:**

template choice 





        

<hr>



### function ~SmithingTransformRecipe 

```C++
endstone::SmithingTransformRecipe::~SmithingTransformRecipe () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/smithing_transform_recipe.h`

