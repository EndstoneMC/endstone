

# Class endstone::Recipe



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Recipe**](classendstone_1_1Recipe.md)



_Represents some type of crafting recipe._ 

* `#include <endstone/inventory/recipe.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::unique\_ptr&lt; [**ItemStack**](classendstone_1_1ItemStack.md) &gt; | [**getResult**](#function-getresult) () const = 0<br>_Get the result of this recipe._  |
| virtual  | [**~Recipe**](#function-recipe) () = default<br> |




























## Public Functions Documentation




### function getResult 

_Get the result of this recipe._ 
```C++
virtual std::unique_ptr< ItemStack > endstone::Recipe::getResult () const = 0
```





**Returns:**

The result stack 





        

<hr>



### function ~Recipe 

```C++
virtual endstone::Recipe::~Recipe () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/inventory/recipe.h`

