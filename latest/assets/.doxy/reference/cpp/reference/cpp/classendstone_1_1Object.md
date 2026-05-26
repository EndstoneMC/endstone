

# Class endstone::Object



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Object**](classendstone_1_1Object.md)



_Base class providing runtime type identification and safe casting._ [More...](#detailed-description)

* `#include <endstone/object.h>`





Inherited by the following classes: [endstone::ItemMeta](classendstone_1_1ItemMeta.md),  [endstone::Permissible](classendstone_1_1Permissible.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**T**](classendstone_1_1Identifier.md) \* | [**as**](#function-as-12) () <br>_Attempts to cast this object to the given type T._  |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**as**](#function-as-22) () const<br>_Attempts to cast this object to the given type T._  |
| virtual [**const**](classendstone_1_1Identifier.md) std::type\_info & | [**getClassTypeId**](#function-getclasstypeid) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**is**](#function-is) () const<br>_Checks if this object is an instance of the given type T (or a subclass of T)._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](#function-isinstanceof) ([**const**](classendstone_1_1Identifier.md) std::type\_info & target) const = 0<br> |
| virtual  | [**~Object**](#function-object) () = default<br> |




























## Detailed Description


All endstone types that support runtime type narrowing (e.g., [**Permissible**](classendstone_1_1Permissible.md), [**ItemMeta**](classendstone_1_1ItemMeta.md)) inherit from [**Object**](classendstone_1_1Object.md). Subclasses implement getClassTypeId() and isInstanceOf(), and the type hierarchy is registered with entt::meta at startup. 


    
## Public Functions Documentation




### function as [1/2]

_Attempts to cast this object to the given type T._ 
```C++
template<typename  T>
inline T * endstone::Object::as () 
```



Returns nullptr if this object is not an instance of T (or a subclass of T).




**Template parameters:**


* `T` Target type to cast to (must derive from [**Object**](classendstone_1_1Object.md)) 



**Returns:**

Pointer to this as T, or nullptr if the cast is invalid 





        

<hr>



### function as [2/2]

_Attempts to cast this object to the given type T._ 
```C++
template<typename  T>
inline const  T * endstone::Object::as () const
```



Returns nullptr if this object is not an instance of T (or a subclass of T).




**Template parameters:**


* `T` Target type to cast to (must derive from [**Object**](classendstone_1_1Object.md)) 



**Returns:**

Pointer to this as T, or nullptr if the cast is invalid   





        

<hr>



### function getClassTypeId 

```C++
virtual const std::type_info & endstone::Object::getClassTypeId () const = 0
```




<hr>



### function is 

_Checks if this object is an instance of the given type T (or a subclass of T)._ 
```C++
template<typename  T>
inline bool endstone::Object::is () const
```





**Template parameters:**


* `T` Type to check against (must derive from [**Object**](classendstone_1_1Object.md)) 



**Returns:**

True if this object is an instance of T 





        

<hr>



### function isInstanceOf 

```C++
virtual bool endstone::Object::isInstanceOf (
    const std::type_info & target
) const = 0
```




<hr>



### function ~Object 

```C++
virtual endstone::Object::~Object () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/object.h`

