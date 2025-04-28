# Benternet-services-project

![Overview](cooleimage.png)

## About the project
The **Benternet Services Project** is a terminal- and session-based service running on the Benternet network. The backend is primarily developed in **C++**, with planned expansions using **JavaScript** to provide an HTML-based graphical user interface.

At its core, the project relies on a custom-built framework, utilizing the **zmqpp** library to interface with the Benternet network. The framework design is inspired by the **GStreamer pipeline** model, empowering users to construct their own modular and flexible service pipelines.

The modular pipeline serves as the foundation for the first major service: **DND-SERVICE**.
In the DND-SERVICE, players embark on a **Dungeons & Dragons**-inspired campaign, guided by an AI Dungeon Master.
The AI Dungeon Master dynamically adapts the world and scenarios to the players' actions, ensuring a unique and immersive experience each time.
Whether venturing alone or teaming up with friends, users can explore a world full of: Horrors, Mysteries, challenges and Legendary monster battles (yes, goblins included!)

## DND-SERVICE

This section explains how users can connect to the service and what they can expect when using it.

---

## session system.

### How to Connect

The system is designed according to the Benternet uniform style, as outlined by the lector, and has been extended for DND-SERVICE.

### Communication Structure

It consists of four parts:
- **Topic**: The service identifier.
- **Session**: A lobby or user-created session.
- **Name**: Personalized username for identification.
- **Message**: Commands or interaction data.

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

## Game Session

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
- **!quit** — Quit the current session.
- **!gold** — View the current gold amount.
- **!items** — View the inventory items.
- **!interactions** — List all users the player has interacted with.

---

The rest is up to the players!  
The AI Dungeon Master generates scenarios based on player actions, creating a unique, dynamic experience every time.

## FRAMEWORK
handling the traffic from user prompt to system call will be handled using the framework at hand.
### Key Features
- **Modular Pipeline Framework**  
  Create, customize, and extend service pipelines easily in a plug-and-play manner.
- **ZMQPP Integration**  
  Seamlessly connects and communicates over the Benternet network.
- **Extensible Backend**  
  Primarily built in C++, with future frontend support using JavaScript.
### Inner workings
### socket logic
### USED libraries
- zmq/zmqpp
- nlohmann
- openai-cpp






### Getting started

### Contributors
- [**Oliwier Jaworski**](https://github.com/OliwierJaworski)