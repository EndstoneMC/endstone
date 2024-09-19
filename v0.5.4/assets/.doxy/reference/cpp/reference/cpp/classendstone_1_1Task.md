

# Class endstone::Task



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Task**](classendstone_1_1Task.md)



_Represents a task being executed by the scheduler._ 

* `#include <endstone/scheduler/task.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](#function-cancel) () = 0<br> |
| virtual [**Plugin**](classendstone_1_1Plugin.md) \* | [**getOwner**](#function-getowner) () const = 0<br> |
| virtual TaskId | [**getTaskId**](#function-gettaskid) () const = 0<br> |
| virtual bool | [**isCancelled**](#function-iscancelled) () const = 0<br> |
| virtual bool | [**isSync**](#function-issync) () const = 0<br> |
| virtual  | [**~Task**](#function-task) () = default<br> |




























## Public Functions Documentation




### function cancel 


```C++
virtual void endstone::Task::cancel () = 0
```



Attempts to cancel this task. 


        

<hr>



### function getOwner 


```C++
virtual Plugin * endstone::Task::getOwner () const = 0
```



Returns the [**Plugin**](classendstone_1_1Plugin.md) that owns this task.




**Returns:**

The [**Plugin**](classendstone_1_1Plugin.md) that owns the task 





        

<hr>



### function getTaskId 


```C++
virtual TaskId endstone::Task::getTaskId () const = 0
```



Returns the taskId for the task.




**Returns:**

[**Task**](classendstone_1_1Task.md) id number 





        

<hr>



### function isCancelled 


```C++
virtual bool endstone::Task::isCancelled () const = 0
```



Returns true if this task has been cancelled.




**Returns:**

true if the task has been cancelled 





        

<hr>



### function isSync 


```C++
virtual bool endstone::Task::isSync () const = 0
```



Returns true if the [**Task**](classendstone_1_1Task.md) is a sync task.




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

