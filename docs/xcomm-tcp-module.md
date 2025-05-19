# Xcomm TCP Module API Documentation

## Introduction

Specifically designed for communication with external devices via the TCP/IP protocol (e.g., TCP connections). 
This module provides functionalities to create connections, listen for connection requests, accept connections, and send and receive data.

## Features

* **Cross-platform**：The module is designed for cross-platform use, supporting various operating systems including Windows, Linux, macOS, iOS, Android, and HarmonyOS.
* **IPv4/IPv6 dual stack**：The module supports not only traditional IPv4 address formats but is also fully compatible with and supports IPv6 address formats.

## Requirements

* N/A

## Module
```c
extern xcomm_sync_tcp_module_t  xcomm_sync_tcp;
extern xcomm_async_tcp_module_t xcomm_async_tcp;
```

## Module Interfaces
```c
struct xcomm_sync_tcp_module_s {
    /**
     * @brief The name of the module.
     * 
     * A string used to identify this module. For example: "Xcomm Sync TCP Module".
     */
    const char* restrict name;

    /**
     * @brief Creates a new TCP client connection.
     * 
     * Creates a new TCP client connection based on the provided host and port information, and returns a pointer to that connection object.
     * 
     * @param host Remote host address or domain name (supports IPv6).
     * @param port Remote host port number.
     * @return Returns a pointer to the newly created socket object on success, NULL on failure.
     */
    xcomm_socket_t* (*xcomm_dial)(const char* restrict host, const char* restrict port);

    /**
     * @brief Listens for TCP connection requests on a specified host and port.
     * 
     * Opens listening on the specified local host address and port, ready to accept connection requests from  clients.
     * 
     * @param host Local host address or domain name to listen on; can be NULL to indicate any available address.
     * @param port Local port number to listen on.
     * @return Returns a pointer to the socket object on success, NULL on failure.
     */
    xcomm_socket_t* (*xcomm_listen)(const char* restrict host, const char* restrict port);

    /**
     * @brief Accepts an established TCP connection.
     * 
     * Accepts a TCP connection request that has already been listened for using listen() method, and returns the corresponding socket object.
     * 
     * @param socket A socket object that is in listening state.
     * @return Returns a pointer to the new socket object representing the newly established connection on success, NULL on failure.
     */
    xcomm_socket_t* (*xcomm_accept)(xcomm_socket_t* socket);

    /**
     * @brief Receives data from a TCP connection.
     * 
     * Attempts to read a specified amount of data from the TCP connection into a buffer.
     * 
     * @param sock An established socket connection object.
     * @param buf Buffer to receive data.
     * @param len Length of the buffer.
     * @return Returns the actual number of bytes received on success, a negative number on failure.
     */
    int (*xcomm_recv)(xcomm_socket_t* sock, void* buf, int len);

    /**
     * @brief Sends data over a TCP connection.
     * 
     * Sends a specified amount of data through the TCP connection.
     * 
     * @param sock An established socket connection object.
     * @param buf Buffer containing the data to send.
     * @param len Length of the buffer.
     * @return Returns the actual number of bytes sent on success, a negative number on failure.
     */
    int (*xcomm_send)(xcomm_socket_t* sock, void* buf, int len);

    /**
     * @brief Closes a specified TCP connection.
     * 
     * Disconnects the TCP connection and releases associated resources.
     * 
     * @param sock Pointer to the socket object to close.
     */
    void (*xcomm_close)(xcomm_socket_t* sock);

    /**
     * @brief Sets the send timeout.
     * 
     * Sets the timeout for TCP send operations.
     * 
     * @param sock Pointer to the socket object.
     * @param timeout_ms Timeout duration in milliseconds.
     */
    void (*xcomm_sendtimeo)(xcomm_socket_t* sock, int timeout_ms);

    /**
     * @brief Sets the receive timeout.
     * 
     * Sets the timeout for TCP receive operations.
     * 
     * @param sock Pointer to the socket object.
     * @param timeout_ms Timeout duration in milliseconds.
     */
    void (*xcomm_recvtimeo)(xcomm_socket_t* sock, int timeout_ms);
};
```
```c
struct xcomm_async_tcp_module_s {
    const char* restrict name;
};
```

## Example Code(Synchronous)
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

## Example Code(Asynchronous )
```c
#include <stdio.h>
#include <string.h>

#include "xcomm.h"

int main(void) {
    
    return 0;
}
```