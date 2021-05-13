# Simplest_Socket

[![Build Status](https://travis-ci.com/mrxuxg/Simplest_Socket.svg?branch=master)](https://travis-ci.com/mrxuxg/Simplest_Socket)

## Introduction

**Simplest_Socket**是学习UNIX网络编程的入门首选，是网络编程领域的“Hello World”。

本项目基于Linux socket，实现两主机之间最简单的网络通信。客户端发送小写字符给服务端，服务端将其转为大写并回送给客户端。

## Environment

- OS：CentOS 7.5
- Compiler：gcc 4.8
- CMake：version 3.19.6
- Make：GNU Make 3.82
- Shell：GNU bash, version 4.2.46

## Usage

### clone

```shell
git clone https://github.com/mrxuxg/Simplest_Socket.git
```

### build

```shell
cd Simplest_Socket
./build.sh
```

### run

```shell
cd bin
./server
./client
```

> 开启两个shell，分别运行server和client。

### communication

**客户端**

```
输入英文字符串
//Q表示退出
```

可以看到服务端显示客户端输入的英文字符串，并且客户端显示对应字符串的大写形式。
