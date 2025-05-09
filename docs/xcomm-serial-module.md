# Xcomm 串口模块 API 文档

## 目录

- [Xcomm 串口模块 API 文档](#xcomm-串口模块-api-文档)
  - [目录](#目录)
  - [简介](#简介)
  - [特性](#特性)
  - [限制](#限制)
  - [API接口](#api接口)
  - [示例代码](#示例代码)

## 简介

专门用于通过串行接口（例如RS232或RS485）与外部设备进行通信。该模块提供了创建连接、关闭连接以及发送和接收数据的功能。

## 特性

* **跨平台支持**：模块设计为跨平台使用，支持多种操作系统，包括 Windows、Linux、macOS、iOS、Android 和 HarmonyOS
 

## 限制

* 当前版本暂不支持非阻塞I/O模式。如果您需要非阻塞操作的支持，请考虑使用其他库或者关注未来的更新。

## API接口
```c
struct xcomm_serial_module_s {
    /**
     * @brief 模块的名称。
     * 
     * 用于标识该模块的名称字符串。例如："Xcomm Serial Module"。
     */
    const char* restrict name;

    /**
     * @brief 创建一个新的串行连接。
     * 
     * 根据提供的配置信息创建一个新的串行连接，并返回一个指向该连接的对象。
     * 
     * @param config 连接配置信息，包括设备路径、波特率等。
     * @return 成功时返回新建的串行对象指针，失败时返回NULL。
     */
    xcomm_serial_t* (*xcomm_dial)(xcomm_serial_config_t* config);

    /**
     * @brief 关闭指定的串行连接。
     * 
     * 断开与串行设备的连接并释放相关资源。
     * 
     * @param serial 要关闭的串行对象指针。
     */
    void (*xcomm_close)(xcomm_serial_t* serial);

    /**
     * @brief 从串行设备接收数据。
     * 
     * 尝试从串行设备读取指定长度的数据到缓冲区中。
     * 
     * @param serial 已建立的串行连接对象。
     * @param buf 接收数据的缓冲区。
     * @param len 缓冲区的长度。
     * @return 成功时返回实际接收到的数据字节数，失败时返回负数。
     */
    int (*xcomm_recv)(xcomm_serial_t* serial, uint8_t* buf, int len);

    /**
     * @brief 向串行设备发送数据。
     * 
     * 将指定长度的数据通过串行连接发送出去。
     * 
     * @param serial 已建立的串行连接对象。
     * @param buf 发送数据的缓冲区。
     * @param len 缓冲区的长度。
     * @return 成功时返回实际发送的数据字节数，失败时返回负数。
     */
    int (*xcomm_send)(xcomm_serial_t* serial, uint8_t* buf, int len);
};
```

## 示例代码
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
    };
    xcomm_serial_t* serial =
        xcomm_serial_module.xcomm_dial(&serial_config);
    if (!serial) {
        return -1;
    }
    char* buffer = "hello world";
    xcomm_serial_module.xcomm_send(serial, buffer, strlen(buffer));
    printf("write %s to serial.\n", buffer);

    xcomm_serial_module.xcomm_close(serial);
    return 0;
}
```