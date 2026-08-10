

# Class endstone::SignSide



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**SignSide**](classendstone_1_1SignSide.md)



_Represents a side of a sign._ 

* `#include <endstone/block/sign/sign_side.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Color**](classendstone_1_1Color.md) | [**getColor**](#function-getcolor) () const = 0<br>_Gets the color of this side of the sign._  |
| virtual std::string | [**getLine**](#function-getline) ([**int**](classendstone_1_1Identifier.md) index) const = 0<br>_Gets the line of text at the specified index on this side of the sign._  |
| virtual std::vector&lt; std::string &gt; | [**getLines**](#function-getlines) () const = 0<br>_Gets all the lines of text currently on this side of the sign._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isGlowingText**](#function-isglowingtext) () const = 0<br>_Gets whether this side of the sign has glowing text._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setColor**](#function-setcolor) ([**Color**](classendstone_1_1Color.md) color) = 0<br>_Sets the color of this side of the sign._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setGlowingText**](#function-setglowingtext) ([**bool**](classendstone_1_1Identifier.md) glowing) = 0<br>_Sets whether this side of the sign has glowing text._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setLine**](#function-setline) ([**int**](classendstone_1_1Identifier.md) index, std::string line) = 0<br>_Sets the line of text at the specified index on this side of the sign._  |
| virtual  | [**~SignSide**](#function-signside) () = default<br> |




























## Public Functions Documentation




### function getColor 

_Gets the color of this side of the sign._ 
```C++
virtual Color endstone::SignSide::getColor () const = 0
```



Bedrock stores the text color of a sign as a color rather than as one of the dyes, so this returns a [**Color**](classendstone_1_1Color.md) where Bukkit returns a DyeColor.




**Returns:**

the color 





        

<hr>



### function getLine 

_Gets the line of text at the specified index on this side of the sign._ 
```C++
virtual std::string endstone::SignSide::getLine (
    int index
) const = 0
```



For example, `getLine(0)` will return the first line of text.




**Parameters:**


* `index` the index of the line to get, between 0 and 3 



**Returns:**

the text of the line, empty if the line is blank 





        

<hr>



### function getLines 

_Gets all the lines of text currently on this side of the sign._ 
```C++
virtual std::vector< std::string > endstone::SignSide::getLines () const = 0
```





**Returns:**

the lines of text 





        

<hr>



### function isGlowingText 

_Gets whether this side of the sign has glowing text._ 
```C++
virtual bool endstone::SignSide::isGlowingText () const = 0
```





**Returns:**

`true` if this side of the sign has glowing text 





        

<hr>



### function setColor 

_Sets the color of this side of the sign._ 
```C++
virtual void endstone::SignSide::setColor (
    Color color
) = 0
```





**Parameters:**


* `color` the color to set 




        

<hr>



### function setGlowingText 

_Sets whether this side of the sign has glowing text._ 
```C++
virtual void endstone::SignSide::setGlowingText (
    bool glowing
) = 0
```





**Parameters:**


* `glowing` whether this side of the sign has glowing text 




        

<hr>



### function setLine 

_Sets the line of text at the specified index on this side of the sign._ 
```C++
virtual void endstone::SignSide::setLine (
    int index,
    std::string line
) = 0
```



For example, `setLine(0, "Line One")` will set the first line to "Line One".




**Parameters:**


* `index` the index of the line to set, between 0 and 3 
* `line` the new text to set 




        

<hr>



### function ~SignSide 

```C++
virtual endstone::SignSide::~SignSide () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/block/sign/sign_side.h`

