

# Class endstone::Item



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Item**](classendstone_1_1Item.md)



_Represents a base actor in the level._ 

* `#include <endstone/actor/item.h>`



Inherits the following classes: [endstone::Actor](classendstone_1_1Actor.md)






























































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**ItemStack**](classendstone_1_1ItemStack.md) | [**getItemStack**](#function-getitemstack) () const = 0<br>_Gets the item stack associated with this item drop._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getPickupDelay**](#function-getpickupdelay) () const = 0<br>_Gets the delay before this_ [_**Item**_](classendstone_1_1Item.md) _is available to be picked up by players._ |
| virtual std::optional&lt; std::int64\_t &gt; | [**getThrower**](#function-getthrower) () const = 0<br>_Get the thrower of this item._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isUnlimitedLifetime**](#function-isunlimitedlifetime) () const = 0<br>_Gets if this_ [_**Item**_](classendstone_1_1Item.md) _lives forever._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setItemStack**](#function-setitemstack) ([**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & stack) = 0<br>_Sets the item stack associated with this item drop._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setPickupDelay**](#function-setpickupdelay) ([**int**](classendstone_1_1Identifier.md) delay) = 0<br>_Sets the delay before this_ [_**Item**_](classendstone_1_1Item.md) _is available to be picked up by players._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setThrower**](#function-setthrower) (std::optional&lt; std::int64\_t &gt; thrower) = 0<br>_Set the thrower of this item._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setUnlimitedLifetime**](#function-setunlimitedlifetime) ([**bool**](classendstone_1_1Identifier.md) unlimited) = 0<br>_Sets if this_ [_**Item**_](classendstone_1_1Item.md) _should live forever._ |


## Public Functions inherited from endstone::Actor

See [endstone::Actor](classendstone_1_1Actor.md)

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**addScoreboardTag**](classendstone_1_1Actor.md#function-addscoreboardtag) (std::string tag) const = 0<br>_Adds a tag to this actor._  |
| virtual [**Item**](classendstone_1_1Item.md) \* | [**asItem**](classendstone_1_1Actor.md#function-asitem) () const = 0<br>_Gets an_ [_**Actor**_](classendstone_1_1Actor.md) _as_[_**Item**_](classendstone_1_1Item.md) _._ |
| virtual [**Mob**](classendstone_1_1Mob.md) \* | [**asMob**](classendstone_1_1Actor.md#function-asmob) () const = 0<br>_Gets an_ [_**Actor**_](classendstone_1_1Actor.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._ |
| virtual [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](classendstone_1_1Actor.md#function-getdimension) () const = 0<br> |
| virtual std::int64\_t | [**getId**](classendstone_1_1Actor.md#function-getid) () const = 0<br>_Returns a unique id for this actor._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](classendstone_1_1Actor.md#function-getlevel) () const = 0<br> |
| virtual [**Location**](classendstone_1_1Location.md) | [**getLocation**](classendstone_1_1Actor.md#function-getlocation) () const = 0<br> |
| virtual std::string | [**getNameTag**](classendstone_1_1Actor.md#function-getnametag) () const = 0<br>_Gets the current name tag of the actor._  |
| virtual std::uint64\_t | [**getRuntimeId**](classendstone_1_1Actor.md#function-getruntimeid) () const = 0<br> |
| virtual std::string | [**getScoreTag**](classendstone_1_1Actor.md#function-getscoretag) () const = 0<br>_Gets the current score tag of the actor._  |
| virtual std::vector&lt; std::string &gt; | [**getScoreboardTags**](classendstone_1_1Actor.md#function-getscoreboardtags) () const = 0<br>_Returns a list of scoreboard tags for this actor._  |
| virtual std::string | [**getType**](classendstone_1_1Actor.md#function-gettype) () const = 0<br>_Get the type of the actor._  |
| virtual [**Vector**](classendstone_1_1Vector.md) | [**getVelocity**](classendstone_1_1Actor.md#function-getvelocity) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isDead**](classendstone_1_1Actor.md#function-isdead) () const = 0<br>_Returns true if this actor has been marked for removal._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInLava**](classendstone_1_1Actor.md#function-isinlava) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isInWater**](classendstone_1_1Actor.md#function-isinwater) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isNameTagAlwaysVisible**](classendstone_1_1Actor.md#function-isnametagalwaysvisible) () const = 0<br>_Checks if the actor's name tag is always visible._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isNameTagVisible**](classendstone_1_1Actor.md#function-isnametagvisible) () const = 0<br>_Checks if the actor's name tag is currently visible._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isOnGround**](classendstone_1_1Actor.md#function-isonground) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isValid**](classendstone_1_1Actor.md#function-isvalid) () const = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**remove**](classendstone_1_1Actor.md#function-remove) () = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**removeScoreboardTag**](classendstone_1_1Actor.md#function-removescoreboardtag) (std::string tag) const = 0<br>_Removes a given tag from this actor._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setNameTag**](classendstone_1_1Actor.md#function-setnametag) (std::string name) = 0<br>_Sets the name tag for the actor._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setNameTagAlwaysVisible**](classendstone_1_1Actor.md#function-setnametagalwaysvisible) ([**bool**](classendstone_1_1Identifier.md) visible) = 0<br>_Sets whether the actor's name tag should always be visible._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setNameTagVisible**](classendstone_1_1Actor.md#function-setnametagvisible) ([**bool**](classendstone_1_1Identifier.md) visible) = 0<br>_Sets if the actor's name tag is visible or not._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setRotation**](classendstone_1_1Actor.md#function-setrotation) ([**float**](classendstone_1_1Identifier.md) yaw, [**float**](classendstone_1_1Identifier.md) pitch) = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setScoreTag**](classendstone_1_1Actor.md#function-setscoretag) (std::string score) = 0<br>_Sets the score tag for the actor._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**teleport**](classendstone_1_1Actor.md#function-teleport-12) ([**const**](classendstone_1_1Identifier.md) [**Location**](classendstone_1_1Location.md) & location) = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**teleport**](classendstone_1_1Actor.md#function-teleport-22) ([**const**](classendstone_1_1Identifier.md) [**Actor**](classendstone_1_1Actor.md) & target) = 0<br> |


## Public Functions inherited from endstone::CommandSender

See [endstone::CommandSender](classendstone_1_1CommandSender.md)

| Type | Name |
| ---: | :--- |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](classendstone_1_1CommandSender.md#function-asactor) () const = 0<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**BlockCommandSender**](classendstone_1_1BlockCommandSender.md) \* | [**asBlock**](classendstone_1_1CommandSender.md#function-asblock) () const = 0<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**BlockCommandSender**_](classendstone_1_1BlockCommandSender.md) _._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1CommandSender.md#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](classendstone_1_1CommandSender.md#function-asconsole) () const = 0<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**ConsoleCommandSender**_](classendstone_1_1ConsoleCommandSender.md) _._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](classendstone_1_1CommandSender.md#function-asplayer) () const = 0<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
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
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1Permissible.md#function-ascommandsender) () const = 0<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](classendstone_1_1Permissible.md#function-geteffectivepermissions) () const = 0<br>_Gets a set containing all the permissions currently in effect by this object._  |
| virtual PermissionLevel | [**getPermissionLevel**](classendstone_1_1Permissible.md#function-getpermissionlevel) () const = 0<br>_Gets the permission level of this object._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-12) (std::string name) const = 0<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-22) ([**const**](classendstone_1_1Identifier.md) [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br>_Gets the value of the specified permission, if set. If a permission override is not set on this object, the default value of the permission will be returned._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-12) (std::string name) const = 0<br>_Checks if this object contains an override for the specified permission, by fully qualified name._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-22) ([**const**](classendstone_1_1Identifier.md) [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br>_Checks if this object contains an override for the specified_ [_**Permission**_](classendstone_1_1Permission.md) _._ |
| virtual [**void**](classendstone_1_1Identifier.md) | [**recalculatePermissions**](classendstone_1_1Permissible.md#function-recalculatepermissions) () = 0<br>_Recalculates the permissions for this object, if the attachments have changed values. This should very rarely need to be called from a plugin._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**removeAttachment**](classendstone_1_1Permissible.md#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br>_Removes the given_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _from this object._ |
| virtual  | [**~Permissible**](classendstone_1_1Permissible.md#function-permissible) () = default<br> |










































































































## Public Functions Documentation




### function getItemStack 

_Gets the item stack associated with this item drop._ 
```C++
virtual ItemStack endstone::Item::getItemStack () const = 0
```





**Returns:**

An item stack. 





        

<hr>



### function getPickupDelay 

_Gets the delay before this_ [_**Item**_](classendstone_1_1Item.md) _is available to be picked up by players._
```C++
virtual int endstone::Item::getPickupDelay () const = 0
```





**Returns:**

Remaining delay 





        

<hr>



### function getThrower 

_Get the thrower of this item._ 
```C++
virtual std::optional< std::int64_t > endstone::Item::getThrower () const = 0
```





**Note:**

The thrower is the entity which dropped the item.




**Returns:**

unique id of thrower 





        

<hr>



### function isUnlimitedLifetime 

_Gets if this_ [_**Item**_](classendstone_1_1Item.md) _lives forever._
```C++
virtual bool endstone::Item::isUnlimitedLifetime () const = 0
```





**Returns:**

true if the lifetime is unlimited 





        

<hr>



### function setItemStack 

_Sets the item stack associated with this item drop._ 
```C++
virtual void endstone::Item::setItemStack (
    const  ItemStack & stack
) = 0
```





**Parameters:**


* `stack` An item stack. 




        

<hr>



### function setPickupDelay 

_Sets the delay before this_ [_**Item**_](classendstone_1_1Item.md) _is available to be picked up by players._
```C++
virtual void endstone::Item::setPickupDelay (
    int delay
) = 0
```





**Parameters:**


* `delay` New delay 




        

<hr>



### function setThrower 

_Set the thrower of this item._ 
```C++
virtual void endstone::Item::setThrower (
    std::optional< std::int64_t > thrower
) = 0
```





**Note:**

The thrower is the entity which dropped the item.




**Parameters:**


* `thrower` unique id of thrower 




        

<hr>



### function setUnlimitedLifetime 

_Sets if this_ [_**Item**_](classendstone_1_1Item.md) _should live forever._
```C++
virtual void endstone::Item::setUnlimitedLifetime (
    bool unlimited
) = 0
```





**Parameters:**


* `unlimited` true if the lifetime is unlimited 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/actor/item.h`

