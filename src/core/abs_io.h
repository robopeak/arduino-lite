/*
 *  ArduinoLite 
 *  A lightweight runtime system for AVR chips based on arduino project
 *  by Shikai Chen (csk@live.com, http://www.csksoft.net)
 *
 *  abs_io.h  abstract In/Out layer
 *
 */

#ifndef _ABSTRACT_IO_LAYER_H
#define _ABSTRACT_IO_LAYER_H

#include <inttypes.h>

//
typedef int (*getc_func)(void);
typedef void (*putc_func)( uint8_t);

typedef unsigned int (*read_func)(uint8_t *, unsigned int);
typedef void (*write_func)(uint8_t *, unsigned int);

typedef unsigned int (*gets_func)(uint8_t *);
typedef void (*puts_func)(uint8_t *);

#endif