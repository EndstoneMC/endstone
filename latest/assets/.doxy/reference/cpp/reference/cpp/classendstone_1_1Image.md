

# Class endstone::Image



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Image**](classendstone_1_1Image.md)



_Represents an RGBA image._ [More...](#detailed-description)

* `#include <endstone/util/image.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Image**](#function-image-12) ([**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) width, [**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) height) <br>_Create an empty image (all pixels transparent black)._  |
|  [**Color**](classendstone_1_1Color.md) | [**getColor**](#function-getcolor) ([**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) x, [**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) y) const<br>_Get the color of a pixel._  |
|  std::string\_view | [**getData**](#function-getdata) () const<br>_Gets the raw pixel buffer (RGBA, row-major)._  |
|  [**int**](classendstone_1_1Vector.md) | [**getHeight**](#function-getheight) () const<br>_Get the image height._  |
|  [**int**](classendstone_1_1Vector.md) | [**getWidth**](#function-getwidth) () const<br>_Get the image width._  |
|  [**void**](classendstone_1_1Vector.md) | [**setColor**](#function-setcolor) ([**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) x, [**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) y, [**const**](classendstone_1_1Vector.md) [**Color**](classendstone_1_1Color.md) color) <br>_Set the color of a pixel._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Result&lt; [**Image**](classendstone_1_1Image.md) &gt; | [**fromBuffer**](#function-frombuffer) ([**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) width, [**const**](classendstone_1_1Vector.md) [**int**](classendstone_1_1Vector.md) height, std::string\_view buffer) <br>_Creates an image from the pixel data in a byte buffer._  |


























## Detailed Description


Each pixel is four bytes: R, G, B, A, in row-major order. 


    
## Public Functions Documentation




### function Image [1/2]

_Create an empty image (all pixels transparent black)._ 
```C++
inline endstone::Image::Image (
    const  int width,
    const  int height
) 
```





**Parameters:**


* `width` [**Image**](classendstone_1_1Image.md) width in pixels 
* `height` [**Image**](classendstone_1_1Image.md) height in pixels 




        

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

_Gets the raw pixel buffer (RGBA, row-major)._ 
```C++
inline std::string_view endstone::Image::getData () const
```





**Returns:**

The underlying image data buffer 





        

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




### function fromBuffer 

_Creates an image from the pixel data in a byte buffer._ 
```C++
static inline Result< Image > endstone::Image::fromBuffer (
    const  int width,
    const  int height,
    std::string_view buffer
) 
```





**Parameters:**


* `width` [**Image**](classendstone_1_1Image.md) width in pixels 
* `height` [**Image**](classendstone_1_1Image.md) height in pixels 
* `buffer` A string\_view over exactly width\*height\*4 bytes of RGBA data 



**Returns:**

The image on success, or an error on failure. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/image.h`

