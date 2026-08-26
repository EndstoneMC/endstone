

# Class endstone::CampfireRecipe



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CampfireRecipe**](classendstone_1_1CampfireRecipe.md)



_Represents a campfire recipe._ 

* `#include <endstone/inventory/campfire_recipe.h>`



Inherits the following classes: [endstone::CookingRecipe](classendstone_1_1CookingRecipe.md)






























































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**~CampfireRecipe**](#function-campfirerecipe) () override<br> |


## Public Functions inherited from endstone::CookingRecipe

See [endstone::CookingRecipe](classendstone_1_1CookingRecipe.md)

| Type | Name |
| ---: | :--- |
| virtual [**Nullable**](classendstone_1_1Nullable.md)&lt; [**RecipeIngredient**](classendstone_1_1RecipeIngredient.md) &gt; | [**getInputChoice**](classendstone_1_1CookingRecipe.md#function-getinputchoice) () const = 0<br>_Get the input choice._  |
|   | [**~CookingRecipe**](classendstone_1_1CookingRecipe.md#function-cookingrecipe) () override<br> |


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




### function ~CampfireRecipe 

```C++
endstone::CampfireRecipe::~CampfireRecipe () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/campfire_recipe.h`

