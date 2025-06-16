# Chat App: Broker-Client-Server Model in C++

This project implements a terminal-based chat application using a **Broker-Client** architecture in **C++** with **POSIX sockets** and **multithreading**.

## 🔧 Features

- 🗣️ Multiple chat rooms
- 👤 Private messaging using `@username <message>`
- 📊 Active room tracking
- 🔀 Concurrent client handling with threads
- 🧵 Thread-safe message broadcasting with mutex

## 🧱 Architecture


The **broker** (server) acts as a central relay for client messages and manages chat rooms.

## 🚀 Getting Started

### 1. Build

```bash
make

[ Client ] <---> [ Broker / ChatServer ] <---> [ Client ]


The **broker** (server) acts as a central relay for client messages and manages chat rooms.

## 🚀 Getting Started

### 1. Build

```bash
make

./chat_app broker [port]

./chat_app client <username> <room> [port]

./chat_app client Alice Room1
