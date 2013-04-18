/*
 *  ArduinoLite 
 *  A lightweight runtime system for AVR chips
 *  by Shikai Chen (csk@live.com, http://www.csksoft.net)
 *
 *  net_serial.c - AVR serial library for arduino_lite
 *  Original file name : HardwareSerial.cpp
 *
 *  ArduinoLite is based on the work of Arduino Project (arduino.cc) 
 *
 */

/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

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

#if defined (__AVR_ATtiny26__)
//no serial port for Attiny26
#else

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h> //for abs
#include "wiring.h"
#include "wiring_private.h"

#include "net_serial.h"
#include "simp_print.h"

#ifdef _ENABLE_SERIAL_CALLBACK
volatile onByteProc_t g_onDataProc = NULL;
#endif

#if defined(__AVR_ATtiny2313__)
#define DEF_USART_DESC( _name_, _UBRRH_, _UBRRL_, _UCSRA_, _UCSRB_, _UDR_, _RXEN_, _TXEN_, _RXCIE_, _UDRE_, _U2X_ )
#else
#ifdef _ENABLE_MULTI_SERIAL_ 
#define DEF_USART_DESC( _name_, _UBRRH_, _UBRRL_, _UCSRA_, _UCSRB_, _UDR_, _RXEN_, _TXEN_, _RXCIE_, _UDRE_, _U2X_ ) \
    usart_desc_t  _name_ = { { { 0 }, 0, 0 },                                          \
                            (_UBRRH_), (_UBRRL_), (_UCSRA_), (_UCSRB_), (_UDR_),       \
                            (_RXEN_), (_TXEN_), (_RXCIE_), (_UDRE_), (_U2X_) }
#else
#define DEF_USART_DESC( _name_, _UBRRH_, _UBRRL_, _UCSRA_, _UCSRB_, _UDR_, _RXEN_, _TXEN_, _RXCIE_, _UDRE_, _U2X_ ) \
    usart_desc_t  _name_ = { { { 0 }, 0, 0 }}
#endif
#endif


#ifndef _SERIAL_CUSTOM_RX_ROUTINE_
// Preinstantiate Objects //////////////////////////////////////////////////////
DEF_USART_DESC(g_usart, &_UBRRH, &_UBRRL, &_UCSRA, &_UCSRB, &_UDR, _RXEN, _TXEN, _RXCIE, _UDRE, _U2X);
#endif

#if defined(__AVR_ATmega1280__) && defined(_ENABLE_MULTI_SERIAL_)
DEF_USART_DESC(g_usart1, &UBRR1H, &UBRR1L, &UCSR1A, &UCSR1B, &UDR1, RXEN1, TXEN1, RXCIE1, UDRE1, U2X1);
DEF_USART_DESC(g_usart2, &UBRR2H, &UBRR2L, &UCSR2A, &UCSR2B, &UDR2, RXEN2, TXEN2, RXCIE2, UDRE2, U2X2);
DEF_USART_DESC(g_usart3, &UBRR3H, &UBRR3L, &UCSR3A, &UCSR3B, &UDR3, RXEN3, TXEN3, RXCIE3, UDRE3, U2X3);
#endif

#if !defined(__AVR_ATtiny2313__) && !defined(_SERIAL_CUSTOM_RX_ROUTINE_)
inline void store_char(unsigned char c, ring_buffer_t *rx_buffer)
{
  unsigned char i = (rx_buffer->head + 1) % RX_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != rx_buffer->tail) {
    rx_buffer->buffer[rx_buffer->head] = c;
    rx_buffer->head = i;
  }
}

#endif


#ifndef _SERIAL_CUSTOM_RX_ROUTINE_

#if defined(__AVR_ATmega1280__)

ISR(SIG_USART0_RECV)
{
  unsigned char c = UDR0;
#ifdef _ENABLE_SERIAL_CALLBACK
  if (g_onDataProc && g_onDataProc(c)) return;
#endif
  store_char(c, &g_usart.rx_buffer);
}

#ifdef _ENABLE_MULTI_SERIAL_
ISR(SIG_USART1_RECV)
{
  unsigned char c = UDR1;
  store_char(c, &g_usart1.rx_buffer1);
}

ISR(SIG_USART2_RECV)
{
  unsigned char c = UDR2;
  store_char(c, &g_usart2.rx_buffer2);
}

ISR(SIG_USART3_RECV)
{
  unsigned char c = UDR3;
  store_char(c, &g_usart3.rx_buffer3);
}
#endif
#else

#if defined(__AVR_ATmega8__)
ISR(SIG_UART_RECV)
#else
ISR(USART_RX_vect)
#endif
{
  unsigned char c = _UDR;
#ifdef _ENABLE_SERIAL_CALLBACK
  if (g_onDataProc && g_onDataProc(c)) return;
#endif

#if !defined(__AVR_ATtiny2313__)
  store_char(c, &g_usart.rx_buffer);
#endif
}

#endif

#endif //end of _SERIAL_CUSTOM_RX_ROUTINE_

