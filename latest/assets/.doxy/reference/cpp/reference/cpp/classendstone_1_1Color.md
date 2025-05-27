

# Class endstone::Color



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Color**](classendstone_1_1Color.md)



_Represents a color with red, green, blue, and alpha components._ 

* `#include <endstone/util/color.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Color**](#function-color) (const std::uint8\_t red=0, const std::uint8\_t green=0, const std::uint8\_t blue=0, const std::uint8\_t alpha=255) <br>_Creates a color with the specified red, green, blue, and alpha values in the range (0 - 255)._  |
|  std::uint8\_t | [**getAlpha**](#function-getalpha) () const<br>_Gets the alpha component of the color._  |
|  std::uint8\_t | [**getBlue**](#function-getblue) () const<br>_Gets the blue component of the color._  |
|  std::uint8\_t | [**getGreen**](#function-getgreen) () const<br>_Gets the green component of the color._  |
|  std::uint8\_t | [**getRed**](#function-getred) () const<br>_Gets the red component of the color._  |




























## Public Functions Documentation




### function Color 

_Creates a color with the specified red, green, blue, and alpha values in the range (0 - 255)._ 
```C++
inline explicit endstone::Color::Color (
    const std::uint8_t red=0,
    const std::uint8_t green=0,
    const std::uint8_t blue=0,
    const std::uint8_t alpha=255
) 
```





**Parameters:**


* `red` Red component (0–255) 
* `green` Green component (0–255) 
* `blue` Blue component (0–255) 
* `alpha` Alpha component (0–255, default 255 = opaque) 




        

<hr>



### function getAlpha 

_Gets the alpha component of the color._ 
```C++
inline std::uint8_t endstone::Color::getAlpha () const
```





**Returns:**

The alpha component (0–255) of the color. 





        

<hr>



### function getBlue 

_Gets the blue component of the color._ 
```C++
inline std::uint8_t endstone::Color::getBlue () const
```





**Returns:**

The blue component (0–255) of the color. 





        

<hr>



### function getGreen 

_Gets the green component of the color._ 
```C++
inline std::uint8_t endstone::Color::getGreen () const
```





**Returns:**

The green component (0–255) of the color. 





        

<hr>



### function getRed 

_Gets the red component of the color._ 
```C++
inline std::uint8_t endstone::Color::getRed () const
```





**Returns:**

The red component (0–255) of the color. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/color.h`

