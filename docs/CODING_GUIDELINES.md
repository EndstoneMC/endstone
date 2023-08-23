# C++ Coding Guidelines

This document outlines the coding standards and conventions for our C++ project.

## 1. Code Formatting with clang-format

All code should be formatted according to the project's `.clang-format` file. Before committing your changes, make sure
to run:

```bash
clang-format -i [file or directory]
```

This will format your C++ files in-place according to our project's style guide. Adherence to the style guide is
mandatory for all contributions.

## 2. Braces

We use the [Stroustrup](https://en.wikipedia.org/wiki/Indent_style#Variant:_Stroustrup) style. Open braces go on the
same line.

```cpp
if (condition) {
    // code
}
else {
    // code
}
```

## 3. Naming Conventions

Avoid using any sort of Hungarian notation on names and "_ptr" on pointers.

| **Code Element**         | **Style**                                     |
|--------------------------|-----------------------------------------------|
| Namespace                | under_scored                                  |
| Class name               | CamelCase                                     |
| Function name            | camelCase                                     |
| Parameters/Locals        | under_scored                                  |
| Member variables         | under_scored_with_                            |
| Enum classes and members | CamelCase                                     |
| Globals                  | g_under_scored                                |
| Constants                | UPPER_CASE                                    |
| File names               | under_scored (if the class name is CamelCase) |

## 4. Documentation

### 4.1 Comments

Code should be self-documenting, but when comments are needed, they should explain why something is done, not what is
done.

### 4.2 Function Comments

Functions should have a brief comment describing what they do, their parameters, and their return value.

```cpp
/**
 * @brief Performs some computation
 * 
 * @param x Input parameter
 * @return The result of the computation
 */
int computeSomething(int x);
```

## 5. Code Structure

### 5.1 Include Guard

Always use include guards in header files. The naming convention for the include guards should
be `PROJECT_NAME_HEADER_NAME_H`.

```cpp
#ifndef PROJECT_NAME_HEADER_NAME_H
#define PROJECT_NAME_HEADER_NAME_H

// header content

#endif // PROJECT_NAME_HEADER_NAME_H
```

### 5.2 Standard Headers

Standard headers should be included using angle brackets (`< >`), while project headers should be included using double
quotes (`" "`). Ensure you regroup the `#include` blocks, with the main file header first, followed by project headers
in double quotes and then system headers with angle brackets.

### 5.3 Functions

Functions should be short and do one thing. If a function becomes too long or complex, consider breaking it up into
smaller functions.

## 6. Error Handling

### 6.1 Exceptions

Use exceptions for handling errors whenever possible and reasonable.

### 6.2 Error Codes

If you cannot use exceptions, use error codes, but make sure to document the possible error codes a function can return.
