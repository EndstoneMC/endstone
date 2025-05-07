

# Class endstone::Registry

**template &lt;Keyed T&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Registry**](classendstone_1_1Registry.md)



_Abstract registry interface for keyed objects._ [More...](#detailed-description)

* `#include <endstone/registry.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**typename**](classendstone_1_1Vector.md) storage\_type::const\_iterator | [**const\_iterator**](#typedef-const_iterator)  <br>_Const iterator over stored elements._  |
| typedef [**const**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) & | [**const\_reference**](#typedef-const_reference)  <br> |
| typedef [**typename**](classendstone_1_1Vector.md) storage\_type::iterator | [**iterator**](#typedef-iterator)  <br>_Iterator over stored elements._  |
| typedef std::string\_view | [**key\_type**](#typedef-key_type)  <br> |
| typedef [**T**](classendstone_1_1Vector.md) & | [**reference**](#typedef-reference)  <br> |
| typedef std::unique\_ptr&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**value\_type**](#typedef-value_type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**iterator**](classendstone_1_1Registry.md#typedef-iterator) | [**begin**](#function-begin-12) () = 0<br>_Returns iterator to beginning of registry items._  |
| virtual [**const\_iterator**](classendstone_1_1Registry.md#typedef-const_iterator) | [**begin**](#function-begin-22) () const = 0<br>_Const begin iterator._  |
| virtual [**iterator**](classendstone_1_1Registry.md#typedef-iterator) | [**end**](#function-end-12) () = 0<br>_Returns iterator to end of registry items._  |
| virtual [**const\_iterator**](classendstone_1_1Registry.md#typedef-const_iterator) | [**end**](#function-end-22) () const = 0<br>_Const end iterator._  |
| virtual [**T**](classendstone_1_1Vector.md) \* | [**get**](#function-get-12) (key\_type key) noexcept = 0<br>_Get the object by its key._  |
| virtual [**const**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) \* | [**get**](#function-get-22) (key\_type key) noexcept const = 0<br>_Const overload of_ [_**get()**_](classendstone_1_1Registry.md#function-get-12) _._ |
| virtual [**T**](classendstone_1_1Vector.md) & | [**getOrThrow**](#function-getorthrow-12) (key\_type key) <br>_Get the object by its key or throw if missing._  |
| virtual [**const**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) & | [**getOrThrow**](#function-getorthrow-22) (key\_type key) const<br>_Const overload of_ [_**getOrThrow()**_](classendstone_1_1Registry.md#function-getorthrow-12) _._ |
| virtual  | [**~Registry**](#function-registry) () = default<br> |




## Protected Types

| Type | Name |
| ---: | :--- |
| typedef std::unordered\_map&lt; key\_type, value\_type &gt; | [**storage\_type**](#typedef-storage_type)  <br>_Underlying storage type._  |
























## Detailed Description


Provides lookup by key (nullable or exception-throwing) and iteration.




**Template parameters:**


* `T` Must satisfy Keyed, i.e. implement getKey(). 




    
## Public Types Documentation




### typedef const\_iterator 

_Const iterator over stored elements._ 
```C++
using endstone::Registry< T >::const_iterator =  typename storage_type::const_iterator;
```




<hr>



### typedef const\_reference 

```C++
using endstone::Registry< T >::const_reference =  const T &;
```




<hr>



### typedef iterator 

_Iterator over stored elements._ 
```C++
using endstone::Registry< T >::iterator =  typename storage_type::iterator;
```




<hr>



### typedef key\_type 

```C++
using endstone::Registry< T >::key_type =  std::string_view;
```




<hr>



### typedef reference 

```C++
using endstone::Registry< T >::reference =  T &;
```




<hr>



### typedef value\_type 

```C++
using endstone::Registry< T >::value_type =  std::unique_ptr<T>;
```




<hr>
## Public Functions Documentation




### function begin [1/2]

_Returns iterator to beginning of registry items._ 
```C++
virtual iterator endstone::Registry::begin () = 0
```



Enables range-based for loops.




**Returns:**

iterator Begin iterator. 





        

<hr>



### function begin [2/2]

_Const begin iterator._ 
```C++
virtual const_iterator endstone::Registry::begin () const = 0
```





**Returns:**

const\_iterator Const begin iterator. 





        

<hr>



### function end [1/2]

_Returns iterator to end of registry items._ 
```C++
virtual iterator endstone::Registry::end () = 0
```





**Returns:**

iterator End iterator. 





        

<hr>



### function end [2/2]

_Const end iterator._ 
```C++
virtual const_iterator endstone::Registry::end () const = 0
```





**Returns:**

const\_iterator Const end iterator. 





        

<hr>



### function get [1/2]

_Get the object by its key._ 
```C++
virtual T * endstone::Registry::get (
    key_type key
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
virtual const  T * endstone::Registry::get (
    key_type key
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
    key_type key
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
inline virtual const  T & endstone::Registry::getOrThrow (
    key_type key
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
## Protected Types Documentation




### typedef storage\_type 

_Underlying storage type._ 
```C++
using endstone::Registry< T >::storage_type =  std::unordered_map<key_type, value_type>;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/registry.h`

