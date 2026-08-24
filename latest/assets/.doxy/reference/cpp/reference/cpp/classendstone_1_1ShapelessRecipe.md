

# Class endstone::ShapelessRecipe



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ShapelessRecipe**](classendstone_1_1ShapelessRecipe.md)



_Represents a shapeless recipe, where the arrangement of the ingredients on the crafting grid does not matter._ 

* `#include <endstone/inventory/shapeless_recipe.h>`



Inherits the following classes: [endstone::Recipe](classendstone_1_1Recipe.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**~ShapelessRecipe**](#function-shapelessrecipe) () override<br> |


## Public Functions inherited from endstone::Recipe

See [endstone::Recipe](classendstone_1_1Recipe.md)

| Type | Name |
| ---: | :--- |
| virtual [**const**](classendstone_1_1Identifier.md) std::vector&lt; [**Nullable**](classendstone_1_1Nullable.md)&lt; [**RecipeIngredient**](classendstone_1_1RecipeIngredient.md) &gt; &gt; & | [**getIngredients**](classendstone_1_1Recipe.md#function-getingredients) () const = 0<br> |
| virtual [**const**](classendstone_1_1Identifier.md) std::string & | [**getRecipeId**](classendstone_1_1Recipe.md#function-getrecipeid) () const = 0<br> |
| virtual [**ItemStack**](classendstone_1_1ItemStack.md) | [**getResult**](classendstone_1_1Recipe.md#function-getresult) () const = 0<br>_Get the result of this recipe._  |
| virtual [**const**](classendstone_1_1Identifier.md) std::string & | [**getTag**](classendstone_1_1Recipe.md#function-gettag) () const = 0<br>_Get the crafting station this recipe belongs to, such as_ `crafting_table` _or_`smithing_table` _._ |
|   | [**~Recipe**](classendstone_1_1Recipe.md#function-recipe) () override<br> |


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




### function ~ShapelessRecipe 

```C++
endstone::ShapelessRecipe::~ShapelessRecipe () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/shapeless_recipe.h`

