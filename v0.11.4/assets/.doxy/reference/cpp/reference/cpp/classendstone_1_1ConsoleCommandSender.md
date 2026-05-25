

# Class endstone::ConsoleCommandSender



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md)



_Represents a console command sender._ 

* `#include <endstone/command/console_command_sender.h>`



Inherits the following classes: [endstone::CommandSender](classendstone_1_1CommandSender.md)












































































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

















































































------------------------------
The documentation for this class was generated from the following file `include/endstone/command/console_command_sender.h`

