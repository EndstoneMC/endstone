

# Class endstone::Registry

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Registry**](classendstone_1_1Registry.md)



_Abstract registry interface for keyed objects._ [More...](#detailed-description)

* `#include <endstone/registry.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**forEach**](#function-foreach) (std::function&lt; bool(const T &)&gt; func) const = 0<br>_Apply a function to each stored element._  |
| virtual T \* | [**get**](#function-get-12) ([**NamespacedKey**](classendstone_1_1NamespacedKey.md) key) noexcept = 0<br>_Get the object by its key._  |
| virtual const T \* | [**get**](#function-get-22) ([**NamespacedKey**](classendstone_1_1NamespacedKey.md) key) noexcept const = 0<br>_Const overload of_ [_**get()**_](classendstone_1_1Registry.md#function-get-12) _._ |
| virtual T & | [**getOrThrow**](#function-getorthrow-12) (const [**NamespacedKey**](classendstone_1_1NamespacedKey.md) key) <br>_Get the object by its key or throw if missing._  |
| virtual const T & | [**getOrThrow**](#function-getorthrow-22) (const [**NamespacedKey**](classendstone_1_1NamespacedKey.md) key) const<br>_Const overload of_ [_**getOrThrow()**_](classendstone_1_1Registry.md#function-getorthrow-12) _._ |
| virtual  | [**~Registry**](#function-registry) () = default<br> |




























## Detailed Description


Provides lookup by key (nullable or exception-throwing) and iteration.




**Template parameters:**


* `T` Must satisfy Keyed, i.e. implement getKey(). 




    
## Public Functions Documentation




### function forEach 

_Apply a function to each stored element._ 
```C++
virtual void endstone::Registry::forEach (
    std::function< bool(const T &)> func
) const = 0
```



Iterates over all stored elements and applies the provided function to them. The iteration continues until the function returns false or all elements have been processed.




**Parameters:**


* `func` A callable object taking a const reference to an element and returning a boolean. Returning false stops the iteration. 




        

<hr>



### function get [1/2]

_Get the object by its key._ 
```C++
virtual T * endstone::Registry::get (
    NamespacedKey key
) noexcept = 0
```



Returns a raw pointer to the object, or nullptr if not found.




**Parameters:**


* `key` Non-null key to look up. 



**Returns:**

T\* Pointer to the object, or nullptr if it does not exist. 





        

<hr>



### function get [2/2]

_Const overload of_ [_**get()**_](classendstone_1_1Registry.md#function-get-12) _._
```C++
virtual const T * endstone::Registry::get (
    NamespacedKey key
) noexcept const = 0
```





**Parameters:**


* `key` Non-null key to look up. 



**Returns:**

const T\* Pointer to the object, or nullptr if it does not exist. 





        

<hr>



### function getOrThrow [1/2]

_Get the object by its key or throw if missing._ 
```C++
inline virtual T & endstone::Registry::getOrThrow (
    const NamespacedKey key
) 
```





**Parameters:**


* `key` Key of the object to retrieve. 



**Returns:**

T& Reference to the object with the given key. 




**Exception:**


* `std::invalid_argument` if no object with the given key exists. 




        

<hr>



### function getOrThrow [2/2]

_Const overload of_ [_**getOrThrow()**_](classendstone_1_1Registry.md#function-getorthrow-12) _._
```C++
inline virtual const T & endstone::Registry::getOrThrow (
    const NamespacedKey key
) const
```





**Parameters:**


* `key` Key of the object to retrieve. 



**Returns:**

const T& Const reference to the object with the given key. 




**Exception:**


* `std::invalid_argument` if no object with the given key exists. 




        

<hr>



### function ~Registry 

```C++
virtual endstone::Registry::~Registry () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/registry.h`

