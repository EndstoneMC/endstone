

# Class endstone::Criteria



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Criteria**](classendstone_1_1Criteria.md)



_Represents a scoreboard criteria._ 

* `#include <endstone/scoreboard/criteria.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
| virtual RenderType | [**getDefaultRenderType**](#function-getdefaultrendertype) () const = 0<br> |
| virtual std::string | [**getName**](#function-getname) () const = 0<br>_Get the name of this criteria (its unique id)._  |
| virtual bool | [**isReadOnly**](#function-isreadonly) () const = 0<br>_Get whether or not this criteria is read only. If read only, scoreboards with this criteria cannot have their scores changed._  |
| virtual  | [**~Criteria**](#function-criteria) () = default<br> |




























## Public Types Documentation




### enum Type 

```C++
enum endstone::Criteria::Type {
    Dummy
};
```




<hr>
## Public Functions Documentation




### function getDefaultRenderType 


```C++
virtual RenderType endstone::Criteria::getDefaultRenderType () const = 0
```



Get the RenderType used by default for this criteria.




**Returns:**

the default render type 





        

<hr>



### function getName 

_Get the name of this criteria (its unique id)._ 
```C++
virtual std::string endstone::Criteria::getName () const = 0
```





**Returns:**

the name 





        

<hr>



### function isReadOnly 

_Get whether or not this criteria is read only. If read only, scoreboards with this criteria cannot have their scores changed._ 
```C++
virtual bool endstone::Criteria::isReadOnly () const = 0
```





**Returns:**

true if read only, false otherwise 





        

<hr>



### function ~Criteria 

```C++
virtual endstone::Criteria::~Criteria () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/scoreboard/criteria.h`

