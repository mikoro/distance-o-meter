/**
	Copyright (c) 2008 Mikko Ronkainen
	This software is licensed under the MIT License.
	
	@file main.c
	@brief Main program for the Distant-O-Meter device
	
	Compiler:	GCC-AVR
	Target:		Atmel AVR devices
	ChangeLog:	22.3.08 Initial version
				24.3.08 SRF08 presence detection
				27.3.08 Fine tuned lcd output
*/

#include "lcd.h"
#include "i2c.h"
#include "uart.h"
#include "srf08.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

volatile bool connected = false;
bool keepalive = true;

static void doInitializationEffects();

int main()
{
	lcdInitialize();
	i2cInitialize();
	uartInitialize();
	uartEnableInterrupt();

	lcdSetCursor(false);
	lcdSetBlink(false);

	doInitializationEffects();

	// setup timer1 so that it fires at 1Hz, if F_CPU is 1843200Hz
	TCCR1B |= (1 << WGM12);
	TIMSK |= (1 << OCIE1A);
	sei();
	OCR1A   = 28800;
	TCCR1B |= ((1 << CS10) | (1 << CS11));

	while(1)
	{
		// if not connected, act as a standalone meter, doing continuous rangings
		// if connected, everything is dealed in the interrupts
		if(!connected)
		{
			// initiate ranging
			srf08InitiateRanging(SRF08_UNIT_CENTIMETERS);
			bool ready = srf08WaitReady(true);

			lcdSetPosition(0, 1);
			lcdWriteString("                ");

			// if not ready, srf08 is most likely missing
			if(!ready)
			{
				lcdSetPosition(0, 1);
				lcdWriteString("SRF08 not found!");
				continue;
			}

			uint8_t light = srf08GetLight();
			uint16_t distance = srf08GetDistance(0);
			char result[6];
			itoa(distance, result, 10);
			lcdSetPosition(0, 1);
			lcdWriteString("D: ");
			lcdWriteString(result);
			itoa(light, result, 10);
			lcdSetPosition(10, 1);
			lcdWriteString("L: ");
			lcdWriteString(result);
		}
		else
		{
			lcdSetPosition(0, 1);
			lcdWriteString("Connected       ");
		}

		_delay_ms(10);
	}
}

static void doInitializationEffects()
{
	// following bitmaps: - \ | /
	uint8_t bitmaps [] = { 128,128,128,159,128,128,128,128,128,144,136,132,130,129,128,128,128,132,132,132,132,132,132,128,128,129,130,132,136,144,128,128 };

	// write bitmaps into custom bitmap slots
	for(uint8_t i = 0; i < 4; ++i)
		lcdWriteBitmap(i, bitmaps + i * 8);

	_delay_ms(500);

	// do two way scrolling
	for(uint8_t i = 0; i < 9; ++i)
	{
		lcdSetPosition(96 + i, 0);
		lcdWriteString("Distance");

		lcdSetPosition(19 - i, 0);
		lcdWriteString("Meter");

		_delay_ms(100);
	}

	_delay_ms(500);
	lcdSetPosition(8, 0);
	lcdWriteString("-O-");
	_delay_ms(1000);
	lcdSetPosition(0, 1);
	lcdWriteString("Initializing...");

	// animate a busy indicator
	for(uint8_t i = 0; i < 8; ++i)
	{
		for(uint8_t j = 0; j < 4; ++j)
		{
			lcdSetPosition(15, 1);
			lcdWriteCharacter(j);
			_delay_ms(100);
		}
	}

	lcdSetPosition(0, 1);
	lcdWriteString("     Ready!     ");
	_delay_ms(2000);
}

// UART reception interrupt
ISR(USART_RXC_vect)
{
	uint8_t input = uartReceiveNoWait();

	switch(input)
	{
		// connection request
		case 'c':
		{
			if(!connected)
			{
				// do simple handshake
				uartTransmit('a');
				uint8_t reply = uartReceive(true);
				connected = (reply == 'a');

				keepalive = true;
			}

		} break;

		// ranging request
		case 'r':
		{
			// only respond if verified connection is established
			if(connected)
			{
				srf08InitiateRanging(SRF08_UNIT_CENTIMETERS);
				connected = srf08WaitReady(true);

				if(connected)
				{
					// write back all 17 echo values
					for(uint8_t i = 0; i < 17; ++i)
					{
						uint16_t distance = srf08GetDistance(i);
						uartTransmit(distance >> 8);
						uartTransmit(distance);
					}

					keepalive = true;
				}
			}

		} break;

		// disconnect request
		case 'd':
		{
			if(connected)
			{
				connected = false;
				lcdSetPosition(0, 1);
				lcdWriteString("Disconnected");
				_delay_ms(1000);
			}

		} break;

		// keep alive ping pong
		case 'k':
		{
			if(connected)
			{
				uartTransmit('k');
				keepalive = true;
			}

		} break;
	}
}

// timer interrupt
ISR(TIMER1_COMPA_vect)
{
	// if other party has not kept the connection alive, close it
	if(connected)
	{
		if(keepalive)
			keepalive = false;
		else
		{
			connected = false;
			lcdSetPosition(0, 1);
			lcdWriteString("Disconnected");
			_delay_ms(1000);
		}
	}
}
