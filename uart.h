/**
	Copyright (c) 2008 Mikko Ronkainen
	This software is licensed under the MIT License.
	
	@file uart.h
	@brief Helper functions for controlling the UART
	
	Compiler:	GCC-AVR
	Target:		Atmel AVR devices
	ChangeLog:	22.3.08 Initial version
*/

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

#define UART_BAUDRATE 9600 ///< Select baudrate
#define UART_RECEIVE_TIMEOUT 1000 ///< Select timeout in ms for the uartReceive function

/// @brief Initialize the UART
/// @details Enables both transmission and reception and 8-bit mode at the baud rate of UART_BAUDRATE
void uartInitialize();

/// @brief Enable global and UART reception interrupts
void uartEnableInterrupt();

/// @brief Disable UART reception interrupt
void uartDisableInterrupt();

/// @brief Transmit given byte
void uartTransmit(uint8_t data);

/// @brief Wait if necessary and receive one byte 
/// @param timeout If true, will timeout after UART_RECEIVE_TIMEOUT, otherwise blocks as long as necessary
/// @return Zero if timed out, received byte otherwise
uint8_t uartReceive(bool timeout);

/// @brief Receive one byte without waiting
uint8_t uartReceiveNoWait();

/// @brief Transmit given string
void uartTransmitString(char* string);

#endif
