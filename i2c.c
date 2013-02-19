/*
	Copyright (c) 2008 Mikko Ronkainen
	This software is licensed under the MIT License.
*/

#include "i2c.h"
#include "util.h"

#include <avr/io.h>
#include <util/delay.h>

#define DATAPORT_OUT CONCAT(PORT, I2C_DATAPORT)
#define DATAPORT_IN CONCAT(PIN, I2C_DATAPORT)
#define DATAPORT_DIR CONCAT(DDR, I2C_DATAPORT)
#define PIN_SDA (1 << I2C_PIN_SDA)
#define PIN_SCL (1 << I2C_PIN_SCL)

#define SDA_1() TURN_OFF(PIN_SDA, DATAPORT_DIR) // act as high resistance -> external pull-up pulls line to +5v
#define SDA_0() TURN_ON(PIN_SDA, DATAPORT_DIR) // act as fast sink  -> microcontroller pulls line to ground (PORTXn should be 0)
#define SCL_1() TURN_OFF(PIN_SCL, DATAPORT_DIR)
#define SCL_0() TURN_ON(PIN_SCL, DATAPORT_DIR)

// to understand what's going on here, refer to this:
// http://www.esacademy.com/faq/i2c/general/i2cproto.htm

static void i2cDelay()
{
	_delay_us(I2C_DELAY);
}

void i2cInitialize()
{
	TURN_OFF(PIN_SDA, DATAPORT_OUT);
	TURN_OFF(PIN_SCL, DATAPORT_OUT);
	SDA_1();
	SCL_1();
}

void i2cStart()
{
	SDA_1();
	SCL_1();
	i2cDelay();
	SDA_0();
	i2cDelay();
	SCL_0();
	i2cDelay();
}

void i2cStop()
{
	SDA_0();
	SCL_1();
	i2cDelay();
	SDA_1();
	i2cDelay();
}

bool i2cTransmit(uint8_t data)
{
	for(uint8_t i = 0; i < 8; ++i)
	{
		(data & 0x80) ? SDA_1() : SDA_0();
		SCL_1();
		i2cDelay();
		SCL_0();
		i2cDelay();
		data <<= 1;
	}

	SDA_1();
	SCL_1();
	i2cDelay();
	bool ack = (DATAPORT_IN & PIN_SDA);
	SCL_0();
	i2cDelay();

	return ack;
}

uint8_t i2cReceive(bool ack)
{
	SDA_1();
	uint8_t input = 0;

	for(uint8_t i = 0; i < 8; ++i)
	{
		// wait for clock scretching
		do { SCL_1(); }
		while(!(DATAPORT_IN & PIN_SCL));

		i2cDelay();
		input <<= 1;
		input |= ((DATAPORT_IN & PIN_SDA) != 0);
		SCL_0();
		i2cDelay();
	}

	ack ? SDA_0() : SDA_1();
	SCL_1();
	i2cDelay();
	SCL_0();
	i2cDelay();

	return input;
}
