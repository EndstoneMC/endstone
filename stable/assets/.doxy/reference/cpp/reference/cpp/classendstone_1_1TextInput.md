

# Class endstone::TextInput



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**TextInput**](classendstone_1_1TextInput.md)



_Represents a text input field._ 

* `#include <endstone/form/controls/text_input.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TextInput**](#function-textinput-12) () = default<br> |
|   | [**TextInput**](#function-textinput-22) (Message label, Message placeholder, std::optional&lt; std::string &gt; default\_text=std::nullopt) <br> |
|  std::optional&lt; std::string &gt; | [**getDefaultValue**](#function-getdefaultvalue) () const<br>_Gets the default text of the text input field._  |
|  Message | [**getLabel**](#function-getlabel) () const<br>_Gets the label of the text input field._  |
|  Message | [**getPlaceholder**](#function-getplaceholder) () const<br>_Gets the placeholder of the text input field._  |
|  [**TextInput**](classendstone_1_1TextInput.md) & | [**setDefaultValue**](#function-setdefaultvalue) (std::optional&lt; std::string &gt; text) <br>_Sets the default text of the text input field._  |
|  [**TextInput**](classendstone_1_1TextInput.md) & | [**setLabel**](#function-setlabel) (Message label) <br>_Sets the label of the text input field._  |
|  [**TextInput**](classendstone_1_1TextInput.md) & | [**setPlaceholder**](#function-setplaceholder) (Message placeholder) <br>_Sets the placeholder of the text input field._  |




























## Public Functions Documentation




### function TextInput [1/2]

```C++
endstone::TextInput::TextInput () = default
```




<hr>



### function TextInput [2/2]

```C++
inline explicit endstone::TextInput::TextInput (
    Message label,
    Message placeholder,
    std::optional< std::string > default_text=std::nullopt
) 
```




<hr>



### function getDefaultValue 

_Gets the default text of the text input field._ 
```C++
inline std::optional< std::string > endstone::TextInput::getDefaultValue () const
```





**Returns:**

The default text of the text input field. 





        

<hr>



### function getLabel 

_Gets the label of the text input field._ 
```C++
inline Message endstone::TextInput::getLabel () const
```





**Returns:**

The label of the text input field. 





        

<hr>



### function getPlaceholder 

_Gets the placeholder of the text input field._ 
```C++
inline Message endstone::TextInput::getPlaceholder () const
```





**Returns:**

The placeholder of the text input field. 





        

<hr>



### function setDefaultValue 

_Sets the default text of the text input field._ 
```C++
inline TextInput & endstone::TextInput::setDefaultValue (
    std::optional< std::string > text
) 
```





**Parameters:**


* `text` The new default text for the text input field. 



**Returns:**

A reference to the text input field itself. 





        

<hr>



### function setLabel 

_Sets the label of the text input field._ 
```C++
inline TextInput & endstone::TextInput::setLabel (
    Message label
) 
```





**Parameters:**


* `label` The new label for the text input field. 



**Returns:**

A reference to the text input field itself. 





        

<hr>



### function setPlaceholder 

_Sets the placeholder of the text input field._ 
```C++
inline TextInput & endstone::TextInput::setPlaceholder (
    Message placeholder
) 
```





**Parameters:**


* `placeholder` The new placeholder for the text input field. 



**Returns:**

A reference to the text input field itself. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/controls/text_input.h`

