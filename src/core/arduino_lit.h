/*
 *  ArduinoLite 
 *  A lightweight and efficient runtime system for AVR chips
 *  by Shikai Chen (csk@live.com, http://www.csksoft.net)
 *
 *  arduino_lit.h  the ArduinoLite's main header file
 *  Original file name : WProgram.h
 *
 *  ArduinoLite is based on the work of Arduino Project (arduino.cc) 
 *
 */

#ifndef WProgram_h
#define WProgram_h

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <avr/interrupt.h>
#include <util/delay.h>
#include "wiring.h"
#include "simp_print.h"
#include "net_serial.h"

static inline uint16_t makeWord(uint16_t w)
{
	return w;
}
static inline uint16_t makeWord(byte h, byte l)
{
	return (((uint16_t)h)<<8 | l);
}

#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);


// WMath prototype
// ArduinoLite: The random/randomSeed funtions have been removed from ArduinoLite lib. 
//              Please use rand() in libc.
#if 0
long random(long);
long random(long, long);
void randomSeed(unsigned int);
#endif
extern "C" long map(long, long, long, long, long);


// Helper Functions


#define PRINT(x, ...) print(x, ##__VA_ARGS__, serial_puts)

#if defined(_DEBUG)
#define DBG_PRINT(x, ...) print(x, ##__VA_ARGS__, serial_puts)
#else
#define DBG_PRINT(x, ...) 
#endif

#endif
