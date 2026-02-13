

# Class endstone::Color



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Color**](classendstone_1_1Color.md)



_Represents a color with red, green, blue, and alpha components._ 

* `#include <endstone/util/color.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) | [**BIT\_MASK**](#variable-bit_mask)   = `0xff`<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) | [**DEFAULT\_ALPHA**](#variable-default_alpha)   = `255`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Color**](#function-color) ([**const**](classendstone_1_1Identifier.md) std::uint8\_t red, [**const**](classendstone_1_1Identifier.md) std::uint8\_t green, [**const**](classendstone_1_1Identifier.md) std::uint8\_t blue, [**const**](classendstone_1_1Identifier.md) std::uint8\_t alpha=DEFAULT\_ALPHA) <br> |
|  [**int**](classendstone_1_1Identifier.md) | [**asABGR**](#function-asabgr) () const<br>_Gets the color as an ABGR integer._  |
|  [**int**](classendstone_1_1Identifier.md) | [**asBGR**](#function-asbgr) () const<br>_Gets the color as an BGR integer._  |
|  [**int**](classendstone_1_1Identifier.md) | [**asRGB**](#function-asrgb) () const<br>_Gets the color as an RGB integer._  |
|  [**int**](classendstone_1_1Identifier.md) | [**asRGBA**](#function-asrgba) () const<br>_Gets the color as an RGBA integer._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getAlpha**](#function-getalpha) () const<br>_Gets the alpha component of the color._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getBlue**](#function-getblue) () const<br>_Gets the blue component of the color._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getGreen**](#function-getgreen) () const<br>_Gets the green component of the color._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getRed**](#function-getred) () const<br>_Gets the red component of the color._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Color**](classendstone_1_1Color.md) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**Color**](classendstone_1_1Color.md) & other) const<br> |
|  [**Color**](classendstone_1_1Color.md) | [**setAlpha**](#function-setalpha) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) alpha) const<br>_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object with specified component._ |
|  [**Color**](classendstone_1_1Color.md) | [**setBlue**](#function-setblue) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) blue) const<br>_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object with specified component._ |
|  [**Color**](classendstone_1_1Color.md) | [**setGreen**](#function-setgreen) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) green) const<br>_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object with specified component._ |
|  [**Color**](classendstone_1_1Color.md) | [**setRed**](#function-setred) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) red) const<br>_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object with specified component._ |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**Color**](classendstone_1_1Color.md) | [**fromABGR**](#function-fromabgr) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) abgr) <br>_Creates a new color object from an integer that contains the alpha, blue, green, and red bytes._  |
|  [**Color**](classendstone_1_1Color.md) | [**fromBGR**](#function-frombgr-12) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) blue, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) green, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) red) <br>_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object from a blue, green, and red._ |
|  [**Color**](classendstone_1_1Color.md) | [**fromBGR**](#function-frombgr-22) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) bgr) <br> |
|  [**Color**](classendstone_1_1Color.md) | [**fromRGB**](#function-fromrgb-12) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) red, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) green, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) blue) <br>_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object from a red, green, and blue._ |
|  [**Color**](classendstone_1_1Color.md) | [**fromRGB**](#function-fromrgb-22) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) rgb) <br>_Creates a new color object from an integer that contains the red, green, and blue bytes in the lowest order 24 bits._  |
|  [**Color**](classendstone_1_1Color.md) | [**fromRGBA**](#function-fromrgba-12) ([**int**](classendstone_1_1Identifier.md) red, [**int**](classendstone_1_1Identifier.md) green, [**int**](classendstone_1_1Identifier.md) blue, [**int**](classendstone_1_1Identifier.md) alpha) <br>_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object from a red, green, blue, and alpha._ |
|  [**Color**](classendstone_1_1Color.md) | [**fromRGBA**](#function-fromrgba-22) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) rgba) <br>_Creates a new color object from an integer that contains the red, green, blue, and alpha bytes._  |


























## Public Static Attributes Documentation




### variable BIT\_MASK 

```C++
constexpr int endstone::Color::BIT_MASK;
```




<hr>



### variable DEFAULT\_ALPHA 

```C++
constexpr int endstone::Color::DEFAULT_ALPHA;
```




<hr>
## Public Functions Documentation




### function Color 

```C++
inline constexpr endstone::Color::Color (
    const std::uint8_t red,
    const std::uint8_t green,
    const std::uint8_t blue,
    const std::uint8_t alpha=DEFAULT_ALPHA
) 
```




<hr>



### function asABGR 

_Gets the color as an ABGR integer._ 
```C++
inline int endstone::Color::asABGR () const
```





**Returns:**

An integer representation of this color, as 0xAABBGGRR 





        

<hr>



### function asBGR 

_Gets the color as an BGR integer._ 
```C++
inline int endstone::Color::asBGR () const
```





**Returns:**

An integer representation of this color, as 0xBBGGRR 





        

<hr>



### function asRGB 

_Gets the color as an RGB integer._ 
```C++
inline int endstone::Color::asRGB () const
```





**Returns:**

An integer representation of this color, as 0xRRGGBB 





        

<hr>



### function asRGBA 

_Gets the color as an RGBA integer._ 
```C++
inline int endstone::Color::asRGBA () const
```





**Returns:**

An integer representation of this color, as 0xRRGGBBAA 





        

<hr>



### function getAlpha 

_Gets the alpha component of the color._ 
```C++
inline int endstone::Color::getAlpha () const
```





**Returns:**

The alpha component (0–255) of the color. 





        

<hr>



### function getBlue 

_Gets the blue component of the color._ 
```C++
inline int endstone::Color::getBlue () const
```





**Returns:**

The blue component (0–255) of the color. 





        

<hr>



### function getGreen 

_Gets the green component of the color._ 
```C++
inline int endstone::Color::getGreen () const
```





**Returns:**

The green component (0–255) of the color. 





        

<hr>



### function getRed 

_Gets the red component of the color._ 
```C++
inline int endstone::Color::getRed () const
```





**Returns:**

The red component (0–255) of the color. 





        

<hr>



### function operator!= 

```C++
inline bool endstone::Color::operator!= (
    const  Color & other
) const
```




<hr>



### function operator== 

```C++
inline bool endstone::Color::operator== (
    const  Color & other
) const
```




<hr>



### function setAlpha 

_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object with specified component._
```C++
inline Color endstone::Color::setAlpha (
    const  int alpha
) const
```





**Parameters:**


* `alpha` the alpha component, from 0 to 255



**Returns:**

a new color object with the alpha component 





        

<hr>



### function setBlue 

_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object with specified component._
```C++
inline Color endstone::Color::setBlue (
    const  int blue
) const
```





**Parameters:**


* `blue` the blue component, from 0 to 255



**Returns:**

a new color object with the blue component 





        

<hr>



### function setGreen 

_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object with specified component._
```C++
inline Color endstone::Color::setGreen (
    const  int green
) const
```





**Parameters:**


* `green` the green component, from 0 to 255



**Returns:**

a new color object with the green component 





        

<hr>



### function setRed 

_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object with specified component._
```C++
inline Color endstone::Color::setRed (
    const  int red
) const
```





**Parameters:**


* `red` the red component, from 0 to 255



**Returns:**

a new color object with the red component 





        

<hr>
## Public Static Functions Documentation




### function fromABGR 

_Creates a new color object from an integer that contains the alpha, blue, green, and red bytes._ 
```C++
static inline Color endstone::Color::fromABGR (
    const  int abgr
) 
```





**Parameters:**


* `abgr` the integer storing the alpha, blue, green, and red values



**Returns:**

a new color object for specified values 





        

<hr>



### function fromBGR [1/2]

_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object from a blue, green, and red._
```C++
static inline Color endstone::Color::fromBGR (
    const  int blue,
    const  int green,
    const  int red
) 
```





**Parameters:**


* `blue` integer from 0-255 
* `green` integer from 0-255 
* `red` integer from 0-255



**Returns:**

a new [**Color**](classendstone_1_1Color.md) object for the red, green, blue 





        

<hr>



### function fromBGR [2/2]

```C++
static inline Color endstone::Color::fromBGR (
    const  int bgr
) 
```




<hr>



### function fromRGB [1/2]

_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object from a red, green, and blue._
```C++
static inline Color endstone::Color::fromRGB (
    const  int red,
    const  int green,
    const  int blue
) 
```





**Parameters:**


* `red` integer from 0-255 
* `green` integer from 0-255 
* `blue` integer from 0-255



**Returns:**

a new [**Color**](classendstone_1_1Color.md) object for the red, green, blue 





        

<hr>



### function fromRGB [2/2]

_Creates a new color object from an integer that contains the red, green, and blue bytes in the lowest order 24 bits._ 
```C++
static inline Color endstone::Color::fromRGB (
    const  int rgb
) 
```





**Parameters:**


* `rgb` the integer storing the red, green, and blue values



**Returns:**

a new color object for specified values 





        

<hr>



### function fromRGBA [1/2]

_Creates a new_ [_**Color**_](classendstone_1_1Color.md) _object from a red, green, blue, and alpha._
```C++
static inline Color endstone::Color::fromRGBA (
    int red,
    int green,
    int blue,
    int alpha
) 
```





**Parameters:**


* `red` integer from 0-255 
* `green` integer from 0-255 
* `blue` integer from 0-255 
* `alpha` integer from 0-255



**Returns:**

a new [**Color**](classendstone_1_1Color.md) object for the red, green, blue, alpha 





        

<hr>



### function fromRGBA [2/2]

_Creates a new color object from an integer that contains the red, green, blue, and alpha bytes._ 
```C++
static inline Color endstone::Color::fromRGBA (
    const  int rgba
) 
```





**Parameters:**


* `rgba` the integer storing the red, green, blue, and alpha values



**Returns:**

a new color object for specified values 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/color.h`

