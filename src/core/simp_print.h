/*
 *  ArduinoLite 
 *  A lightweight runtime system for AVR chips
 *  by Shikai Chen (csk@live.com, http://www.csksoft.net)
 *
 *  simp_print.h - simple and lightweight text serialize lib for arduino_lite
 *  Original file name : Print.h
 *
 *  ArduinoLite is based on the work of Arduino Project (arduino.cc) 
 */
 
/*
  Print.h - Base class that provides print() and println()
  Copyright (c) 2008 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Print_h
#define Print_h

#include <inttypes.h>
#include <stdio.h> // for size_t
#include "abs_io.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0



void print(char, putc_func);
void print(const char[], puts_func);
void print(uint8_t, putc_func);
void print(int, puts_func);
void print(unsigned int, puts_func);
void print(long, puts_func);
void print(unsigned long, puts_func);
void print(long, int, puts_func);
void print(double, puts_func);
void println(puts_func);
void println(char, puts_func);
void println(const char[], puts_func);
void println(uint8_t, puts_func);
void println(int, puts_func);
void println(unsigned int, puts_func);
void println(long, puts_func);
void println(unsigned long, puts_func);
void println(long, int, puts_func);
void println(double, puts_func);


#endif
