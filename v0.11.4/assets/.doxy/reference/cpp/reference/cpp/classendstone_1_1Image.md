

# Class endstone::Image



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Image**](classendstone_1_1Image.md)



_Represents an RGBA image._ [More...](#detailed-description)

* `#include <endstone/util/image.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Image**](#function-image-12) ([**const**](classendstone_1_1Identifier.md) [**Type**](classendstone_1_1Image.md#enum-type) type, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) width, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) height) <br>_Create an empty image (all pixels transparent black)._  |
|   | [**Image**](#function-image-22) ([**Type**](classendstone_1_1Image.md#enum-type) type, [**int**](classendstone_1_1Identifier.md) width, [**int**](classendstone_1_1Identifier.md) height, [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) & data) <br> |
|  [**Color**](classendstone_1_1Color.md) | [**getColor**](#function-getcolor) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) x, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) y) const<br>_Get the color of a pixel._  |
|  std::string\_view | [**getData**](#function-getdata) () const<br>_Gets the raw pixel buffer (row-major, H x W x D)._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getDepth**](#function-getdepth) () const<br>_Get the image depth._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getHeight**](#function-getheight) () const<br>_Get the image height._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getWidth**](#function-getwidth) () const<br>_Get the image width._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setColor**](#function-setcolor) ([**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) x, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) y, [**const**](classendstone_1_1Identifier.md) [**Color**](classendstone_1_1Color.md) color) <br>_Set the color of a pixel._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Result&lt; [**Image**](classendstone_1_1Image.md) &gt; | [**fromArray**](#function-fromarray) ([**Type**](classendstone_1_1Image.md#enum-type) type, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) width, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) height, [**const**](classendstone_1_1Identifier.md) std::vector&lt; [**unsigned**](classendstone_1_1Identifier.md) [**char**](classendstone_1_1Identifier.md) &gt; & array) <br>_Creates an image from the pixel data in a byte array._  |
|  Result&lt; [**Image**](classendstone_1_1Image.md) &gt; | [**fromBuffer**](#function-frombuffer) ([**Type**](classendstone_1_1Image.md#enum-type) type, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) width, [**const**](classendstone_1_1Identifier.md) [**int**](classendstone_1_1Identifier.md) height, std::string\_view buffer) <br>_Creates an image from the pixel data in a byte buffer._  |


























## Detailed Description


Each pixel is four bytes: R, G, B, A, in row-major order. 


    
## Public Types Documentation




### enum Type 

```C++
enum endstone::Image::Type {
    Invalid = 0,
    Grayscale = 1,
    RGB = 3,
    RGBA = 4
};
```




<hr>
## Public Functions Documentation




### function Image [1/2]

_Create an empty image (all pixels transparent black)._ 
```C++
inline explicit endstone::Image::Image (
    const  Type type,
    const  int width,
    const  int height
) 
```





**Parameters:**


* `type` [**Image**](classendstone_1_1Image.md) type 
* `width` [**Image**](classendstone_1_1Image.md) width in pixels 
* `height` [**Image**](classendstone_1_1Image.md) height in pixels 




        

<hr>



### function Image [2/2]

```C++
template<typename  T>
inline endstone::Image::Image (
    Type type,
    int width,
    int height,
    const  T & data
) 
```




<hr>



### function getColor 

_Get the color of a pixel._ 
```C++
inline Color endstone::Image::getColor (
    const  int x,
    const  int y
) const
```





**Parameters:**


* `x` X coordinate (0 ≤ x &lt; width) 
* `y` Y coordinate (0 ≤ y &lt; height)



**Returns:**

[**Color**](classendstone_1_1Color.md) at (x, y) 





        

<hr>



### function getData 

_Gets the raw pixel buffer (row-major, H x W x D)._ 
```C++
inline std::string_view endstone::Image::getData () const
```





**Returns:**

The underlying image data buffer 





        

<hr>



### function getDepth 

_Get the image depth._ 
```C++
inline int endstone::Image::getDepth () const
```





**Returns:**

[**Image**](classendstone_1_1Image.md) depth 





        

<hr>



### function getHeight 

_Get the image height._ 
```C++
inline int endstone::Image::getHeight () const
```





**Returns:**

[**Image**](classendstone_1_1Image.md) height in pixels 





        

<hr>



### function getWidth 

_Get the image width._ 
```C++
inline int endstone::Image::getWidth () const
```





**Returns:**

[**Image**](classendstone_1_1Image.md) width in pixels 





        

<hr>



### function setColor 

_Set the color of a pixel._ 
```C++
inline void endstone::Image::setColor (
    const  int x,
    const  int y,
    const  Color color
) 
```





**Parameters:**


* `x` X coordinate (0 ≤ x &lt; width) 
* `y` Y coordinate (0 ≤ y &lt; height) 
* `color` New color for the pixel 




        

<hr>
## Public Static Functions Documentation




### function fromArray 

_Creates an image from the pixel data in a byte array._ 
```C++
static inline Result< Image > endstone::Image::fromArray (
    Type type,
    const  int width,
    const  int height,
    const std::vector< unsigned  char > & array
) 
```





**Parameters:**


* `type` [**Image**](classendstone_1_1Image.md) type 
* `width` [**Image**](classendstone_1_1Image.md) width in pixels 
* `height` [**Image**](classendstone_1_1Image.md) height in pixels 
* `array` 



**Returns:**

The image on success, or an error on failure. 





        

<hr>



### function fromBuffer 

_Creates an image from the pixel data in a byte buffer._ 
```C++
static inline Result< Image > endstone::Image::fromBuffer (
    Type type,
    const  int width,
    const  int height,
    std::string_view buffer
) 
```





**Parameters:**


* `type` [**Image**](classendstone_1_1Image.md) type 
* `width` [**Image**](classendstone_1_1Image.md) width in pixels 
* `height` [**Image**](classendstone_1_1Image.md) height in pixels 
* `buffer` 



**Returns:**

The image on success, or an error on failure. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/image.h`