//init a serial port for communication
void serial_begin(unsigned long baud)
{
  uint16_t baud_setting;
  boolean use_u2x;

  // U2X mode is needed for baud rates higher than (CPU Hz / 16)
  if (baud > F_CPU / 16) {
    use_u2x = true;
  } else {
    // figure out if U2X mode would allow for a better connection
    
    // calculate the percent difference between the baud-rate specified and
    // the real baud rate for both U2X and non-U2X mode (0-255 error percent)
#if 0
    uint8_t nonu2x_baud_error = abs((int)(255-((F_CPU/(16*(((F_CPU/8/baud-1)/2)+1))*255)/baud)));
    uint8_t u2x_baud_error = abs((int)(255-((F_CPU/(8*(((F_CPU/4/baud-1)/2)+1))*255)/baud)));
#else
    uint8_t nonu2x_baud_error = abs((int) ((F_CPU/16)/((((F_CPU/8)/baud-1)/2)+1) - baud));
    uint8_t u2x_baud_error =    abs((int) ((F_CPU/8)/((((F_CPU/4)/baud-1)/2) +1) - baud));
#endif
    // prefer non-U2X mode because it handles clock skew better
    use_u2x = (nonu2x_baud_error > u2x_baud_error);
  }
  
  if (use_u2x) {
    _UCSRA = 1 << _U2X;
    baud_setting = (F_CPU / 4 / baud - 1) / 2;
  } else {
    _UCSRA = 0;
    baud_setting = (F_CPU / 8 / baud - 1) / 2;
  }

  // assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
  _UBRRH = baud_setting >> 8;
  _UBRRL = baud_setting;

  sbi(_UCSRB, _RXEN);
  sbi(_UCSRB, _TXEN);
  sbi(_UCSRB, _RXCIE);
}


#if !defined(__AVR_ATtiny2313__) && !defined(_SERIAL_CUSTOM_RX_ROUTINE_)
uint8_t serial_available(void)
{
  return (RX_BUFFER_SIZE + g_usart.rx_buffer.head - g_usart.rx_buffer.tail) % RX_BUFFER_SIZE;
}

int serial_getc(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (g_usart.rx_buffer.head == g_usart.rx_buffer.tail) {
    return -1;
  } else {
    unsigned char c = g_usart.rx_buffer.buffer[g_usart.rx_buffer.tail];
    g_usart.rx_buffer.tail = (g_usart.rx_buffer.tail + 1) % RX_BUFFER_SIZE;
    return c;
  }
}

void serial_flush(void)
{
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // were full, not empty.
    g_usart.rx_buffer.head = g_usart.rx_buffer.tail;
}

#endif

void serial_putc(uint8_t c)
{
  
  loop_until_bit_is_set(_UCSRA, _UDRE);
  _UDR = c;
}

void serial_puts(uint8_t *buf, size_t size)
{
   while(size--)
   {
      serial_putc(*(buf++));
   }
}


void serial_puts(uint8_t *buf)
{
   while(*buf)
   {
       serial_putc(*(buf++));
   }
}


#if defined(_ENABLE_MULTI_SERIAL_) && !defined(__AVR_ATtiny2313__)
//init a serial port for communication
void serial_begin_at(usart_desc_t * desc, unsigned long baud)
{
  uint16_t baud_setting;
  boolean use_u2x;

  // U2X mode is needed for baud rates higher than (CPU Hz / 16)
  if (baud > F_CPU / 16) {
    use_u2x = true;
  } else {
    // figure out if U2X mode would allow for a better connection
    
    // calculate the percent difference between the baud-rate specified and
    // the real baud rate for both U2X and non-U2X mode (0-255 error percent)
#if 0
    uint8_t nonu2x_baud_error = abs((int)(255-((F_CPU/(16*(((F_CPU/8/baud-1)/2)+1))*255)/baud)));
    uint8_t u2x_baud_error = abs((int)(255-((F_CPU/(8*(((F_CPU/4/baud-1)/2)+1))*255)/baud)));
#else
    uint8_t nonu2x_baud_error = abs((int) ((F_CPU/16)/((((F_CPU/8)/baud-1)/2)+1) - baud));
    uint8_t u2x_baud_error =    abs((int) ((F_CPU/8)/((((F_CPU/4)/baud-1)/2) +1) - baud));
#endif
    
    // prefer non-U2X mode because it handles clock skew better
    use_u2x = (nonu2x_baud_error > u2x_baud_error);
  }
  
  if (use_u2x) {
    *(desc->ucsra) = 1 << desc->u2x;
    baud_setting = (F_CPU / 4 / baud - 1) / 2;
  } else {
    *(desc->ucsra) = 0;
    baud_setting = (F_CPU / 8 / baud - 1) / 2;
  }

  // assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
  *(desc->ubrrh) = baud_setting >> 8;
  *(desc->ubrrl) = baud_setting;

  sbi(*(desc->ucsrb), desc->rxen);
  sbi(*(desc->ucsrb), desc->txen);
  sbi(*(desc->ucsrb), desc->rxcie);
}


uint8_t serial_available_at(usart_desc_t * desc)
{
  return (RX_BUFFER_SIZE + desc->rx_buffer.head - desc->rx_buffer.tail) % RX_BUFFER_SIZE;
}

int serial_getc_at(usart_desc_t * desc)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (desc->rx_buffer.head == desc->rx_buffer.tail) {
    return -1;
  } else {
    unsigned char c = desc->rx_buffer.buffer[desc->rx_buffer.tail];
    desc->rx_buffer.tail = (desc->rx_buffer.tail + 1) % RX_BUFFER_SIZE;
    return c;
  }
}

void serial_flush_at(usart_desc_t * desc)
{
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // were full, not empty.
    desc->rx_buffer.head = desc->rx_buffer.tail;
}

void serial_putc_at(usart_desc_t * desc, uint8_t c)
{
  while (!((*(desc->ucsra)) & (1 << desc->udre)))
    ;

  *(desc->udr) = c;
}

void serial_puts_at(usart_desc_t * desc,uint8_t *buf, size_t size)
{
   while(size--)
   {
      while (!((*(desc->ucsra)) & (1 << desc->udre)));
      *(desc->udr) = *(buf++);
   }
}

void serial_puts_at(usart_desc_t * desc,uint8_t *buf)
{

   while(*buf)
   {
       while (!((*(desc->ucsra)) & (1 << desc->udre)));
       *(desc->udr) = *(buf++);
   }
}

#endif

#endif


