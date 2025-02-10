
# Class Hierarchy

This inheritance list is sorted roughly, but not completely, alphabetically:


* **class** [**endstone::Form**](classendstone_1_1Form.md) _Represents a generic form._ 
* **class** [**endstone::ActionForm::Button**](classendstone_1_1ActionForm_1_1Button.md) _Represents a button with text and an optional icon._ 
* **class** [**endstone::Permissible**](classendstone_1_1Permissible.md) _Represents an object that may become a server operator and can be assigned permissions._     
    * **class** [**endstone::CommandSender**](classendstone_1_1CommandSender.md) _Represents a command sender._     
        * **class** [**endstone::Actor**](classendstone_1_1Actor.md) _Represents a base actor in the level._     
            * **class** [**endstone::Mob**](classendstone_1_1Mob.md) _Represents a mobile entity (i.e. living entity), such as a monster or player._     
                * **class** [**endstone::Player**](classendstone_1_1Player.md) _Represents a player._ 
        * **class** [**endstone::CommandSenderWrapper**](classendstone_1_1CommandSenderWrapper.md) _Represents a wrapper that forwards commands to the wrapped_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _and captures its output._
        * **class** [**endstone::ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) _Represents a console command sender._ 
        * **class** [**endstone::ProxiedCommandSender**](classendstone_1_1ProxiedCommandSender.md) _Represents a proxied command sender._ 
* **class** [**endstone::ICancellable**](classendstone_1_1ICancellable.md)     
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
* **class** [**endstone::Event**](classendstone_1_1Event.md) _Represents an event._     
    * **class** [**endstone::ActorEvent**](classendstone_1_1ActorEvent.md) _Represents an Actor-related event._ 
    * **class** [**endstone::ActorEvent**](classendstone_1_1ActorEvent.md) _Represents an Actor-related event._ 
    * **class** [**endstone::ActorEvent**](classendstone_1_1ActorEvent.md) _Represents an Actor-related event._ 
    * **class** [**endstone::BlockEvent**](classendstone_1_1BlockEvent.md) _Represents an Block-related event._     
        * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
        * **class** [**endstone::BlockPistonEvent**](classendstone_1_1BlockPistonEvent.md) _Called when a piston block is triggered._ 
    * **class** [**endstone::PlayerEvent**](classendstone_1_1PlayerEvent.md) _Represents a player related event._     
        * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
        * **class** [**endstone::PlayerDeathEvent**](classendstone_1_1PlayerDeathEvent.md) _Called when a_ [_**Player**_](classendstone_1_1Player.md) _dies._
        * **class** [**endstone::PlayerEmoteEvent**](classendstone_1_1PlayerEmoteEvent.md) _Called when a player uses an emote._ 
        * **class** [**endstone::PlayerJoinEvent**](classendstone_1_1PlayerJoinEvent.md) _Called when a player joins a server._ 
        * **class** [**endstone::PlayerQuitEvent**](classendstone_1_1PlayerQuitEvent.md) _Called when a player leaves a server._ 
        * **class** [**endstone::PlayerRespawnEvent**](classendstone_1_1PlayerRespawnEvent.md) _Called when a player respawns._ 
    * **class** [**endstone::ServerEvent**](classendstone_1_1ServerEvent.md) _Represents a Server-related event._     
        * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
        * **class** [**endstone::PluginDisableEvent**](classendstone_1_1PluginDisableEvent.md) _Called when a plugin is disabled._ 
        * **class** [**endstone::PluginEnableEvent**](classendstone_1_1PluginEnableEvent.md) _Called when a plugin is enabled._ 
        * **class** [**endstone::ServerLoadEvent**](classendstone_1_1ServerLoadEvent.md) _Called when either the server startup or reload has completed._ 
    * **class** [**endstone::WeatherEvent**](classendstone_1_1WeatherEvent.md) _Represents a Weather-related event._     
        * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
* **class** [**endstone::BanEntry**](classendstone_1_1BanEntry.md) _A single entry from a ban list._     
    * **class** [**endstone::IpBanEntry**](classendstone_1_1IpBanEntry.md) 
    * **class** [**endstone::PlayerBanEntry**](classendstone_1_1PlayerBanEntry.md) 
