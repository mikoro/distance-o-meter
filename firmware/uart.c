/*
	Copyright (c) 2008 Mikko Ronkainen
	This software is licensed under the MIT License.
*/

#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD_PRESCALE (((F_CPU / (UART_BAUDRATE * 16UL))) - 1)

void uartInitialize()
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UBRRL = BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8);
}

void uartEnableInterrupt()
{
	UCSRB |= (1 << RXCIE);
	sei();
}

void uartDisableInterrupt()
{
	UCSRB &= ~(1 << RXCIE);
}

void uartTransmit(uint8_t data)
{
	while (!(UCSRA & (1 << UDRE)));
	UDR = data;
}

uint8_t uartReceive(bool timeout)
{
	uint8_t input = 0;

	if(timeout)
	{
		for(uint16_t i = 0; i < UART_RECEIVE_TIMEOUT; ++i)
		{
			if(!(UCSRA & (1 << RXC)))
			{
				_delay_ms(1);
				continue;
			}
			else
			{
				input = UDR;
				break;
			}
		}
	}
	else
	{
		while (!(UCSRA & (1 << RXC)));
		input = UDR;
	}

	return input;
}

uint8_t uartReceiveNoWait()
{
	uint8_t input = UDR;
	return input;
}

void uartTransmitString(char* string)
{
	while(*string)
		uartTransmit(*string++);
}
