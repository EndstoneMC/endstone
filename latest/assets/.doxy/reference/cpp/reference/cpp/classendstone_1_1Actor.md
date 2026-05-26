

# Class endstone::Actor



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Actor**](classendstone_1_1Actor.md)



_Represents a base actor in the level._ 

* `#include <endstone/actor/actor.h>`



Inherits the following classes: [endstone::CommandSender](classendstone_1_1CommandSender.md)


Inherited by the following classes: [endstone::Item](classendstone_1_1Item.md),  [endstone::Mob](classendstone_1_1Mob.md)




























































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**addScoreboardTag**](#function-addscoreboardtag) (std::string tag) const = 0<br>_Adds a tag to this actor._  |
| virtual [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](#function-getdimension) () const = 0<br> |
| virtual std::int64\_t | [**getId**](#function-getid) () const = 0<br>_Returns a unique id for this actor._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](#function-getlevel) () const = 0<br> |
| virtual [**Location**](classendstone_1_1Location.md) | [**getLocation**](#function-getlocation) () const = 0<br> |
| virtual std::string | [**getNameTag**](#function-getnametag) () const = 0<br>_Gets the current name tag of the actor._  |
| virtual std::uint64\_t | [**getRuntimeId**](#function-getruntimeid) () const = 0<br> |
| virtual std::string | [**getScoreTag**](#function-getscoretag) () const = 0<br>_Gets the current score tag of the actor._  |
| virtual std::vector&lt; std::string &gt; | [**getScoreboardTags**](#function-getscoreboardtags) () const = 0<br>_Returns a list of scoreboard tags for this actor._  |
| virtual [**const**](classendstone_1_1Identifier.md) [**ActorType**](classendstone_1_1ActorType.md) & | [**getType**](#function-gettype) () const = 0<br>_Get the type of the actor._  |
| virtual [**Vector**](classendstone_1_1Vector.md) | [**getVelocity**](#function-getvelocity) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isDead**](#function-isdead) () const = 0<br>_Returns true if this actor has been marked for removal._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInLava**](#function-isinlava) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInWater**](#function-isinwater) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isNameTagAlwaysVisible**](#function-isnametagalwaysvisible) () const = 0<br>_Checks if the actor's name tag is always visible._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isNameTagVisible**](#function-isnametagvisible) () const = 0<br>_Checks if the actor's name tag is currently visible._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isOnGround**](#function-isonground) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isValid**](#function-isvalid) () const = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**remove**](#function-remove) () = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**removeScoreboardTag**](#function-removescoreboardtag) (std::string tag) const = 0<br>_Removes a given tag from this actor._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setNameTag**](#function-setnametag) (std::string name) = 0<br>_Sets the name tag for the actor._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setNameTagAlwaysVisible**](#function-setnametagalwaysvisible) ([**bool**](classendstone_1_1Identifier.md) visible) = 0<br>_Sets whether the actor's name tag should always be visible._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setNameTagVisible**](#function-setnametagvisible) ([**bool**](classendstone_1_1Identifier.md) visible) = 0<br>_Sets if the actor's name tag is visible or not._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setRotation**](#function-setrotation) ([**float**](classendstone_1_1Identifier.md) yaw, [**float**](classendstone_1_1Identifier.md) pitch) = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setScoreTag**](#function-setscoretag) (std::string score) = 0<br>_Sets the score tag for the actor._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**teleport**](#function-teleport-12) ([**const**](classendstone_1_1Identifier.md) [**Location**](classendstone_1_1Location.md) & location) = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**teleport**](#function-teleport-22) ([**const**](classendstone_1_1Identifier.md) [**Actor**](classendstone_1_1Actor.md) & target) = 0<br> |


## Public Functions inherited from endstone::CommandSender

See [endstone::CommandSender](classendstone_1_1CommandSender.md)

| Type | Name |
| ---: | :--- |
| virtual std::string | [**getName**](classendstone_1_1CommandSender.md#function-getname) () const = 0<br>_Gets the name of this command sender._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](classendstone_1_1CommandSender.md#function-getserver) () const = 0<br>_Returns the server instance that this command is running on._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-12) ([**const**](classendstone_1_1Identifier.md) Message & message) const = 0<br>_Sends this sender a error message._  |
|  [**void**](classendstone_1_1Identifier.md) | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-22) ([**const**](classendstone_1_1Identifier.md) fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Identifier.md) &&... args) const<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-12) ([**const**](classendstone_1_1Identifier.md) Message & message) const = 0<br>_Sends this sender a message._  |
|  [**void**](classendstone_1_1Identifier.md) | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-22) ([**const**](classendstone_1_1Identifier.md) fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Identifier.md) &&... args) const<br> |


## Public Functions inherited from endstone::Permissible

See [endstone::Permissible](classendstone_1_1Permissible.md)

| Type | Name |
| ---: | :--- |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, [**const**](classendstone_1_1Identifier.md) std::string & name, [**bool**](classendstone_1_1Identifier.md) value) = 0<br>_Adds a new_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _with a single permission by name and value._ |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Adds a new empty_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _to this object._ |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](classendstone_1_1Permissible.md#function-geteffectivepermissions) () const = 0<br>_Gets a set containing all the permissions currently in effect by this object._  |
| virtual PermissionLevel | [**getPermissionLevel**](classendstone_1_1Permissible.md#function-getpermissionlevel) () const = 0<br>_Gets the permission level of this object._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-12) (std::string name) const = 0<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-22) ([**const**](classendstone_1_1Identifier.md) [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-12) (std::string name) const = 0<br>_Checks if this object contains an override for the specified permission, by fully qualified name._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-22) ([**const**](classendstone_1_1Identifier.md) [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br>_Checks if this object contains an override for the specified_ [_**Permission**_](classendstone_1_1Permission.md) _._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**recalculatePermissions**](classendstone_1_1Permissible.md#function-recalculatepermissions) () = 0<br>_Recalculates the permissions for this object, if the attachments have changed values. This should very rarely need to be called from a plugin._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**removeAttachment**](classendstone_1_1Permissible.md#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br>_Removes the given_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _from this object._ |


## Public Functions inherited from endstone::Object

See [endstone::Object](classendstone_1_1Object.md)

| Type | Name |
| ---: | :--- |
|  [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1Object.md#function-as-12) () <br>_Attempts to cast this object to the given type T._  |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**as**](classendstone_1_1Object.md#function-as-22) () const<br>_Attempts to cast this object to the given type T._  |
| virtual [**const**](classendstone_1_1Identifier.md) std::type\_info & | [**getClassTypeId**](classendstone_1_1Object.md#function-getclasstypeid) () const = 0<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**is**](classendstone_1_1Object.md#function-is) () const<br>_Checks if this object is an instance of the given type T (or a subclass of T)._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInstanceOf**](classendstone_1_1Object.md#function-isinstanceof) ([**const**](classendstone_1_1Identifier.md) std::type\_info & target) const = 0<br> |
| virtual  | [**~Object**](classendstone_1_1Object.md#function-object) () = default<br> |










































































































## Public Functions Documentation




### function addScoreboardTag 

_Adds a tag to this actor._ 
```C++
virtual bool endstone::Actor::addScoreboardTag (
    std::string tag
) const = 0
```





**Parameters:**


* `tag` the tag to add 



**Returns:**

true if the tag was successfully added, false if the tag already exists. 





        

<hr>



### function getDimension 

```C++
virtual Dimension & endstone::Actor::getDimension () const = 0
```



Gets the current [**Dimension**](classendstone_1_1Dimension.md) this actor resides in




**Returns:**

The current [**Dimension**](classendstone_1_1Dimension.md) this actor resides in 





        

<hr>



### function getId 

_Returns a unique id for this actor._ 
```C++
virtual std::int64_t endstone::Actor::getId () const = 0
```





**Returns:**

[**Actor**](classendstone_1_1Actor.md) id 





        

<hr>



### function getLevel 

```C++
virtual Level & endstone::Actor::getLevel () const = 0
```



Gets the current [**Level**](classendstone_1_1Level.md) this actor resides in




**Returns:**

The current [**Level**](classendstone_1_1Level.md) this actor resides in 





        

<hr>



### function getLocation 

```C++
virtual Location endstone::Actor::getLocation () const = 0
```



Gets the actor's current position




**Returns:**

a new copy of [**Location**](classendstone_1_1Location.md) containing the position of this actor 





        

<hr>



### function getNameTag 

_Gets the current name tag of the actor._ 
```C++
virtual std::string endstone::Actor::getNameTag () const = 0
```





**Returns:**

The name tag. 





        

<hr>



### function getRuntimeId 

```C++
virtual std::uint64_t endstone::Actor::getRuntimeId () const = 0
```



Returns the runtime id for this actor




**Returns:**

Runtime id for this actor 





        

<hr>



### function getScoreTag 

_Gets the current score tag of the actor._ 
```C++
virtual std::string endstone::Actor::getScoreTag () const = 0
```





**Returns:**

The score tag. 





        

<hr>



### function getScoreboardTags 

_Returns a list of scoreboard tags for this actor._ 
```C++
virtual std::vector< std::string > endstone::Actor::getScoreboardTags () const = 0
```





**Returns:**

a list of scoreboard tags for this actor 





        

<hr>



### function getType 

_Get the type of the actor._ 
```C++
virtual const  ActorType & endstone::Actor::getType () const = 0
```





**Returns:**

The type of the actor. 





        

<hr>



### function getVelocity 

```C++
virtual Vector endstone::Actor::getVelocity () const = 0
```



Gets this actor's current velocity




**Returns:**

Current traveling velocity of this actor 





        

<hr>



### function isDead 

_Returns true if this actor has been marked for removal._ 
```C++
virtual bool endstone::Actor::isDead () const = 0
```





**Returns:**

True if it is dead. 





        

<hr>



### function isInLava 

```C++
virtual bool endstone::Actor::isInLava () const = 0
```



Returns true if the actor is in lava.




**Returns:**

True if the actor is in lava. 





        

<hr>



### function isInWater 

```C++
virtual bool endstone::Actor::isInWater () const = 0
```



Returns true if the actor is in water.




**Returns:**

True if the actor is in water. 





        

<hr>



### function isNameTagAlwaysVisible 

_Checks if the actor's name tag is always visible._ 
```C++
virtual bool endstone::Actor::isNameTagAlwaysVisible () const = 0
```





**Returns:**

True if the name tag is always visible, false otherwise. 





        

<hr>



### function isNameTagVisible 

_Checks if the actor's name tag is currently visible._ 
```C++
virtual bool endstone::Actor::isNameTagVisible () const = 0
```





**Returns:**

True if the name tag is visible, false otherwise. 





        

<hr>



### function isOnGround 

```C++
virtual bool endstone::Actor::isOnGround () const = 0
```



Returns true if the actor is supported by a block.




**Returns:**

True if actor is on ground. 





        

<hr>



### function isValid 

```C++
virtual bool endstone::Actor::isValid () const = 0
```



Returns false if the entity has died, been despawned for some other reason, or has not been added to the level.




**Returns:**

True if valid. 





        

<hr>



### function remove 

```C++
virtual void endstone::Actor::remove () = 0
```



Remove this actor from the level.


If you are trying to remove a [**Player**](classendstone_1_1Player.md), use [**Player::kick**](classendstone_1_1Player.md#function-kick) instead. 


        

<hr>



### function removeScoreboardTag 

_Removes a given tag from this actor._ 
```C++
virtual bool endstone::Actor::removeScoreboardTag (
    std::string tag
) const = 0
```





**Parameters:**


* `tag` the tag to remove 



**Returns:**

true if the tag was successfully removed, false if the tag does not exist. 





        

<hr>



### function setNameTag 

_Sets the name tag for the actor._ 
```C++
virtual void endstone::Actor::setNameTag (
    std::string name
) = 0
```





**Parameters:**


* `name` The new name tag to set. 




        

<hr>



### function setNameTagAlwaysVisible 

_Sets whether the actor's name tag should always be visible._ 
```C++
virtual void endstone::Actor::setNameTagAlwaysVisible (
    bool visible
) = 0
```





**Parameters:**


* `visible` True to make the name tag always visible, false to disable always visibility. 




        

<hr>



### function setNameTagVisible 

_Sets if the actor's name tag is visible or not._ 
```C++
virtual void endstone::Actor::setNameTagVisible (
    bool visible
) = 0
```





**Parameters:**


* `visible` True to make the name tag visible, false to hide it. 




        

<hr>



### function setRotation 

```C++
virtual void endstone::Actor::setRotation (
    float yaw,
    float pitch
) = 0
```





**Returns:**

Sets the actor's rotation. 


Note that if the actor is affected by AI, it may override this rotation.




**Parameters:**


* `yaw` Rotation around the up axis (Y axis) 
* `pitch` Rotation around the right axis (X axis) 




        

<hr>



### function setScoreTag 

_Sets the score tag for the actor._ 
```C++
virtual void endstone::Actor::setScoreTag (
    std::string score
) = 0
```





**Parameters:**


* `score` The new score tag to set. 




        

<hr>



### function teleport [1/2]

```C++
virtual bool endstone::Actor::teleport (
    const  Location & location
) = 0
```





**Returns:**

Teleports this actor to the given location.




**Parameters:**


* `location` New location to teleport this actor to 




        

<hr>



### function teleport [2/2]

```C++
virtual bool endstone::Actor::teleport (
    const  Actor & target
) = 0
```



Teleports this actor to the target [**Actor**](classendstone_1_1Actor.md).




**Parameters:**


* `target` [**Actor**](classendstone_1_1Actor.md) to teleport this actor to 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/actor/actor.h`