* **class** [**endstone::BanList**](classendstone_1_1BanList.md) _Represents a ban list, containing bans._ 
* **class** [**endstone::BlockState**](classendstone_1_1BlockState.md) _Represents a captured state of a block, which will not update automatically._ 
* **class** [**endstone::BossBar**](classendstone_1_1BossBar.md) _Represents a boss bar that is displayed to players._ 
* **class** [**endstone::Chunk**](classendstone_1_1Chunk.md) _Represents a chunk of blocks._ 
* **class** [**endstone::Command**](classendstone_1_1Command.md) _Represents a_ [_**Command**_](classendstone_1_1Command.md) _, which executes various tasks upon user input._    
    * **class** [**endstone::PluginCommand**](classendstone_1_1PluginCommand.md) _Represents a_ [_**Command**_](classendstone_1_1Command.md) _belonging to a_[_**Plugin**_](classendstone_1_1Plugin.md) _._
* **class** [**endstone::CommandExecutor**](classendstone_1_1CommandExecutor.md) _Represents a class which contains a single method for executing commands._     
    * **class** [**endstone::Plugin**](classendstone_1_1Plugin.md) _Represents a_ [_**Plugin**_](classendstone_1_1Plugin.md) _._
* **class** [**endstone::CommandMap**](classendstone_1_1CommandMap.md) _Represents a command map that manages all commands of the_ [_**Server**_](classendstone_1_1Server.md) _._
* **class** [**endstone::Criteria**](classendstone_1_1Criteria.md) _Represents a scoreboard criteria._ 
* **class** [**endstone::DamageSource**](classendstone_1_1DamageSource.md) _Represents a source of damage._ 
* **class** [**endstone::Dimension**](classendstone_1_1Dimension.md) _Represents a dimension within a_ [_**Level**_](classendstone_1_1Level.md) _._
* **class** [**endstone::Dropdown**](classendstone_1_1Dropdown.md) _Represents a dropdown with a set of predefined options._     
    * **class** [**endstone::StepSlider**](classendstone_1_1StepSlider.md) _Represents a step slider with a set of predefined options._ 
* **class** [**endstone::Error**](classendstone_1_1Error.md) _Represents an error with a message and the stack trace._ 
* **class** [**endstone::EventHandler**](classendstone_1_1EventHandler.md) _Represents a registered_ [_**EventHandler**_](classendstone_1_1EventHandler.md) _which associates with a_[_**Plugin**_](classendstone_1_1Plugin.md) _._
* **class** [**endstone::HandlerList**](classendstone_1_1HandlerList.md) _A list of event handlers. Should be instantiated on a per-event basis._ 
* **class** [**endstone::Inventory**](classendstone_1_1Inventory.md) _Interface to the various inventories._     
    * **class** [**endstone::PlayerInventory**](classendstone_1_1PlayerInventory.md) _Interface to the inventory of a_ [_**Player**_](classendstone_1_1Player.md) _, including the four armor slots and any extra slots._
* **class** [**endstone::Label**](classendstone_1_1Label.md) _Represents a text label._ 
* **class** [**endstone::Language**](classendstone_1_1Language.md) _Represents the interface for translating text into different languages._ 
* **class** [**endstone::Level**](classendstone_1_1Level.md) _Represents a level, which may contain actors, chunks and blocks._ 
* **class** [**endstone::Vector**](classendstone_1_1Vector.md) _Represents a 3-dimensional vector._ 
* **class** [**endstone::Logger**](classendstone_1_1Logger.md) [_**Logger**_](classendstone_1_1Logger.md) _class which can format and output varies levels of logs._
* **class** [**endstone::Objective**](classendstone_1_1Objective.md) _Represents an objective on a scoreboard that can show scores specific to entries._ 
* **class** [**endstone::OfflinePlayer**](classendstone_1_1OfflinePlayer.md) _Represents a reference to a player identity and the data belonging to a player that is stored on the disk and can, thus, be retrieved without the player needing to be online._     
    * **class** [**endstone::Player**](classendstone_1_1Player.md) _Represents a player._ 
* **class** [**endstone::Packet**](classendstone_1_1Packet.md) _Represents a packet._     
    * **class** [**endstone::SpawnParticleEffectPacket**](classendstone_1_1SpawnParticleEffectPacket.md) _Represents a packet for spawning a particle effect._ 
