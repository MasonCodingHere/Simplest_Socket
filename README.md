# Simplest_Socket

[![CI](https://github.com/bitdove/Simplest_Socket/actions/workflows/ci.yml/badge.svg?branch=main&event=push)](https://github.com/bitdove/Simplest_Socket/actions/workflows/ci.yml)

## Introduction

This project is the first choice for learning UNIX network programming, and is the "Hello World" in the field of network programming.

This project is based on Linux socket and implements the simplest network communication between two hosts. The client sends lowercase characters to the server, and the server converts them to uppercase and sends them back to the client.

## Environment

- OS：CentOS 7.5
- Compiler：gcc 4.8
- CMake：version 3.19.6
- Make：GNU Make 3.82
- Shell：GNU bash, version 4.2.46

## Usage

### Clone

```shell
git clone https://github.com/bitdove/Simplest_Socket.git
```

### Build

```shell
cd Simplest_Socket
./build.sh
```

### Run

```shell
cd bin
./server 或 ./epoll_server
./client
```

> Open two shells and run the server and client respectively.
> Note: epoll_server still has some problems:
> - The IP:Port displayed when disconnecting is incorrect.

### Communication

**Client**

```
Enter an English string
//Q means quit
```

You can see that the server displays the English string entered by the client, and the client displays the uppercase form of the corresponding string.
