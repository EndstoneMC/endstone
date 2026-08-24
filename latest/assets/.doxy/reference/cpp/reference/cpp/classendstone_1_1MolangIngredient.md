

# Class endstone::MolangIngredient



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**MolangIngredient**](classendstone_1_1MolangIngredient.md)



_Represents an ingredient that matches the items a Molang expression selects._ 

* `#include <endstone/inventory/molang_ingredient.h>`



Inherits the following classes: [endstone::RecipeIngredient](classendstone_1_1RecipeIngredient.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**const**](classendstone_1_1Identifier.md) std::string & | [**getExpression**](#function-getexpression) () const = 0<br>_Gets the Molang expression that this ingredient will match._  |
|   | [**~MolangIngredient**](#function-molangingredient) () override<br> |


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
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof) ([**ClassInfo**](classendstone_1_1ClassInfo.md) target) const = 0<br> |
| virtual  | [**~Object**](classendstone_1_1Object.md#function-object) () = default<br> |
















































































## Public Functions Documentation




### function getExpression 

_Gets the Molang expression that this ingredient will match._ 
```C++
virtual const std::string & endstone::MolangIngredient::getExpression () const = 0
```





**Returns:**

the expression 





        

<hr>



### function ~MolangIngredient 

```C++
endstone::MolangIngredient::~MolangIngredient () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/molang_ingredient.h`

