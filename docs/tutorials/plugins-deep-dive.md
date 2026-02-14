---
comments: true
---

# Plugins Deep Dive

How does Endstone load and run your plugins?

## Registration

At a high level, the Endstone server first creates the plugin manager. The manager then registers the C++ and Python plugin loaders.

```mermaid
flowchart TD
	A[Server] -->|Creates plugin manager| B[Plugin Manager]
	subgraph Plugin System
	B ~~~ C[Python Plugin Loader] --Register loader--> B
	B ~~~ D[C++ Plugin Loader] --Register loader--> B
	end
```

Each plugin loader defines what files they parse, and how to parse them. This information is passed during registration.

```mermaid
flowchart BT
	subgraph g1 [C++ Plugin Loader]
	A[Parser]
	B[File Filter]
	end	
	subgraph g2 [Python Plugin Loader]
	C[Parser]
	D[File Filter]
	end
	
	E[Plugin Manager]
	g1 --Register responsibility for C++ plugins--> E
	g2 --Register responsibility for Python plugins--> E	
	
```

## Loading

The server now looks for a `plugins` directory to load plugins from.

```mermaid
flowchart LR
	A[Server executable file]
	subgraph plugins
	B["Plugin A (C++)"]
	C["Plugin B (Python)"]
	end
	
	A --Checks if directory exists--> plugins
	
```

When the server finds plugins in that directory, it calls the manager to load them.

```mermaid
flowchart LR
	A[Server executable file]
	subgraph g1 [plugins]
	B["Plugin A (C++)"]
	C["Plugin B (Python)"]
	end
	
	B --Plugin path--> A
	C --Plugin path--> A

	subgraph g2 [Plugin System]
	PPL[Python Plugin Loader]
	CPL[C++ Plugin Loader]
	MNG[Plugin Manager]
	end
	
	A --Load plugin A--> MNG
	A --Load plugin B--> MNG
	MNG --Load Plugin B (Python)-->PPL
	MNG --Load Plugin A (C++)-->CPL
	
```

!!! note 

    This is not what's exactly done in the source code. The logic for scanning a directory belongs to the plugin manager.

## Runtime

Once the server is running, in-game events are passed to the loaded plugins for handling and optional cancellation, if the event is cancellable.

```mermaid
sequenceDiagram
	participant GR as Game Runtime
	participant PM as Plugin Manager
	participant PL as Plugin
	
	PL->>PM: Register event listeners
	
	
	GR->> GR: Running
	
	
	GR->>PM: callEvent(Event)
	activate GR
	
	PM->>PL: Provide event to registered listeners
	
	PL->>PL: Handle event
	
	PL-->>PM: Optionally return cancellation information
	
	PM-->>GR: Process cancellable event
	
	deactivate GR

	GR->> GR: Running
```
