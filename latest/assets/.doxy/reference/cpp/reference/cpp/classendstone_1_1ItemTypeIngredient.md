

# Class endstone::ItemTypeIngredient



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ItemTypeIngredient**](classendstone_1_1ItemTypeIngredient.md)



_Represents an ingredient that matches an item type, whatever its data value._ 

* `#include <endstone/inventory/item_type_ingredient.h>`



Inherits the following classes: [endstone::RecipeIngredient](classendstone_1_1RecipeIngredient.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**const**](classendstone_1_1Identifier.md) [**ItemType**](classendstone_1_1ItemType.md) & | [**getItemType**](#function-getitemtype) () const = 0<br>_Gets the item type that this ingredient will match._  |
|   | [**~ItemTypeIngredient**](#function-itemtypeingredient) () override<br> |


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
| virtual [**const**](classendstone_1_1Identifier.md) std::type\_info & | [**getClassTypeId**](classendstone_1_1Object.md#function-getclasstypeid) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**is**](classendstone_1_1Object.md#function-is) () const<br>_Checks if this object is an instance of the given type T (or a subclass of T)._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof) ([**const**](classendstone_1_1Identifier.md) std::type\_info & target) const = 0<br> |
| virtual  | [**~Object**](classendstone_1_1Object.md#function-object) () = default<br> |
















































































## Public Functions Documentation




### function getItemType 

_Gets the item type that this ingredient will match._ 
```C++
virtual const  ItemType & endstone::ItemTypeIngredient::getItemType () const = 0
```





**Returns:**

the item type 





        

<hr>



### function ~ItemTypeIngredient 

```C++
endstone::ItemTypeIngredient::~ItemTypeIngredient () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/item_type_ingredient.h`

