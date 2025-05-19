# Xcomm Serial Module API Documentation

## Introduction
The Xcomm Serial Module is a lightweight, cross-platform library for serial communication. 
It provides an easy-to-use API to configure and manage serial ports across multiple operating systems, including Windows, Linux, macOS, iOS, Android, and HarmonyOS.
Ideal for connecting with external devices like sensors or embedded systems.

## Features

* **Cross-platform**：The module is designed for cross-platform use, supporting various operating systems including Windows, Linux, macOS, iOS, Android, and HarmonyOS.

## Requirements

* N/A

## Module
```c
extern xcomm_serial_module_t xcomm_serial;
```

## Module Interfaces
```c
struct xcomm_serial_module_s {
    /**
     * @brief The name of the module.
     * 
     * A string used to identify this module. For example: "Xcomm Serial Module".
     */
    const char* restrict name;

    /**
     * @brief Establishes a new serial connection.
     * 
     * Initializes and opens a new serial connection based on the provided configuration.
     * 
     * @param config Pointer to a configuration structure that specifies the parameters for the serial connection.
     * @return Returns a pointer to the newly created serial connection object on success, NULL on failure.
     */
    xcomm_serial_t* (*dial)(xcomm_serial_config_t* config);

    /**
     * @brief Closes an existing serial connection.
     * 
     * Terminates the specified serial connection and releases any associated resources.
     * 
     * @param serial Pointer to the serial connection object to be closed.
     */
    void (*close)(xcomm_serial_t* serial);

    /**
     * @brief Receives data from the serial connection.
     * 
     * Reads up to 'len' bytes of data from the serial connection into the buffer 'buf'.
     * 
     * @param serial Pointer to the established serial connection object.
     * @param buf Buffer to store received data.
     * @param len Maximum number of bytes to read.
     * @return Returns the actual number of bytes received on success, a negative number on failure.
     */
    int (*recv)(xcomm_serial_t* serial, uint8_t* buf, int len);

    /**
     * @brief Sends data over the serial connection.
     * 
     * Writes up to 'len' bytes of data from the buffer 'buf' to the serial connection.
     * 
     * @param serial Pointer to the established serial connection object.
     * @param buf Buffer containing the data to send.
     * @param len Number of bytes to write.
     * @return Returns the actual number of bytes sent on success, a negative number on failure.
     */
    int (*send)(xcomm_serial_t* serial, uint8_t* buf, int len);
};
```

## Example Code
```c
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xcomm.h"

int main(void) {
    xcomm_serial_config_t serial_config = {
        .device   = "COM2",
        .baudrate = XCOMM_SERIAL_BAUDRATE_9600,
        .parity   = XCOMM_SERIAL_PARITY_NO,
        .databits = XCOMM_SERIAL_DATABITS_CS8,
        .stopbits = XCOMM_SERIAL_STOPBITS_ONE,
        .timeout_ms  = 0,
    };
    xcomm_serial_t* serial = xcomm_serial.dial(&serial_config);
    if (!serial) {
        return -1;
    }
    char* buffer = "hello world";
    xcomm_serial.send(serial, buffer, strlen(buffer));
    printf("write %s to serial.\n", buffer);

    xcomm_serial.close(serial);
    return 0;
}
```