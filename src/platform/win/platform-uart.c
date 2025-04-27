/** Copyright (c) 2025, Archermind Tech. Co., Ltd. <marketing@archermind.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

#include "platform/platform-uart.h"

void platform_uart_close(platform_uart_t uart) {
    CloseHandle(uart);
}

int platform_uart_read(platform_uart_t uart, uint8_t* buf, int len) {
    DWORD bytes_read = 0;
    if (!ReadFile(uart, buf, len, &bytes_read, NULL)) {
        return -1;
    }
    return (int)bytes_read;
}

int platform_uart_write(platform_uart_t uart, uint8_t* buf, int len) {
    DWORD bytes_written = 0;
    if (!WriteFile(uart, buf, len, &bytes_written, NULL)) {
        return -1;
    }
    return (int)bytes_written;
}

platform_uart_t platform_uart_open(platform_uart_config_t* config) {
    platform_uart_t uart;

    uart = CreateFileA(
        config->device,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        0);
    if (uart == INVALID_HANDLE_VALUE) {
        return PLATFORM_UART_ERROR_INVALID_VALUE;
    }
    DCB dcb;
    SecureZeroMemory(&dcb, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);

    switch (config->baudrate) {
    case PLATFORM_UART_BAUDRATE_9600:
        dcb.BaudRate = CBR_9600;
        break;
    case PLATFORM_UART_BAUDRATE_19200:
        dcb.BaudRate = CBR_19200;
        break;
    case PLATFORM_UART_BAUDRATE_38400:
        dcb.BaudRate = CBR_38400;
        break;
    case PLATFORM_UART_BAUDRATE_57600:
        dcb.BaudRate = CBR_57600;
        break;
    case PLATFORM_UART_BAUDRATE_115200:
        dcb.BaudRate = CBR_115200;
        break;
    default:
        platform_uart_close(uart);
        return PLATFORM_UART_ERROR_INVALID_VALUE;
    }
    switch (config->databits) {
    case PLATFORM_UART_DATABITS_CS7:
        dcb.ByteSize = 7;
        break;
    case PLATFORM_UART_DATABITS_CS8:
        dcb.ByteSize = 8;
        break;
    default:
        platform_uart_close(uart);
        return PLATFORM_UART_ERROR_INVALID_VALUE;
    }
    switch (config->stopbits) {
    case PLATFORM_UART_STOPBITS_ONE:
        dcb.StopBits = ONESTOPBIT;
        break;
    case PLATFORM_UART_STOPBITS_TWO:
        dcb.StopBits = TWOSTOPBITS;
        break;
    default:
        platform_uart_close(uart);
        return PLATFORM_UART_ERROR_INVALID_VALUE;
    }
    switch (config->parity) {
    case PLATFORM_UART_PARITY_NO:
        dcb.Parity = NOPARITY;
        break;
    case PLATFORM_UART_PARITY_ODD:
        dcb.fParity = TRUE;
        dcb.Parity = ODDPARITY;
        break;
    case PLATFORM_UART_PARITY_EVEN:
        dcb.fParity = TRUE;
        dcb.Parity = EVENPARITY;
        break;
    default:
        platform_uart_close(uart);
        return PLATFORM_UART_ERROR_INVALID_VALUE;
    }
    dcb.fBinary = TRUE;
    if (!SetCommState(uart, &dcb)) {
        platform_uart_close(uart);
        return PLATFORM_UART_ERROR_INVALID_VALUE;
    }
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(uart, &timeouts)) {
        platform_uart_close(uart);
        return PLATFORM_UART_ERROR_INVALID_VALUE;
    }
    return uart;
}