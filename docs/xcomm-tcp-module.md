# Xcomm 同步TCP模块 API 文档

## 目录

- [Xcomm 同步TCP模块 API 文档](#xcomm-同步TCP模块-api-文档)
  - [目录](#目录)
  - [简介](#简介)
  - [特性](#特性)
  - [限制](#限制)
  - [API接口](#api接口)
  - [示例代码](#示例代码)

## 简介

专门用于通过TCP/IP协议（例如TCP连接）与外部设备进行通信。该模块提供了创建连接、监听连接请求、接受连接以及发送和接收数据的功能。

## 特性

* **跨平台支持**：模块设计为跨平台使用，支持多种操作系统，包括 Windows、Linux、macOS、iOS、Android 和 HarmonyOS
* **IPv6支持**：该模块不仅支持传统的IPv4地址格式，也完全兼容并支持IPv6地址格式

## 限制

* N/A

## API接口
```c
struct xcomm_serial_module_s {
    /**
     * @brief 模块的名称。
     * 
     * 用于标识该模块的名称字符串。例如："Xcomm Sync TCP Module"。
     */
    const char* restrict name;

    /**
     * @brief 创建一个新的TCP客户端连接。
     * 
     * 根据提供的主机和端口信息创建一个新的TCP客户端连接，并返回一个指向该连接的对象。
     * 
     * @param host 远程主机地址或域名，（支持IPv6）。
     * @param port 远程主机端口号。
     * @return 成功时返回新建的socket对象指针，失败时返回NULL。
     */
    xcomm_socket_t* (*xcomm_dial)(const char* restrict host, const char* restrict port);

    /**
     * @brief 在指定的主机和端口上监听TCP连接请求。
     * 
     * 在指定的主机和端口上开启监听，准备接受来自客户端的连接请求。
     * 
     * @param host 要监听的本地主机地址或域名，可以为空表示任意可用地址。
     * @param port 要监听的本地端口号。
     * @return 成功时返回socket对象指针，失败时返回NULL。
     */
    xcomm_socket_t* (*xcomm_listen)(const char* restrict host, const char* restrict port);

    /**
     * @brief 接受一个已建立的TCP连接。
     * 
     * 接受一个已经通过listen()方法监听到的TCP连接请求，并返回对应的socket对象。
     * 
     * @param socket 已经处于监听状态的socket对象。
     * @return 成功时返回新的socket对象指针，代表新建立的连接，失败时返回NULL。
     */
    xcomm_socket_t* (*xcomm_accept)(xcomm_socket_t* socket);

    /**
     * @brief 从TCP连接中接收数据。
     * 
     * 尝试从TCP连接中读取指定长度的数据到缓冲区中。
     * 
     * @param sock 已建立的socket连接对象。
     * @param buf 接收数据的缓冲区。
     * @param len 缓冲区的长度。
     * @return 成功时返回实际接收到的数据字节数，失败时返回负数。
     */
    int (*xcomm_recv)(xcomm_socket_t* sock, void* buf, int len);

    /**
     * @brief 向TCP连接发送数据。
     * 
     * 将指定长度的数据通过TCP连接发送出去。
     * 
     * @param sock 已建立的socket连接对象。
     * @param buf 发送数据的缓冲区。
     * @param len 缓冲区的长度。
     * @return 成功时返回实际发送的数据字节数，失败时返回负数。
     */
    int (*xcomm_send)(xcomm_socket_t* sock, void* buf, int len);

    /**
     * @brief 关闭指定的TCP连接。
     * 
     * 断开与TCP连接并释放相关资源。
     * 
     * @param sock 要关闭的socket对象指针。
     */
    void (*xcomm_close)(xcomm_socket_t* sock);

    /**
     * @brief 设置发送超时时间。
     * 
     * 设置TCP发送操作的超时时间。
     * 
     * @param sock socket对象指针。
     * @param timeout_ms 超时时间，单位毫秒。
     */
    void (*xcomm_sendtimeo)(xcomm_socket_t* sock, int timeout_ms);

    /**
     * @brief 设置接收超时时间。
     * 
     * 设置TCP接收操作的超时时间。
     * 
     * @param sock socket对象指针。
     * @param timeout_ms 超时时间，单位毫秒。
     */
    void (*xcomm_recvtimeo)(xcomm_socket_t* sock, int timeout_ms);
};
```

## 示例代码
```c
#include <stdio.h>
#include <string.h>

#include "xcomm.h"

int main(void) {
    char sbuf[64] = "ping";
    char rbuf[64] = {0};
    xcomm_socket_t* socket =
        xcomm_sync_tcp_module.xcomm_dial("127.0.0.1", "1234");
    
    xcomm_sync_tcp_module.xcomm_send(socket, sbuf, sizeof(sbuf));
    printf("cli send %s to srv.\n", sbuf);

    xcomm_sync_tcp_module.xcomm_recv(socket, rbuf, sizeof(rbuf));
    printf("cli recv %s from srv.\n", rbuf);

    xcomm_sync_tcp_module.xcomm_close(socket);
    return 0;
}
```