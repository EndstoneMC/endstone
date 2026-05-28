

# Class endstone::Task



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Task**](classendstone_1_1Task.md)



_Represents a task being executed by the scheduler._ 

* `#include <endstone/scheduler/task.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](#function-cancel) () = 0<br>_Attempts to cancel this task._  |
| virtual [**Plugin**](classendstone_1_1Plugin.md) \* | [**getOwner**](#function-getowner) () const = 0<br>_Returns the_ [_**Plugin**_](classendstone_1_1Plugin.md) _that owns this task._ |
| virtual TaskId | [**getTaskId**](#function-gettaskid) () const = 0<br>_Returns the taskId for the task._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](#function-iscancelled) () const = 0<br>_Returns true if this task has been cancelled._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isSync**](#function-issync) () const = 0<br>_Returns true if the_ [_**Task**_](classendstone_1_1Task.md) _is a sync task._ |
| virtual  | [**~Task**](#function-task) () = default<br> |




























## Public Functions Documentation




### function cancel 

_Attempts to cancel this task._ 
```C++
virtual void endstone::Task::cancel () = 0
```




<hr>



### function getOwner 

_Returns the_ [_**Plugin**_](classendstone_1_1Plugin.md) _that owns this task._
```C++
virtual Plugin * endstone::Task::getOwner () const = 0
```





**Returns:**

The [**Plugin**](classendstone_1_1Plugin.md) that owns the task 





        

<hr>



### function getTaskId 

_Returns the taskId for the task._ 
```C++
virtual TaskId endstone::Task::getTaskId () const = 0
```





**Returns:**

[**Task**](classendstone_1_1Task.md) id number 





        

<hr>



### function isCancelled 

_Returns true if this task has been cancelled._ 
```C++
virtual bool endstone::Task::isCancelled () const = 0
```





**Returns:**

true if the task has been cancelled 





        

<hr>



### function isSync 

_Returns true if the_ [_**Task**_](classendstone_1_1Task.md) _is a sync task._
```C++
virtual bool endstone::Task::isSync () const = 0
```





**Returns:**

true if the task is run by server thread 





        

<hr>



### function ~Task 

```C++
virtual endstone::Task::~Task () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/scheduler/task.h`

