

# Class endstone::Scheduler



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Scheduler**](classendstone_1_1Scheduler.md)



_Represents a scheduler that executes various tasks._ 

* `#include <endstone/scheduler/scheduler.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**cancelTask**](#function-canceltask) (TaskId id) = 0<br> |
| virtual void | [**cancelTasks**](#function-canceltasks) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br> |
| virtual std::vector&lt; [**Task**](classendstone_1_1Task.md) \* &gt; | [**getPendingTasks**](#function-getpendingtasks) () = 0<br> |
| virtual bool | [**isQueued**](#function-isqueued) (TaskId id) = 0<br> |
| virtual bool | [**isRunning**](#function-isrunning) (TaskId id) = 0<br> |
| virtual std::shared\_ptr&lt; [**Task**](classendstone_1_1Task.md) &gt; | [**runTask**](#function-runtask) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, std::function&lt; void()&gt; task) = 0<br> |
| virtual std::shared\_ptr&lt; [**Task**](classendstone_1_1Task.md) &gt; | [**runTaskAsync**](#function-runtaskasync) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, std::function&lt; void()&gt; task) = 0<br>_Returns a task that will be executed asynchronously on the next server tick._  |
| virtual std::shared\_ptr&lt; [**Task**](classendstone_1_1Task.md) &gt; | [**runTaskLater**](#function-runtasklater) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, std::function&lt; void()&gt; task, std::uint64\_t delay) = 0<br> |
| virtual std::shared\_ptr&lt; [**Task**](classendstone_1_1Task.md) &gt; | [**runTaskLaterAsync**](#function-runtasklaterasync) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, std::function&lt; void()&gt; task, std::uint64\_t delay) = 0<br>_Returns a task that will be executed asynchronously after the specified number of server ticks._  |
| virtual std::shared\_ptr&lt; [**Task**](classendstone_1_1Task.md) &gt; | [**runTaskTimer**](#function-runtasktimer) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, std::function&lt; void()&gt; task, std::uint64\_t delay, std::uint64\_t period) = 0<br> |
| virtual std::shared\_ptr&lt; [**Task**](classendstone_1_1Task.md) &gt; | [**runTaskTimerAsync**](#function-runtasktimerasync) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, std::function&lt; void()&gt; task, std::uint64\_t delay, std::uint64\_t period) = 0<br>_Returns a task that will be executed repeatedly (and asynchronously) until cancelled, starting after the specified number of server ticks._  |
| virtual  | [**~Scheduler**](#function-scheduler) () = default<br> |




























## Public Functions Documentation




### function cancelTask 


```C++
virtual void endstone::Scheduler::cancelTask (
    TaskId id
) = 0
```



Removes task from scheduler.




**Parameters:**


* `taskId` Id number of task to be removed 




        

<hr>



### function cancelTasks 


```C++
virtual void endstone::Scheduler::cancelTasks (
    Plugin & plugin
) = 0
```



Removes all tasks associated with a particular plugin from the scheduler.




**Parameters:**


* `plugin` Owner of tasks to be removed 




        

<hr>



### function getPendingTasks 


```C++
virtual std::vector< Task * > endstone::Scheduler::getPendingTasks () = 0
```



Returns a vector of all pending tasks.


The ordering of the tasks is NOT related to their order of execution.




**Returns:**

Pending tasks 





        

<hr>



### function isQueued 


```C++
virtual bool endstone::Scheduler::isQueued (
    TaskId id
) = 0
```



Check if the task queued to be run later.




**Parameters:**


* `taskId` The task to check.



**Returns:**

If the task is queued to be run. 





        

<hr>



### function isRunning 


```C++
virtual bool endstone::Scheduler::isRunning (
    TaskId id
) = 0
```



Check if the task currently running.




**Parameters:**


* `taskId` The task to check.



**Returns:**

If the task is currently running. 





        

<hr>



### function runTask 


```C++
virtual std::shared_ptr< Task > endstone::Scheduler::runTask (
    Plugin & plugin,
    std::function< void()> task
) = 0
```



Returns a task that will be executed synchronously on the next server tick.




**Parameters:**


* `plugin` the reference to the plugin scheduling task 
* `task` the task to be run 



**Returns:**

a [**Task**](classendstone_1_1Task.md) that contains the id number (nullptr if task is empty) 





        

<hr>



### function runTaskAsync 

_Returns a task that will be executed asynchronously on the next server tick._ 
```C++
virtual std::shared_ptr< Task > endstone::Scheduler::runTaskAsync (
    Plugin & plugin,
    std::function< void()> task
) = 0
```





**Remark:**

Asynchronous tasks should never access any Endstone API




**Parameters:**


* `plugin` the reference to the plugin scheduling task 
* `task` the task to be run 



**Returns:**

a [**Task**](classendstone_1_1Task.md) that contains the id number (nullptr if task is empty) 





        

<hr>



### function runTaskLater 


```C++
virtual std::shared_ptr< Task > endstone::Scheduler::runTaskLater (
    Plugin & plugin,
    std::function< void()> task,
    std::uint64_t delay
) = 0
```



Returns a task that will be executed synchronously after the specified number of server ticks.




**Parameters:**


* `plugin` the reference to the plugin scheduling task 
* `task` the task to be run 
* `delay` the ticks to wait before running the task 



**Returns:**

a [**Task**](classendstone_1_1Task.md) that contains the id number (nullptr if task is empty) 





        

<hr>



### function runTaskLaterAsync 

_Returns a task that will be executed asynchronously after the specified number of server ticks._ 
```C++
virtual std::shared_ptr< Task > endstone::Scheduler::runTaskLaterAsync (
    Plugin & plugin,
    std::function< void()> task,
    std::uint64_t delay
) = 0
```





**Remark:**

Asynchronous tasks should never access any Endstone API




**Parameters:**


* `plugin` the reference to the plugin scheduling task 
* `task` the task to be run 
* `delay` the ticks to wait before running the task 



**Returns:**

a [**Task**](classendstone_1_1Task.md) that contains the id number (nullptr if task is empty) 





        

<hr>



### function runTaskTimer 


```C++
virtual std::shared_ptr< Task > endstone::Scheduler::runTaskTimer (
    Plugin & plugin,
    std::function< void()> task,
    std::uint64_t delay,
    std::uint64_t period
) = 0
```



Returns a task that will be executed repeatedly (and synchronously) until cancelled, starting after the specified number of server ticks.




**Parameters:**


* `plugin` the reference to the plugin scheduling task 
* `task` the task to be run 
* `delay` the ticks to wait before running the task 
* `period` the ticks to wait between runs 



**Returns:**

a [**Task**](classendstone_1_1Task.md) that contains the id number (nullptr if task is empty) 





        

<hr>



### function runTaskTimerAsync 

_Returns a task that will be executed repeatedly (and asynchronously) until cancelled, starting after the specified number of server ticks._ 
```C++
virtual std::shared_ptr< Task > endstone::Scheduler::runTaskTimerAsync (
    Plugin & plugin,
    std::function< void()> task,
    std::uint64_t delay,
    std::uint64_t period
) = 0
```





**Remark:**

Asynchronous tasks should never access any Endstone API




**Parameters:**


* `plugin` the reference to the plugin scheduling task 
* `task` the task to be run 
* `delay` the ticks to wait before running the task 
* `period` the ticks to wait between runs 



**Returns:**

a [**Task**](classendstone_1_1Task.md) that contains the id number (nullptr if task is empty) 





        

<hr>



### function ~Scheduler 

```C++
virtual endstone::Scheduler::~Scheduler () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/scheduler/scheduler.h`

