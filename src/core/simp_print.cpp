/*
 *  ArduinoLite 
 *  A lightweight runtime system for AVR chips
 *  by Shikai Chen (csk@live.com, http://www.csksoft.net)
 *
 *  simp_print.c - simple and lightweight text serialize lib for arduino_lite
 *  Original file name : Print.cpp
 *
 *  ArduinoLite is based on the work of Arduino Project (arduino.cc) 
 */

/*
 Print.cpp - Base class that provides print() and println()
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
 
 Modified 23 November 2006 by David A. Mellis
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wiring.h"

#include "simp_print.h"

static void printNumber(unsigned long n, uint8_t base, puts_func func);
static void printFloat(double number, uint8_t digits, puts_func func);
void print(uint8_t b, putc_func func)
{
  func(b);
}

void print(char c, putc_func func)
{
  func(c);
}

void print(const char str[], puts_func func)
{
  func((uint8_t *)str);
}

void print(int n, puts_func func)
{
  print((long) n, func);
}

void print(unsigned int n, puts_func func)
{
  print((unsigned long) n, func);
}

void print(long n, puts_func func)
{
  if (n < 0) {
    print('-',func);
    n = -n;
  }
  printNumber(n, 10,func);
}

void print(unsigned long n, puts_func func)
{
  printNumber(n, 10, func);
}

void print(long n, int base, puts_func func)
{
  if (base == 0)
    print((char) n, func);
  else if (base == 10)
    print(n, func);
  else
    printNumber(n, base, func);
}

void print(double n, puts_func func)
{
  printFloat(n, 2, func);
}

void println(puts_func func)
{
  print("\r\n", func);
}

void println(char c, puts_func func)
{
  print(c, func);
  println(func);  
}

void println(const char c[], puts_func func)
{
  print(c, func);
  println(func);
}

void println(uint8_t b, puts_func func)
{
  print(b, func);
  println(func);
}

void println(int n, puts_func func)
{
  print(n, func);
  println(func);
}

void println(unsigned int n, puts_func func)
{
  print(n, func);
  println(func);
}

void println(long n, puts_func func)
{
  print(n, func);
  println(func);  
}

void println(unsigned long n, puts_func func)
{
  print(n, func);
  println(func);  
}

void println(long n, int base, puts_func func)
{
  print(n, base, func);
  println(func);
}

void println(double n, puts_func func)
{
  print(n, func);
  println(func);
}



void printNumber(unsigned long n, uint8_t base, puts_func func)
{
  unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 

  if (n == 0) {
    print("0", func);
    return;
  } 

  ultoa(n, (char *)buf, base);
  func(buf);
}

void printFloat(double number, uint8_t digits, puts_func func) 
{ 
#define MAX_FLOAT_DIGITS 16
  if (digits > MAX_FLOAT_DIGITS) return;
  unsigned char buf[32]; //
  dtostrf(number, 1, digits, (char *)buf);
  func(buf);
}
