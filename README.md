# Concurrent Chat Server (C++ & Winsock)

A **multi-client TCP chat server** built in **C++** using **Winsock** and **multithreading**, supporting **real-time message broadcasting** across connected clients.  
The project includes both **server** and **client** implementations.

---

## Features
- Multi-client support using **threads**  
- Real-time **message broadcasting** to all connected clients  
- **Socket programming** with TCP protocol (Winsock)  
- Graceful handling of client **connect/disconnect events**  
- Independent **client sessions** with concurrent message exchange  

---

## Tech Stack
- **Language**: C++  
- **Networking API**: Winsock2 (Windows Sockets API)  
- **Threads**: `std::thread`  

---

## Project Structure
```
Multi-Client Chat Server/
├── client/
│   ├── client.sln
│   ├── client.vcxproj
│   ├── client.vcxproj.filters
│   └── main.cpp         # Client implementation
│
├── server/
│   ├── server.vcxproj
│   ├── server.vcxproj.filters
│   └── main.cpp         # Server implementation
│
├── server.sln           # Combined solution file
├── .gitignore           # Ignore build & user-specific files
└── README.md            # Documentation
```

---

## Setup & Usage

### 1. Prerequisites
- Windows OS  
- Visual Studio (recommended) or MinGW  
- Winsock2 (already included with Windows SDK)

### 2. Compile

#### Using Visual Studio (IDE):
1. Open `server.sln` in Visual Studio.  
2. Build both **server** and **client** projects from the Solution Explorer.  
3. Executables will be generated in the `Debug/` or `Release/` folders depending on your build configuration.  

#### Using Visual Studio (Developer Command Prompt):
```bash
cl server\main.cpp ws2_32.lib
cl client\main.cpp ws2_32.lib
```

#### Using MinGW (g++):
```bash
g++ server\main.cpp -o server -lws2_32 -lwsock32
g++ client\main.cpp -o client -lws2_32 -lwsock32
```

### 3. Run

1. Start the **server** first:
```bash
server.exe
```

2. Start one or more **clients** (in separate terminals):
```bash
client.exe
```

3. Enter your **chat name** when prompted and start chatting!  
   - Type messages to broadcast to all connected clients.  
   - Type `exit` to leave the chat.  

---

## Example

**Server terminal:**
```
Server is listening on port: 12345
Client connected!
Message from client: Alice: Hello!
Message from client: Bob: Hi Alice!
```

**Client terminal (Alice):**
```
Enter your chat name: Alice
Alice: Hello!
Bob: Hi Alice!
```

**Client terminal (Bob):**
```
Enter your chat name: Bob
Alice: Hello!
Bob: Hi Alice!
```

---

## Concepts Demonstrated
- **Socket programming** (TCP/IP, Winsock)  
- **Multithreading** for handling multiple clients simultaneously  
- **Concurrency optimization** using detached threads  
- **Client-server architecture** and real-time data exchange  

---

## Future Improvements
- Add **private messaging** and chat rooms  
- Implement **encryption** for secure communication  
- GUI-based chat client (using Qt/SDL/ImGui)  
- Cross-platform support (Linux with POSIX sockets)  

 
