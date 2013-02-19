/**
	Copyright (c) 2008 Mikko Ronkainen
	This software is licensed under the MIT License.
	
	@file util.h
	@brief General helper functions and definitions
	
	Compiler:	GCC-AVR
	Target:		Atmel AVR devices
	ChangeLog:	22.3.08 Initial version
*/

#ifndef UTIL_H
#define UTIL_H

#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)

#define TURN_ON(mask, port) ((port) |= (mask))
#define TURN_OFF(mask, port) ((port) &= ~(mask))

#endif