* **class** [**endstone::Permission**](classendstone_1_1Permission.md) _Represents a unique permission that may be attached to a_ [_**Permissible**_](classendstone_1_1Permissible.md) _._
* **class** [**endstone::PermissionAttachment**](classendstone_1_1PermissionAttachment.md) _Holds information about a permission attachment on a_ [_**Permissible**_](classendstone_1_1Permissible.md) _object._
* **class** [**endstone::PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) _Holds information on a permission and which_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _provides it._
* **class** [**endstone::PluginDescription**](classendstone_1_1PluginDescription.md) _Represents the basic information about a plugin that the plugin loader needs to know._ 
* **class** [**endstone::PluginLoader**](classendstone_1_1PluginLoader.md) _Represents a plugin loader, which handles direct access to specific types of plugins._ 
* **class** [**endstone::PluginManager**](classendstone_1_1PluginManager.md) _Represents a plugin manager that handles all plugins from the_ [_**Server**_](classendstone_1_1Server.md) _._
* **class** [**endstone::Scheduler**](classendstone_1_1Scheduler.md) _Represents a scheduler that executes various tasks._ 
* **class** [**endstone::Score**](classendstone_1_1Score.md) _Represents a score for an objective on a scoreboard._ 
* **class** [**endstone::Server**](classendstone_1_1Server.md) _Represents a server implementation._ 
* **class** [**endstone::Skin**](classendstone_1_1Skin.md) _Represents a player skin._ 
* **class** [**endstone::Slider**](classendstone_1_1Slider.md) _Represents a slider with a label._ 
* **class** [**endstone::SocketAddress**](classendstone_1_1SocketAddress.md) _Represents an IP Socket Address (hostname + port number)._ 
* **class** [**endstone::Task**](classendstone_1_1Task.md) _Represents a task being executed by the scheduler._ 
* **class** [**endstone::TextInput**](classendstone_1_1TextInput.md) _Represents a text input field._ 
* **class** [**endstone::Toggle**](classendstone_1_1Toggle.md) _Represents a toggle button with a label._ 
* **class** [**endstone::Translatable**](classendstone_1_1Translatable.md) _Represents an object with a text representation that can be translated by the Minecraft client._ 
* **class** [**endstone::UUID**](classendstone_1_1UUID.md) _Implementation of Universally Unique Identifier (_ [_**UUID**_](classendstone_1_1UUID.md) _)_
* **class** [**endstone::detail::CommandBuilder**](classendstone_1_1detail_1_1CommandBuilder.md) 
* **class** [**endstone::detail::PermissionBuilder**](classendstone_1_1detail_1_1PermissionBuilder.md) 
* **struct** [**endstone::ColorFormat**](structendstone_1_1ColorFormat.md) _All supported color and format codes._ 
* **struct** [**endstone::Skin::ImageData**](structendstone_1_1Skin_1_1ImageData.md) 
* **struct** [**endstone::detail::PluginDescriptionBuilder**](structendstone_1_1detail_1_1PluginDescriptionBuilder.md) 
* **struct** [**std::hash&lt; endstone::UUID &gt;**](structstd_1_1hash_3_01endstone_1_1UUID_01_4.md) 
* **class** **EventType**    
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
    * **class** [**endstone::Cancellable**](classendstone_1_1Cancellable.md) _A type characterizing events that may be cancelled by a plugin or the server._ 
* **class** **std::enable_shared_from_this< Block >**    
    * **class** [**endstone::Block**](classendstone_1_1Block.md) _Represents a block._ 
* **class** **std::enable_shared_from_this< BlockData >**    
    * **class** [**endstone::BlockData**](classendstone_1_1BlockData.md) _Represents the data related to a live block._ 
* **class** **std::enable_shared_from_this< ItemStack >**    
    * **class** [**endstone::ItemStack**](classendstone_1_1ItemStack.md) _Represents a stack of items._ 
* **class** **std::enable_shared_from_this< Scoreboard >**    
    * **class** [**endstone::Scoreboard**](classendstone_1_1Scoreboard.md) _Represents a scoreboard._ 
* **class** **endstone::Func**    
    * **struct** [**endstone::overloaded**](structendstone_1_1overloaded.md) _Helper type for visitors._ 
* **class** **formatter< string_view >**    
    * **struct** [**fmt::formatter&lt; endstone::Actor &gt;**](structfmt_1_1formatter_3_01endstone_1_1Actor_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::Block &gt;**](structfmt_1_1formatter_3_01endstone_1_1Block_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::BlockData &gt;**](structfmt_1_1formatter_3_01endstone_1_1BlockData_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::BlockState &gt;**](structfmt_1_1formatter_3_01endstone_1_1BlockState_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::BlockStates &gt;**](structfmt_1_1formatter_3_01endstone_1_1BlockStates_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::BlockStates::mapped\_type &gt;**](structfmt_1_1formatter_3_01endstone_1_1BlockStates_1_1mapped__type_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::BlockStates::value\_type &gt;**](structfmt_1_1formatter_3_01endstone_1_1BlockStates_1_1value__type_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::DamageSource &gt;**](structfmt_1_1formatter_3_01endstone_1_1DamageSource_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::ItemStack &gt;**](structfmt_1_1formatter_3_01endstone_1_1ItemStack_01_4.md) 

