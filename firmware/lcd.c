/*
	Copyright (c) 2008 Mikko Ronkainen
	This software is licensed under the MIT License.
*/

#include "lcd.h"
#include "util.h"

#include <avr/io.h>
#include <util/delay.h>

#define DATAPORT_OUT CONCAT(PORT, LCD_DATAPORT)
#define DATAPORT_IN CONCAT(PIN, LCD_DATAPORT)
#define DATAPORT_DIR CONCAT(DDR, LCD_DATAPORT)
#define CONTROLPORT_OUT CONCAT(PORT, LCD_CONTROLPORT)
#define CONTROLPORT_DIR CONCAT(DDR, LCD_CONTROLPORT)

#ifdef LCD_8BIT_INTERFACE
	#define DATAPORT_BITS 0xff
#else
	#define DATAPORT_BITS ((1 << LCD_PIN_DB0) | (1 << LCD_PIN_DB1) | (1 << LCD_PIN_DB2) | (1 << LCD_PIN_DB3))
#endif

#define CONTROLPORT_BITS ((1 << LCD_PIN_RS) | (1 << LCD_PIN_RW) | (1 << LCD_PIN_E))

#define RS_HIGH() TURN_ON((1 << LCD_PIN_RS), CONTROLPORT_OUT)
#define RS_LOW() TURN_OFF((1 << LCD_PIN_RS), CONTROLPORT_OUT)
#define RW_HIGH() TURN_ON((1 << LCD_PIN_RW), CONTROLPORT_OUT)
#define RW_LOW() TURN_OFF((1 << LCD_PIN_RW), CONTROLPORT_OUT)
#define E_HIGH() TURN_ON((1 << LCD_PIN_E), CONTROLPORT_OUT)
#define E_LOW() TURN_OFF((1 << LCD_PIN_E), CONTROLPORT_OUT)

// for clarification, see:
// http://www.doc.ic.ac.uk/~ih/doc/lcd/instruct.html
#define INS_CLEAR_DISPLAY 0x01
#define INS_RETURN_HOME 0x02
#define INS_ENTRY(increment, shift) (0x04 | ((increment) ? 0x02 : 0) | ((shift) ? 0x01 : 0))
#define INS_DISPLAY(display, cursor, blink) (0x08 | ((display) ? 0x04 : 0) | ((cursor) ? 0x02 : 0) | ((blink) ? 0x01 : 0))
#define INS_SHIFT(target, direction) (0x10 | ((target) ? 0x08 : 0) | ((direction) ? 0x04 : 0))
#define INS_FUNCTION(interface, lines, font) (0x20 | ((interface) ? 0x10 : 0) | ((lines) ? 0x08 : 0) | ((font) ? 0x04 : 0))
#define INS_CGRAM(address) (0x40 | ((address) & 0x3f))
#define INS_DDRAM(address) (0x80 | ((address) & 0x7f))

// these can be fine tuned to suit specific control modules
#define STARTUP_DELAY 40 // ms
#define SHORT_INS_DELAY 37 // us
#define LONG_INS_DELAY 1.6 // ms
#define ENABLE_PULSE_WIDTH 0.5 // us

bool useDisplay = true;
bool useCursor = true;
bool useBlink = true;
bool useIncrement = true;
bool useShift = false;

static uint8_t lcdPulseEnable(bool readback)
{
	uint8_t input;

	E_HIGH();
	_delay_us(ENABLE_PULSE_WIDTH);
	input = readback ? DATAPORT_IN : 0;
	E_LOW();

	return input;
}

static void lcdWriteByte(uint8_t data, bool registerSelect)
{
	uint8_t output = 0;

	output |= ((data & 0x01) != 0) << LCD_PIN_DB0;
	output |= ((data & 0x02) != 0) << LCD_PIN_DB1;
	output |= ((data & 0x04) != 0) << LCD_PIN_DB2;
	output |= ((data & 0x08) != 0) << LCD_PIN_DB3;

#ifdef LCD_8BIT_INTERFACE
	output |= ((data & 0x10) != 0) << LCD_PIN_DB4;
	output |= ((data & 0x20) != 0) << LCD_PIN_DB5;
	output |= ((data & 0x40) != 0) << LCD_PIN_DB6;
	output |= ((data & 0x80) != 0) << LCD_PIN_DB7;
#endif

	registerSelect ? RS_HIGH() : RS_LOW();
	DATAPORT_OUT = (DATAPORT_OUT &= ~DATAPORT_BITS) | (output & DATAPORT_BITS);
	lcdPulseEnable(false);
}

static void lcdWrite(uint8_t data, bool registerSelect)
{
#ifdef LCD_8BIT_INTERFACE
	lcdWriteByte(data, registerSelect);
#else
	lcdWriteByte(data >> 4, registerSelect);
	lcdWriteByte(data, registerSelect);
#endif
}

