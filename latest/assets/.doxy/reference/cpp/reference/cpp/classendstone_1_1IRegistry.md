

# Class endstone::IRegistry



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**IRegistry**](classendstone_1_1IRegistry.md)



_Type-erased base interface for registries._ [More...](#detailed-description)

* `#include <endstone/registry.h>`





Inherited by the following classes: [endstone::Registry](classendstone_1_1Registry.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::size\_t | [**size**](#function-size) () const = 0<br>_Gets the number of entries in this registry._  |
| virtual  | [**~IRegistry**](#function-iregistry) () = default<br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**forEach0**](#function-foreach0) (std::function&lt; [**bool**](classendstone_1_1Identifier.md)([**const**](classendstone_1_1Identifier.md) [**void**](classendstone_1_1Identifier.md) \*)&gt; func) const = 0<br> |
| virtual [**const**](classendstone_1_1Identifier.md) [**void**](classendstone_1_1Identifier.md) \* | [**get0**](#function-get0) (std::string\_view id) noexcept const = 0<br> |
| virtual [**const**](classendstone_1_1Identifier.md) std::type\_info & | [**getTypeId**](#function-gettypeid) () noexcept const = 0<br> |




## Detailed Description


Allows registries of any element type to be handled uniformly without knowing the concrete element type. Prefer the typed [**Registry**](classendstone_1_1Registry.md) interface in plugin code. 


    
## Public Functions Documentation




### function size 

_Gets the number of entries in this registry._ 
```C++
virtual std::size_t endstone::IRegistry::size () const = 0
```





**Returns:**

the entry count 





        

<hr>



### function ~IRegistry 

```C++
virtual endstone::IRegistry::~IRegistry () = default
```




<hr>
## Protected Functions Documentation




### function forEach0 

```C++
virtual void endstone::IRegistry::forEach0 (
    std::function< bool ( const  void *)> func
) const = 0
```




<hr>



### function get0 

```C++
virtual const  void * endstone::IRegistry::get0 (
    std::string_view id
) noexcept const = 0
```




<hr>



### function getTypeId 

```C++
virtual const std::type_info & endstone::IRegistry::getTypeId () noexcept const = 0
```




<hr>## Friends Documentation





### friend PyRegistry 

```C++
class endstone::IRegistry::PyRegistry (
    python::PyRegistry
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/registry.h`

