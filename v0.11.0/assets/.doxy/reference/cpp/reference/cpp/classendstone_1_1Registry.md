

# Class endstone::Registry

**template &lt;[**typename**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Registry**](classendstone_1_1Registry.md)



_Abstract registry interface for objects with identifiers._ [More...](#detailed-description)

* `#include <endstone/registry.h>`



Inherits the following classes: [endstone::IRegistry](classendstone_1_1IRegistry.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**forEach**](#function-foreach) (std::function&lt; [**bool**](classendstone_1_1Identifier.md)([**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) &)&gt; func) const = 0<br>_Apply a function to each stored element._  |
| virtual [**T**](classendstone_1_1Identifier.md) \* | [**get**](#function-get-12) ([**Identifier**](classendstone_1_1Identifier.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; id) noexcept = 0<br>_Get the object by its identifier._  |
| virtual [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**get**](#function-get-22) ([**Identifier**](classendstone_1_1Identifier.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; id) noexcept const = 0<br>_Get the object by its identifier._  |
| virtual [**T**](classendstone_1_1Identifier.md) & | [**getOrThrow**](#function-getorthrow-12) ([**Identifier**](classendstone_1_1Identifier.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; id) <br>_Get the object by its identifier or throw if missing._  |
| virtual [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) & | [**getOrThrow**](#function-getorthrow-22) ([**Identifier**](classendstone_1_1Identifier.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; id) const<br>_Get the object by its identifier or throw if missing._  |


## Public Functions inherited from endstone::IRegistry

See [endstone::IRegistry](classendstone_1_1IRegistry.md)

| Type | Name |
| ---: | :--- |
| virtual  | [**~IRegistry**](classendstone_1_1IRegistry.md#function-iregistry) () = default<br> |






















































## Detailed Description


Provides lookup by identifier (nullable or exception-throwing) and iteration.




**Template parameters:**


* `T` Must implement getId(). 




    
## Public Functions Documentation




### function forEach 

_Apply a function to each stored element._ 
```C++
virtual void endstone::Registry::forEach (
    std::function< bool ( const  T &)> func
) const = 0
```



Iterates over all stored elements and applies the provided function to them. The iteration continues until the function returns false or all elements have been processed.




**Parameters:**


* `func` A callable object taking a const reference to an element and returning a boolean. Returning false stops the iteration. 




        

<hr>



### function get [1/2]

_Get the object by its identifier._ 
```C++
virtual T * endstone::Registry::get (
    Identifier < T > id
) noexcept = 0
```



Returns a raw pointer to the object, or nullptr if not found.




**Parameters:**


* `id` [**Identifier**](classendstone_1_1Identifier.md) to look up. 



**Returns:**

T\* Pointer to the object, or nullptr if it does not exist. 





        

<hr>



### function get [2/2]

_Get the object by its identifier._ 
```C++
virtual const  T * endstone::Registry::get (
    Identifier < T > id
) noexcept const = 0
```



Returns a raw pointer to the object, or nullptr if not found.




**Parameters:**


* `id` Non-null identifier to look up. 



**Returns:**

const T\* Pointer to the object, or nullptr if it does not exist. 





        

<hr>



### function getOrThrow [1/2]

_Get the object by its identifier or throw if missing._ 
```C++
inline virtual T & endstone::Registry::getOrThrow (
    Identifier < T > id
) 
```





**Parameters:**


* `id` identifier of the object to retrieve. 



**Returns:**

T& Reference to the object with the given identifier. 




**Exception:**


* `std::invalid_argument` if no object with the given identifier exists. 




        

<hr>



### function getOrThrow [2/2]

_Get the object by its identifier or throw if missing._ 
```C++
inline virtual const  T & endstone::Registry::getOrThrow (
    Identifier < T > id
) const
```





**Parameters:**


* `id` identifier of the object to retrieve. 



**Returns:**

const T& Const reference to the object with the given identifier. 




**Exception:**


* `std::invalid_argument` if no object with the given identifier exists. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/registry.h`

