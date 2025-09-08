

# Class endstone::MapCanvas



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**MapCanvas**](classendstone_1_1MapCanvas.md)



_Represents a canvas for drawing to a map. Each canvas is associated with a specific_ [_**MapRenderer**_](classendstone_1_1MapRenderer.md) _and represents that renderer's layer on the map._

* `#include <endstone/map/map_canvas.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**drawImage**](#function-drawimage) (int x, int y, const [**Image**](classendstone_1_1Image.md) & image) = 0<br>_Draw an image to the map. The image will be clipped if necessary._  |
| virtual int | [**getBasePixel**](#function-getbasepixel) (int x, int y) const = 0<br>_Get a pixel from the layers below this canvas._  |
| virtual [**Color**](classendstone_1_1Color.md) | [**getBasePixelColor**](#function-getbasepixelcolor) (int x, int y) const = 0<br>_Get a pixel from the layers below this canvas._  |
| virtual [**MapView**](classendstone_1_1MapView.md) & | [**getMapView**](#function-getmapview) () const = 0<br>_Get the map this canvas is attached to._  |
| virtual int | [**getPixel**](#function-getpixel) (int x, int y) const = 0<br>_Get a pixel from the canvas._  |
| virtual std::optional&lt; [**Color**](classendstone_1_1Color.md) &gt; | [**getPixelColor**](#function-getpixelcolor) (int x, int y) const = 0<br>_Get a pixel from the canvas._  |
| virtual void | [**setPixel**](#function-setpixel) (int x, int y, int color) = 0<br>_Draw a pixel to the canvas._  |
| virtual void | [**setPixelColor**](#function-setpixelcolor) (int x, int y, std::optional&lt; [**Color**](classendstone_1_1Color.md) &gt; color) = 0<br>_Draw a pixel to the canvas._  |
| virtual  | [**~MapCanvas**](#function-mapcanvas) () = default<br> |




























## Public Functions Documentation




### function drawImage 

_Draw an image to the map. The image will be clipped if necessary._ 
```C++
virtual void endstone::MapCanvas::drawImage (
    int x,
    int y,
    const Image & image
) = 0
```





**Parameters:**


* `x` The x coordinate of the image. 
* `y` The y coordinate of the image. 
* `image` The [**Image**](classendstone_1_1Image.md) to draw. 




        

<hr>



### function getBasePixel 

_Get a pixel from the layers below this canvas._ 
```C++
virtual int endstone::MapCanvas::getBasePixel (
    int x,
    int y
) const = 0
```





**Parameters:**


* `x` The x coordinate, from 0 to 127. 
* `y` The y coordinate, from 0 to 127. 



**Returns:**

The color. 





        

<hr>



### function getBasePixelColor 

_Get a pixel from the layers below this canvas._ 
```C++
virtual Color endstone::MapCanvas::getBasePixelColor (
    int x,
    int y
) const = 0
```





**Parameters:**


* `x` The x coordinate, from 0 to 127.
* `y` The y coordinate, from 0 to 127. 



**Returns:**

The color. 





        

<hr>



### function getMapView 

_Get the map this canvas is attached to._ 
```C++
virtual MapView & endstone::MapCanvas::getMapView () const = 0
```





**Returns:**

The [**MapView**](classendstone_1_1MapView.md) this canvas is attached to. 





        

<hr>



### function getPixel 

_Get a pixel from the canvas._ 
```C++
virtual int endstone::MapCanvas::getPixel (
    int x,
    int y
) const = 0
```





**Parameters:**


* `x` The x coordinate, from 0 to 127. 
* `y` The y coordinate, from 0 to 127. 



**Returns:**

The color. 





        

<hr>



### function getPixelColor 

_Get a pixel from the canvas._ 
```C++
virtual std::optional< Color > endstone::MapCanvas::getPixelColor (
    int x,
    int y
) const = 0
```



If no color is set at the given position for this canvas, then std::nullopt is returned and the color returned by [**getBasePixelColor()**](classendstone_1_1MapCanvas.md#function-getbasepixelcolor) is shown on the map.




**Parameters:**


* `x` The x coordinate, from 0 to 127. 
* `y` The y coordinate, from 0 to 127. 



**Returns:**

The color, or std::nullopt if no color is set. 





        

<hr>



### function setPixel 

_Draw a pixel to the canvas._ 
```C++
virtual void endstone::MapCanvas::setPixel (
    int x,
    int y,
    int color
) = 0
```





**Parameters:**


* `x` The x coordinate, from 0 to 127. 
* `y` The y coordinate, from 0 to 127. 
* `color` The color. 




        

<hr>



### function setPixelColor 

_Draw a pixel to the canvas._ 
```C++
virtual void endstone::MapCanvas::setPixelColor (
    int x,
    int y,
    std::optional< Color > color
) = 0
```



If std::nullopt is used as color, then the color returned by [**getBasePixelColor()**](classendstone_1_1MapCanvas.md#function-getbasepixelcolor) is shown on the map.




**Parameters:**


* `x` The x coordinate, from 0 to 127. 
* `y` The y coordinate, from 0 to 127. 
* `color` The color, or std::nullopt for base color. 




        

<hr>



### function ~MapCanvas 

```C++
virtual endstone::MapCanvas::~MapCanvas () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/map/map_canvas.h`

