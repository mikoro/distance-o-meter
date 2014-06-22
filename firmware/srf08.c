/*
	Copyright (c) 2008 Mikko Ronkainen
	This software is licensed under the MIT License.
*/

#include "srf08.h"
#include "i2c.h"

#include <util/delay.h>

// refer to this page for better understanding:
// http://www.robot-electronics.co.uk/htm/srf08tech.shtml

// 0xe0 is the factory default address
uint8_t currentAddress = 0xe0;

void srf08SelectAddress(uint8_t address)
{
	// valid address values are 0x00 and [0xe0, 0xfe ]
	if((address != 0x00) && (address < 0xe0 || address > 0xfe))
		return;

	// only even values in the range [ 0xe0, 0xfe ]
	if(address % 2 != 0)
		return;

	currentAddress = address;
}

void srf08ChangeAddress(uint8_t newAddress)
{
	if(newAddress < 0xe0 || newAddress > 0xfe)
		return;

	if(newAddress % 2 != 0)
		return;

	// address can be changed by writing following values
	// in sequence into command register: 0xa0 0xaa 0xa5 address
	i2cStart();
	i2cTransmit(currentAddress); // select current device
	i2cTransmit(0x00); // // select command register
	i2cTransmit(0xa0);
	i2cStop();

	i2cStart();
	i2cTransmit(currentAddress);
	i2cTransmit(0x00);
	i2cTransmit(0xaa);
	i2cStop();

	i2cStart();
	i2cTransmit(currentAddress);
	i2cTransmit(0x00);
	i2cTransmit(0xa5);
	i2cStop();

	i2cStart();
	i2cTransmit(currentAddress);
	i2cTransmit(0x00);
	i2cTransmit(newAddress);
	i2cStop();

	currentAddress = newAddress;
}

void srf08SetRange(uint16_t range)
{
	// range is calculated: ((Range Register x 43mm) + 43mm)
	uint8_t actualValue = (range /= 43) & 0xff;

	i2cStart();
	i2cTransmit(currentAddress); // select current device
	i2cTransmit(0x02); // select range register
	i2cTransmit(actualValue); // write the range value
	i2cStop();
}

void srf08SetGain(uint8_t gain)
{
	if(gain > 31)
		gain = 31;

	i2cStart();
	i2cTransmit(currentAddress);
	i2cTransmit(0x01); // select gain register
	i2cTransmit(gain);
	i2cStop();
}



void srf08InitiateRanging(uint8_t units)
{
	// valid unit values
	if(units != SRF08_UNIT_INCHES &&
	   units != SRF08_UNIT_CENTIMETERS &&
	   units != SRF08_UNIT_MICROSECONDS)
	   return;

	i2cStart();
	i2cTransmit(currentAddress);
	i2cTransmit(0x00); // select command register
	i2cTransmit(units); // write appropriate unit identifier to start ranging
	i2cStop();
}

bool srf08IsBusy()
{
	// if device is bysy, it'll leave sda high so we get 0xff as version
	return (srf08GetVersion() == 0xff);
}

bool srf08WaitReady(bool timeout)
{
	if(timeout)
	{
		for(uint16_t i = 0; i < SRF08_WAIT_TIMEOUT; ++i)
		{
			if(srf08IsBusy())
			{
				_delay_ms(1);
				continue;
			}
			else
				return true;
		}

		return false;
	}
	else
		while(srf08IsBusy());

	return true;
}

uint8_t srf08GetVersion()
{
	i2cStart();
	i2cTransmit(currentAddress);
	i2cTransmit(0x00); // select software version register
	i2cStart(); // restart to indicate read operation
	i2cTransmit(currentAddress + 1); // select current device's read address
	uint8_t version = i2cReceive(false); // get the value at version register
	i2cStop();

	return version;
}

uint8_t srf08GetLight()
{
	i2cStart();
	i2cTransmit(currentAddress);
	i2cTransmit(0x01); // select light register
	i2cStart();
	i2cTransmit(currentAddress + 1);
	uint8_t light = i2cReceive(false);
	i2cStop();

	return light;
}

uint16_t srf08GetDistance(uint8_t echo)
{
	// valid range is [0, 16]
	if(echo > 16)
		echo = 16;

	i2cStart();
	i2cTransmit(currentAddress);
	i2cTransmit(2 + echo * 2); // select the correct distance register
	i2cStart();
	i2cTransmit(currentAddress + 1);
	uint16_t distance = i2cReceive(true) << 8; // get the high byte
	distance |= i2cReceive(false); // get the low byte
	i2cStop();

	return distance;
}
