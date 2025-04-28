# Benternet-services-project



## 🚀 About the project

<img src="./resources/cover.png" width="25%" height="auto" /><br>
The **Benternet Services Project** is a terminal- and session-based service running on the Benternet network. The backend is primarily developed in **C++**, with planned expansions using **JavaScript** to provide an HTML-based graphical user interface.

At its core, the project relies on a custom-built framework, utilizing the **zmqpp** library to interface with the Benternet network. The framework design is inspired by the **GStreamer pipeline** model, empowering users to construct their own modular and flexible service pipelines.

The modular pipeline serves as the foundation for the **DND-SERVICE**.
In the DND-SERVICE, players embark on a **Dungeons & Dragons**-inspired campaign, guided by an AI Dungeon Master.
The AI Dungeon Master dynamically adapts the world and scenarios to the players' actions, ensuring a unique and immersive experience each time.
Whether venturing alone or teaming up with friends, users can explore a world full of: Horrors, Mysteries, challenges and Legendary monster battles (yes, goblins included!)

## 🧙‍♂️ DND-SERVICE

This section explains how users can connect to the service and what they can expect when using it.

---

## session system.

### How to Connect

The system is designed according to the Benternet uniform style, as outlined by the lector, and has been extended for DND-SERVICE.

### 🔗 Communication Structure

| Field   | Purpose                        |
|:--------|:-------------------------------|
| Topic   | The service identifier         |
| Session | Lobby or user-created session  |
| Name    | Personalized username          |
| Message | Commands or data to interact   |

This format allows multiple users to connect to the same session while keeping communication organized.

### Message Direction

- **Sending queries**: Use the `?` suffix.
- **Receiving answers**: Use the `!` suffix.

Format:
- Sending: `topic>session?>Username>Message`
- Receiving: `topic>session!>Username>Message`

### Example User Queries
```
1. DND_SESSION>lobby?>User>hello_world
2. DND_SESSION>lobby?>User>!help
3. DND_SESSION>lobby?>User>!play!
```

### Example System Responses
```
1. DND_SESSION>lobby!>User>[ CORRECT USAGE: ] topic>session>!**command** : for commands: !commands
2. DND_SESSION>lobby!>User>[ CORRECT USAGE: ] topic>session>!**command** : for commands: !commands
3. DND_SESSION>lobby!>User>Your personal session code is: (3--<32)
```

---

## Lobby System

The user can access their DND session by connecting to the **lobby**.  
In the lobby, users can interact with the service using various commands to:
- Create a new game
- Get service information
- View active users
- List current sessions
- Retrieve help documentation

### Available Lobby Commands
- **!commands** — List all available commands.
- **!about** — Service information.
- **!help** — Usage help and syntax guide.
- **!play!** — Starts a new DND session.
- **!time** — Returns the current server time.
- **!delim** — Shows the current delimiter.
- **!ActiveUsers** — Shows the number of currently active users.
- **!Sessions** — Lists current sessions and players.
- **!status** — Displays the current service status.
- **!last_heartbeat** — Shows the last heartbeat timestamp.

---

## 🧝‍♂️ Game Session

Once inside a game session, users can interact with the AI Dungeon Master using actions marked with a `!`.  
The list of available actions can be retrieved with the `!actions` command.

The familiar lobby commands remain available inside game sessions for a smooth and consistent user experience.

### Available Game Commands
- **!commands** — List all available commands.
- **!about** — Service information.
- **!help** — Usage help and syntax guide.
- **!actions** — List all available actions.
- **!ActiveUsers** — Shows the number of currently active users.

### Available Actions
- **!say** — Perform an in-world action as the player.
- **!roll** — Perform a roll.
- **!quit** — Quit the current session.
- **!gold** — View the current gold amount.
- **!items** — View the inventory items.
- **!interactions** — List all users the player has interacted with.

The rest is up to the players!  
The AI Dungeon Master generates scenarios based on player actions, creating a unique, dynamic experience every time.


### 🌐 Outsourced Services

Each service within the system comes with its own **dependency list**, annotated in a generated **JSON object** during class instantiation.

This JSON object:
- Defines the services that the current service depends on.
- Enables a **modular approach** to extending features dynamically.
- Allows easy querying and integration with other services during runtime.

