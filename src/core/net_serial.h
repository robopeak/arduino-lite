/*
 *  ArduinoLite 
 *  A lightweight runtime system for AVR chips
 *  by Shikai Chen (csk@live.com, http://www.csksoft.net)
 *
 *  net_serial.h - AVR serial library for arduino_lite
 *  Original file name : HardwareSerial.h
 *
 *  ArduinoLite is based on the work of Arduino Project (arduino.cc) 
 *
 */
/*
  HardwareSerial.h - Hardware serial library for Wiring
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
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#if defined (__AVR_ATtiny26__)
//no serial port for Attiny26
#else

#include <inttypes.h>

#if defined(__AVR_ATmega8__)

#define _UBRRH UBRRH
#define _UBRRL UBRRL
#define _UCSRA UCSRA
#define _UCSRB UCSRB
#define _UDR   UDR
#define _RXEN  RXEN
#define _TXEN  TXEN
#define _RXCIE RXCIE
#define _UDRE  UDRE
#define _U2X   U2X

#elif defined(__AVR_ATtiny2313__)

#define _UBRRH UBRRH
#define _UBRRL UBRRL
#define _UCSRA UCSRA
#define _UCSRB UCSRB
#define _UDR   UDR
#define _RXEN  RXEN
#define _TXEN  TXEN
#define _RXCIE RXCIE
#define _UDRE  UDRE
#define _U2X   U2X
#else

#define _UBRRH UBRR0H
#define _UBRRL UBRR0L
#define _UCSRA UCSR0A
#define _UCSRB UCSR0B
#define _UDR   UDR0
#define _RXEN  RXEN0
#define _TXEN  TXEN0
#define _RXCIE RXCIE0
#define _UDRE  UDRE0
#define _U2X   U2X0
#endif


// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which rx_buffer_head is the index of the
// location to which to write the next incoming character and rx_buffer_tail
// is the index of the location from which to read.
#define RX_BUFFER_SIZE 128

typedef struct ring_buffer {
  unsigned char buffer[RX_BUFFER_SIZE];
  volatile unsigned char head;
  volatile unsigned char tail;
} ring_buffer_t;

typedef struct usart_desc
{
// TODO: change to const value if the compiler allows
#if !defined(__AVR_ATtiny13__) //no ring buffer for tiny2313
    ring_buffer_t   rx_buffer;
#endif

#if defined(_ENABLE_MULTI_SERIAL_) && !defined(__AVR_ATtiny2313__)
    volatile uint8_t *ubrrh;
    volatile uint8_t *ubrrl;
    volatile uint8_t *ucsra;
    volatile uint8_t *ucsrb;
    volatile uint8_t *udr;
    uint8_t rxen;
    uint8_t txen;
    uint8_t rxcie;
    uint8_t udre;
    uint8_t u2x;
#endif
} usart_desc_t;



#if !defined(__AVR_ATtiny2313__)  && !defined(_SERIAL_CUSTOM_RX_ROUTINE_)
//no ring buffer for tiny2313 or the macro _SERIAL_CUSTOM_RX_ROUTINE_ is defined
extern usart_desc_t g_usart;
#endif

#if defined(__AVR_ATmega1280__)
extern usart_desc_t g_usart1;
extern usart_desc_t g_usart2;
extern usart_desc_t g_usart3;
#endif


void serial_begin(unsigned long baud);

//require the macro BAUD to be defined 
inline static void SERIAL_BEGIN()
{
#ifndef BAUD
#define BAUD 19200 //set BAUD to 19200 as default
#endif
#include <util/setbaud.h>

_UBRRH = UBRRH_VALUE;
_UBRRL = UBRRL_VALUE;

#if USE_2X
    sbi(_UCSRA, _U2X);
#else
    cbi(_UCSRA, _U2X);
#endif

  sbi(_UCSRB, _RXEN);
  sbi(_UCSRB, _TXEN);
  sbi(_UCSRB, _RXCIE);
  
}

#if !defined(__AVR_ATtiny2313__) && !defined(_SERIAL_CUSTOM_RX_ROUTINE_)
////////////////
#ifdef _ENABLE_SERIAL_CALLBACK
typedef unsigned char (*onByteProc_t)(unsigned char);
extern volatile onByteProc_t g_onDataProc;
static inline void serial_SetDataProc(onByteProc_t proc)
{
    g_onDataProc = proc;
}
#endif
////////////////
uint8_t serial_available(void);
int serial_getc(void);
void serial_flush(void);
#endif


//////////////////////////////////////////////////////////////
// Tx routines operate on the default port( the first port )
void serial_putc(uint8_t c);
void serial_puts(uint8_t *buf, size_t size);
void serial_puts(uint8_t *buf);

//////////////////////////////
// Multiple Serial Support
#if defined(_ENABLE_MULTI_SERIAL_) && !defined(__AVR_ATtiny2313__)

void      serial_begin_at       (usart_desc_t * desc, unsigned long baud);
uint8_t   serial_available_at   (usart_desc_t * desc);
int       serial_readc_at       (usart_desc_t * desc);
void      serial_flush_at       (usart_desc_t * desc);
void      serial_putc_at        (usart_desc_t * desc, uint8_t c);
void      serial_puts_at        (usart_desc_t * desc,uint8_t *buf, size_t size);
void      serial_puts_at        (usart_desc_t * desc,uint8_t *buf);

#endif


#endif

#endif
