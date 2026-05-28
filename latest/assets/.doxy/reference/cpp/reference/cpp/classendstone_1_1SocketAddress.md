

# Class endstone::SocketAddress



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**SocketAddress**](classendstone_1_1SocketAddress.md)



_Represents an IP Socket Address (hostname + port number)._ 

* `#include <endstone/util/socket_address.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SocketAddress**](#function-socketaddress-12) () = default<br> |
|   | [**SocketAddress**](#function-socketaddress-22) (std::string hostname, std::uint32\_t port) <br> |
|  [**const**](classendstone_1_1Identifier.md) std::string & | [**getHostname**](#function-gethostname) () const<br>_Gets the hostname._  |
|  std::uint32\_t | [**getPort**](#function-getport) () const<br>_Gets the port number._  |




























## Public Functions Documentation




### function SocketAddress [1/2]

```C++
endstone::SocketAddress::SocketAddress () = default
```




<hr>



### function SocketAddress [2/2]

```C++
inline endstone::SocketAddress::SocketAddress (
    std::string hostname,
    std::uint32_t port
) 
```




<hr>



### function getHostname 

_Gets the hostname._ 
```C++
inline const std::string & endstone::SocketAddress::getHostname () const
```





**Returns:**

The hostname of this socket address 





        

<hr>



### function getPort 

_Gets the port number._ 
```C++
inline std::uint32_t endstone::SocketAddress::getPort () const
```





**Returns:**

The port number of this socket address 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/socket_address.h`