#ifdef LCD_USE_BUSYFLAG
static uint8_t lcdRead(bool registerSelect)
{
	TURN_OFF(DATAPORT_BITS, DATAPORT_DIR);
	RW_HIGH();
	registerSelect ? RS_HIGH() : RS_LOW();

	uint8_t input = 0;

#ifdef LCD_8BIT_INTERFACE
	uint8_t data = lcdPulseEnable(true);
	input |= ((data & (1 << LCD_PIN_DB0)) != 0);
	input |= ((data & (1 << LCD_PIN_DB1)) != 0) << 1;
	input |= ((data & (1 << LCD_PIN_DB2)) != 0) << 2;
	input |= ((data & (1 << LCD_PIN_DB3)) != 0) << 3;
	input |= ((data & (1 << LCD_PIN_DB4)) != 0) << 4;
	input |= ((data & (1 << LCD_PIN_DB5)) != 0) << 5;
	input |= ((data & (1 << LCD_PIN_DB6)) != 0) << 6;
	input |= ((data & (1 << LCD_PIN_DB7)) != 0) << 7;
#else
	uint8_t data = lcdPulseEnable(true);
	input |= ((data & (1 << LCD_PIN_DB0)) != 0);
	input |= ((data & (1 << LCD_PIN_DB1)) != 0) << 1;
	input |= ((data & (1 << LCD_PIN_DB2)) != 0) << 2;
	input |= ((data & (1 << LCD_PIN_DB3)) != 0) << 3;
	input <<= 4;
	data = lcdPulseEnable(true);
	input |= ((data & (1 << LCD_PIN_DB0)) != 0);
	input |= ((data & (1 << LCD_PIN_DB1)) != 0) << 1;
	input |= ((data & (1 << LCD_PIN_DB2)) != 0) << 2;
	input |= ((data & (1 << LCD_PIN_DB3)) != 0) << 3;
#endif

	RW_LOW();
	TURN_ON(DATAPORT_BITS, DATAPORT_DIR);

	return input;
}
#endif

static void lcdWait()
{
#ifdef LCD_USE_BUSYFLAG
	while(lcdRead(false) & 0x80);
#else
	_delay_us(SHORT_INS_DELAY);
#endif
}

void lcdInitialize()
{
	TURN_ON(DATAPORT_BITS, DATAPORT_DIR);
	TURN_OFF(DATAPORT_BITS, DATAPORT_OUT);
	TURN_ON(CONTROLPORT_BITS, CONTROLPORT_DIR);
	TURN_OFF(CONTROLPORT_BITS, CONTROLPORT_OUT);

	_delay_ms(STARTUP_DELAY);

#ifdef LCD_8BIT_INTERFACE
	lcdWrite(INS_FUNCTION(1, 1, 0), false);
	_delay_us(SHORT_INS_DELAY);
	lcdWrite(INS_FUNCTION(1, 1, 0), false);
	_delay_us(SHORT_INS_DELAY);
#else
	lcdWriteByte(INS_FUNCTION(1, 0, 0) >> 4, false);
	_delay_us(SHORT_INS_DELAY);
	lcdWrite(INS_FUNCTION(0, 1, 0), false);
	_delay_us(SHORT_INS_DELAY);
	lcdWrite(INS_FUNCTION(0, 1, 0), false);
	_delay_us(SHORT_INS_DELAY);
#endif

	lcdWrite(INS_DISPLAY(1, 1, 1), false);
	lcdWait();
	lcdClearDisplay();
	lcdWrite(INS_ENTRY(1, 0), false);
	lcdWait();

	uint8_t bitmap[] = { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f };

	for(uint8_t i = 0; i < 8; ++i)
		lcdWriteBitmap(i, bitmap);
}

void lcdWriteCharacter(uint8_t character)
{
	lcdWrite(character, true);
	lcdWait();
}

void lcdWriteString(char* string)
{
	while(*string)
		lcdWriteCharacter((uint8_t)*string++);
}

void lcdWriteBitmap(uint8_t location, uint8_t* data)
{
	if(location > 8)
		return;

	lcdWrite(INS_CGRAM(location * 8), false);
	lcdWait();

	for(uint8_t i = 0; i < 8; ++i)
	{
		lcdWrite(data[i] & 0x1f, true);
		lcdWait();
	}

	lcdWrite(INS_DDRAM(0), false);
	lcdWait();
}

void lcdClearDisplay()
{
	lcdWrite(INS_CLEAR_DISPLAY, false);

#ifdef LCD_USE_BUSYFLAG
	lcdWait();
#else
	_delay_ms(LONG_INS_DELAY);
#endif
}

void lcdShift(bool cursorOrDisplay, bool leftOrRight)
{
	lcdWrite(INS_SHIFT(cursorOrDisplay, leftOrRight), false);
	lcdWait();
}

void lcdSetPosition(uint8_t column, uint8_t row)
{
	lcdWrite(INS_DDRAM((row * 0x40 + column)), false);
	lcdWait();
}

static void lcdApplyChanges()
{
	lcdWrite(INS_DISPLAY(useDisplay, useCursor, useBlink), false);
	lcdWait();
	lcdWrite(INS_ENTRY(useIncrement, useShift), false);
	lcdWait();
}

void lcdSetDisplay(bool state)
{
	useDisplay = state;
	lcdApplyChanges();
}

void lcdSetCursor(bool state)
{
	useCursor = state;
	lcdApplyChanges();
}

void lcdSetBlink(bool state)
{
	useBlink = state;
	lcdApplyChanges();
}

void lcdSetIncrement(bool state)
{
	useIncrement = state;
	lcdApplyChanges();
}

void lcdSetShift(bool state)
{
	useShift = state;
	lcdApplyChanges();
}
