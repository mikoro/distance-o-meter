/**
	Copyright (c) 2008 Mikko Ronkainen
	This software is licensed under the MIT License.
	
	@file lcd.h
	@brief HD44780-compatible character lcd control library
	
	Compiler:	GCC-AVR
	Target:		Atmel AVR devices
	ChangeLog:	22.3.08 Initial version
				23.3.08 Support for selecting arbitrary pins
*/

#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>

//#define LCD_8BIT_INTERFACE ///< Undefine to use 4-bit mode
//#define LCD_USE_BUSYFLAG ///< Undefine to use predefined delays (BUG: predefined delays will not work with -Os!)

#define LCD_DATAPORT C ///< Select dataport
#define LCD_PIN_DB0 3 ///< Assign pins, don't have to be in order, 4-bit mode uses DB0-DB3
#define LCD_PIN_DB1 2
#define LCD_PIN_DB2 1
#define LCD_PIN_DB3 0
//#define LCD_PIN_DB4 3
//#define LCD_PIN_DB5 2
//#define LCD_PIN_DB6 1
//#define LCD_PIN_DB7 0

#define LCD_CONTROLPORT C ///< Select control port
#define LCD_PIN_RS 6 ///< Assign pins
#define LCD_PIN_RW 5
#define LCD_PIN_E 4

/// @brief Initialize the display
/// @details Must be called before doing anything else
void lcdInitialize();

/// @brief Write a character to current cursor position
/// @details Cursor either moves forward, backward or stays still depending on the modes
/// @param character Character code as unsigned byte (check display datasheet for actual codes)
void lcdWriteCharacter(uint8_t character);

/// @brief Write string of characters to current cursor position
/// @details Cursor either moves forward, backward or stays still depending on the modes
/// @param string Character codes
void lcdWriteString(char* string);

/// @brief Write a bitmap to display memory
/// @details Returns cursor to home. Fails silently if location is out of range
/// @param location The memory slot where the bitmap is written to, must be in range of [0, 8]
/// @param data An array of eight bytes. Each byte represents a row and only first five bits are used (one character is 5x8 pixels)
void lcdWriteBitmap(uint8_t location, uint8_t* data);

/// @brief Clear the display and return cursor to home
void lcdClearDisplay();

/// @brief Either shift cursor or display left or right
/// @param cursorOrDisplay False for cursor, true for display
/// @param leftOrRight False for left, true for right
void lcdShift(bool cursorOrDisplay, bool leftOrRight);

/// @brief Set the position of the cursor
/// @details Positions out of real physical bounds can also be set
/// @param column The new column
/// @param row The new row
void lcdSetPosition(uint8_t column, uint8_t row);

/// @brief Set the display on or off
void lcdSetDisplay(bool state);

/// @brief Set the cursor on or off
void lcdSetCursor(bool state);

/// @brief Set the cursor blinking on or off
void lcdSetBlink(bool state);

/// @brief Set the increment direction
/// @param state True for right, false for left
void lcdSetIncrement(bool state);

/// @brief Set the display shift on or off
void lcdSetShift(bool state);

#endif
