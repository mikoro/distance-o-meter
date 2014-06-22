/**
	Copyright (c) 2008 Mikko Ronkainen
	This software is licensed under the MIT License.
	
	@file srf08.h
	@brief Control library for the SRF08 Ultra sonic range finder
	
	Compiler:	GCC-AVR
	Target:		Atmel AVR devices
	ChangeLog:	22.3.08 Initial version
				24.3.08 Wait functionality with timeout
*/

#ifndef SRF08_H
#define SRF08_H

#include <stdint.h>
#include <stdbool.h>

#define SRF08_WAIT_TIMEOUT 500 ///< Select timeout in ms for the srf08WaitReady function

#define SRF08_UNIT_INCHES 0x50 ///< Don't change, use as arguments for srf08InitiateRanging function
#define SRF08_UNIT_CENTIMETERS 0x51
#define SRF08_UNIT_MICROSECONDS 0x52

/// @brief Select which device will be controlled
/// @details By default address 0xe0 is used, so calling this function is not mandatory
/// Fails silently if given illegal arguments
/// @param address Address of the new device in range [0xe0, 0xfe] (must be even)
void srf08SelectAddress(uint8_t address);

/// @brief Change the address of the selected device
/// @details Modified device continues being the active one
/// Ensure that only one device is listening the bus when invoking this function
/// Fails silently if given illegal arguments
/// @param newAddress New address in the range [0xe0, 0xfe] (must be even)
void srf08ChangeAddress(uint8_t newAddress);

/// @brief Set the selected device's range
/// @param range Range in millimeters (will be converted and clamped to fit the registers)
void srf08SetRange(uint16_t range);

/// @brief Set the selected device's gain
/// @param gain Gain in the range [0, 31] (will be clamped)
void srf08SetGain(uint8_t gain);

/// @brief Initiate the selected device's distance ranging
/// @details Device will be busy for an amount of time that depends on the range and gain
/// @param units One of the three SRF08_UNIT_* values
void srf08InitiateRanging(uint8_t units);

/// @brief Check whether selected device is busy
bool srf08IsBusy();

/// @brief Wait as long as selected device becomes ready or timeout occurs
/// @param timeout If true, will timeout after SRF08_WAIT_TIMEOUT, else will wait as long as necessary
/// @return True if device is ready, false if timed out
bool srf08WaitReady(bool timeout);

/// @brief Get the selected device's software version
uint8_t srf08GetVersion();

/// @brief Get the selected device's light measurement
uint8_t srf08GetLight();

/// @brief Get the selected device's range measurement
/// @details Will be in the units specified in the range initiation
/// @param echo Echo position in the range [0, 17] (will be clamped)
uint16_t srf08GetDistance(uint8_t echo);

#endif
