

# Class endstone::CookingRecipe



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CookingRecipe**](classendstone_1_1CookingRecipe.md)



_Represents a cooking recipe._ [More...](#detailed-description)

* `#include <endstone/inventory/cooking_recipe.h>`



Inherits the following classes: [endstone::Recipe](classendstone_1_1Recipe.md)


Inherited by the following classes: [endstone::BlastingRecipe](classendstone_1_1BlastingRecipe.md),  [endstone::CampfireRecipe](classendstone_1_1CampfireRecipe.md),  [endstone::FurnaceRecipe](classendstone_1_1FurnaceRecipe.md),  [endstone::SmokingRecipe](classendstone_1_1SmokingRecipe.md)








































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Nullable**](classendstone_1_1Nullable.md)&lt; [**RecipeIngredient**](classendstone_1_1RecipeIngredient.md) &gt; | [**getInputChoice**](#function-getinputchoice) () const = 0<br>_Get the input choice._  |
|   | [**~CookingRecipe**](#function-cookingrecipe) () override<br> |


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
















































































## Detailed Description


Bedrock records neither an experience reward nor a cooking time on the recipe itself, so neither is reported here. The experience a smelt awards belongs to the input item, and the time a cook takes to the station. 


    
## Public Functions Documentation




### function getInputChoice 

_Get the input choice._ 
```C++
virtual Nullable < RecipeIngredient > endstone::CookingRecipe::getInputChoice () const = 0
```





**Returns:**

the input choice 





        

<hr>



### function ~CookingRecipe 

```C++
endstone::CookingRecipe::~CookingRecipe () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/cooking_recipe.h`

