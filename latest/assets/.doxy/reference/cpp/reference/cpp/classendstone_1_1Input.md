

# Class endstone::Input



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Input**](classendstone_1_1Input.md)



_Represents a movement input applied to a player._ 

* `#include <endstone/input.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Input**](#function-input-14) () = default<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Input**](#function-input-24) ([**bool**](classendstone_1_1Identifier.md) forward, [**bool**](classendstone_1_1Identifier.md) backward, [**bool**](classendstone_1_1Identifier.md) left, [**bool**](classendstone_1_1Identifier.md) right, [**bool**](classendstone_1_1Identifier.md) jump, [**bool**](classendstone_1_1Identifier.md) sneak, [**bool**](classendstone_1_1Identifier.md) sprint) <br> |
|   | [**Input**](#function-input-34) ([**const**](classendstone_1_1Identifier.md) [**Input**](classendstone_1_1Input.md) &) = default<br> |
|   | [**Input**](#function-input-44) ([**Input**](classendstone_1_1Input.md) &&) noexcept<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isBackward**](#function-isbackward) () const<br>_Gets whether a backward input is applied._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isForward**](#function-isforward) () const<br>_Gets whether a forward input is applied._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isJump**](#function-isjump) () const<br>_Gets whether a jump input is applied._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isLeft**](#function-isleft) () const<br>_Gets whether a left input is applied._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isRight**](#function-isright) () const<br>_Gets whether a right input is applied._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isSneak**](#function-issneak) () const<br>_Gets whether a sneak input is applied._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isSprint**](#function-issprint) () const<br>_Gets whether a sprint input is applied._  |
|  [**Input**](classendstone_1_1Input.md) & | [**operator=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Input**](classendstone_1_1Input.md) &) = default<br> |
|  [**Input**](classendstone_1_1Input.md) & | [**operator=**](#function-operator_1) ([**Input**](classendstone_1_1Input.md) &&) noexcept<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_2) ([**const**](classendstone_1_1Identifier.md) [**Input**](classendstone_1_1Input.md) &) const<br> |




























## Public Functions Documentation




### function Input [1/4]

```C++
constexpr endstone::Input::Input () = default
```




<hr>



### function Input [2/4]

```C++
inline constexpr endstone::Input::Input (
    bool forward,
    bool backward,
    bool left,
    bool right,
    bool jump,
    bool sneak,
    bool sprint
) 
```




<hr>



### function Input [3/4]

```C++
endstone::Input::Input (
    const  Input &
) = default
```




<hr>



### function Input [4/4]

```C++
endstone::Input::Input (
    Input &&
) noexcept
```




<hr>



### function isBackward 

_Gets whether a backward input is applied._ 
```C++
inline constexpr  bool endstone::Input::isBackward () const
```





**Returns:**

true if a backward input is applied 





        

<hr>



### function isForward 

_Gets whether a forward input is applied._ 
```C++
inline constexpr  bool endstone::Input::isForward () const
```





**Returns:**

true if a forward input is applied 





        

<hr>



### function isJump 

_Gets whether a jump input is applied._ 
```C++
inline constexpr  bool endstone::Input::isJump () const
```





**Returns:**

true if a jump input is applied 





        

<hr>



### function isLeft 

_Gets whether a left input is applied._ 
```C++
inline constexpr  bool endstone::Input::isLeft () const
```





**Returns:**

true if a left input is applied 





        

<hr>



### function isRight 

_Gets whether a right input is applied._ 
```C++
inline constexpr  bool endstone::Input::isRight () const
```





**Returns:**

true if a right input is applied 





        

<hr>



### function isSneak 

_Gets whether a sneak input is applied._ 
```C++
inline constexpr  bool endstone::Input::isSneak () const
```





**Returns:**

true if a sneak input is applied 





        

<hr>



### function isSprint 

_Gets whether a sprint input is applied._ 
```C++
inline constexpr  bool endstone::Input::isSprint () const
```





**Returns:**

true if a sprint input is applied 





        

<hr>



### function operator= 

```C++
Input & endstone::Input::operator= (
    const  Input &
) = default
```




<hr>



### function operator= 

```C++
Input & endstone::Input::operator= (
    Input &&
) noexcept
```




<hr>



### function operator== 

```C++
constexpr  bool endstone::Input::operator== (
    const  Input &
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/input.h`

