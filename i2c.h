/**
	Copyright (c) 2008 Mikko Ronkainen
	This software is licensed under the MIT License.
	
	@file i2c.h
	@brief Software based I2C-bus control library
	
	Compiler:	GCC-AVR
	Target:		Atmel AVR devices
	ChangeLog:	22.3.08 Initial version
*/

#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

#define I2C_DATAPORT B ///< Select dataport
#define I2C_PIN_SDA 0 ///< Select data pin
#define I2C_PIN_SCL 1 ///< Select clock pin
#define I2C_DELAY 5 ///< Select clock pulse half width in ms (5ms -> 10ms -> 100kHz)

/// @brief Set data pins into correct state
/// @details Should be called before doing anything else
void i2cInitialize();

/// @brief Issue start condition on the bus
void i2cStart();

/// @brief Issue stop condition on the bus
void i2cStop();

/// @brief Transmit a byte on the bus
/// @return Whether transmit was acknowledged by the other party or not
bool i2cTransmit(uint8_t data);

/// @brief Receive a byte on the bus
/// @param ack Whether to send acknowledge after the byte or not
/// @return The byte that was read
uint8_t i2cReceive(bool ack);

#endif