---

#### Service Dependency Management

Upon creation, the service:
1. Parses its dependency list from the JSON object.
2. Establishes the necessary connections.
3. Dynamically creates Pipelines responsible for interfacing with the external services.

This architecture allows seamless expansion of functionalities without needing to hardcode dependencies.

---

#### 🎲 Example: Dice Service Integration

One outsourced service example is the **Dice Service** (`Dice>dx`), where users can retrieve random dice rolls based on their desired dice size.

Supported dice examples:
- **d2** (coin flip)
- **d4**
- **d20** (standard DND dice)
- **d100** (percentile roll)

#### How It Works
- The DND-Service reads its dependency list and detects the `Dice>dx` service.
- A dedicated **Pipeline** is created to handle communication with the Dice service.
- Users inside the DND session can now request dice rolls without the DND-Service itself handling the randomization logic.
- This keeps services **modular**, **lightweight**, and **focused**.

---

## 🏗️ FRAMEWORK

Handling traffic from user prompts to system calls is managed through a custom-built framework.

---

### ✨ Key Features

- **Modular Pipeline Framework**  
  Create, customize, and extend service pipelines easily in a plug-and-play manner.
- **ZMQPP Integration**  
  Connects and communicates over the Benternet network.
- **Extensible Backend**  
  Primarily built in **C++**, with future frontend support using **JavaScript**.

---

### 🧩 Inner Workings

The framework revolves around three main classes:

- **BManager**  
  Singleton class responsible for managing the ZMQ context. Handles creating, adding, removing, and interfacing with user-defined pipelines. It also provides an interface for enabling, disabling, making pipelines continuous, and adding elements to pipelines.

- **PFactory**  
  Factory class for creating **Pipeline** wrapper objects using the builder pattern. Pipelines created by PFactory are managed and executed by the BManager.

- **EFactory**  
  Factory class for creating **Element** objects (also using the builder pattern) which are consumed by Pipelines.

---

## 📋 BManager Key Functionality

- Manages all user-created pipelines.
- Polls events for each pipeline:
  - **POLLIN** (incoming message)
  - **POLLOUT** (ready to send)
  - or a **filter object callback** (cb).
- Provides an interface to:
  - Push and pop pipelines.
  - Run pipelines.
  - Set pipelines to continuous mode.

---

## 🏭 *Factory objects
interface for creating pipelines and elements.


## Factory Objects

Both **PFactory** and **EFactory** offer interfaces for:

- Creating new objects without the need to see internal functionality.
- Simplifying object construction using the builder pattern.

---

## ⚙️ Pipelines

Pipelines act as containers for holding multiple **Element** objects.  
Key properties:
- They can hold **user-defined data** using `templates` and `std::any`.
- Data is accessible and modifiable across each Element within a Pipeline.

---

## 🔥 Elements

Elements perform specific tasks such as:

- **Receiving** from sockets.
- **Processing** data (filters).
- **Sending** data to sockets.

Each Element has a **callback (cb)** that defines its specific behavior:
- **Recv Elements**:  
  - Deserialize incoming `zmq::message_t` messages.
- **Filter Elements**:  
  - Process and transform data according to user-defined logic.
- **Send Elements**:  
  - Serialize processed data back into `zmq::message_t` messages for transmission.

---

### 🛠️ Used Libraries

- [ZeroMQ / zmqpp](https://github.com/zeromq/zmqpp)
- [nlohmann/json (JSON for Modern C++)](https://github.com/nlohmann/json)
- [openai-cpp (Unofficial OpenAI C++ SDK)](https://github.com/Olivine-Labs/openai-cpp)

---

## Diagrams and Workflow

*(Diagrams coming soon!)*

- **Workflow Overview Diagram** — How user prompts travel through the framework.
- **Class Relationship Diagram** — How BManager, PFactory, EFactory, Pipelines, and Elements interact.
- **Pipeline Lifecycle Diagram** — Building, running, and modifying pipelines dynamically.

![Workflow Overview](path/to/your_workflow_image.png)

![Class Relationships](path/to/your_class_diagram.png)

### ✍️ Contributors
- [**Oliwier Jaworski**](https://github.com/OliwierJaworski)
