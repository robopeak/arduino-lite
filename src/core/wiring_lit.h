/*
 *  ArduinoLite 
 *  A lightweight runtime system for AVR chips
 *  by Shikai Chen (csk@live.com, http://www.csksoft.net)
 *
 *  wiring_lit.h - a very fast wiring lib implemention
 *
 *
 *  ArduinoLite is based on the work of Arduino Project (arduino.cc) 
 *
 *  Support AVR Chips
 *  Atmega8
 *  Atmegax8 series: Atmega48 Atmega88 Atmega168 ...
 *  Attiny2313
 *  Attiny26
 *  Atmega1280 (not vaildated yet)
 */

#include "pins_arduino.h"
#include "pins_arduino_lit.h"
#ifndef _WIRING_LIT_H
#define _WIRING_LIT_H

#define MERGE_TO_FUNC(prefix, id)   prefix##_##id
#define EXPAND_WRAPPER( NEXTLEVEL, ...)  NEXTLEVEL( __VA_ARGS__ )



#define _PWM_SET(id, val)   \
    do{                     \
        OCR##id = val;      \
    }                       \
    while(0)            

#define _PWM_ENABLE(TCCR, id)  sbi(TCCR, COM##id##1)
#define _PWM_DISABLE(TCCR, id) cbi(TCCR, COM##id##1)

#define _SET_OUTPUT(port_id, msk)  PORTID_TO_DIR_REG(port_id) |= (msk)
#define _SET_INTPUT(port_id, msk)  PORTID_TO_DIR_REG(port_id) &= ~(msk)

#define _D_WRITE_HIGH(port_id, msk) PORTID_TO_OUTPUT_REG(port_id) |= (msk)
#define _D_WRITE_LOW(port_id, msk)  PORTID_TO_OUTPUT_REG(port_id) &= ~(msk)

#define _D_READ(port_id, msk)       ((PORTID_TO_INPUT_REG(port_id)) & (msk))
/*
 *
 *  NOTICE: for pins at timer0A/0B, 
 *          if the duty cycle to be set equals to zero, using the following code:
 *            DIGITAL_WRITE(pin, LOW);
 *             -- or --
 *            digitalWrite(pin, LOW);
 *  The caller should make sure the current pin has been set to OUTPUT mode first
 */

#define SET_1(pin)              SET_OUTPUT(pin)
#define SET_0(pin)              SET_INPUT(pin)

#define SET_0x1(pin)            SET_OUTPUT(pin)
#define SET_0x0(pin)            SET_INPUT(pin)

#define D_WRITE_HIGH(pin)       EXPAND_WRAPPER(_D_WRITE_HIGH, ARDUINOPIN_TO_PORTID(pin), ARDUINOPIN_TO_PORTMSK(pin) )
#define D_WRITE_LOW(pin)        EXPAND_WRAPPER(_D_WRITE_LOW, ARDUINOPIN_TO_PORTID(pin), ARDUINOPIN_TO_PORTMSK(pin) )

#define D_WRITE_1(pin)          D_WRITE_HIGH(pin) 
#define D_WRITE_0(pin)          D_WRITE_LOW(pin)

#define D_WRITE_0x1(pin)        D_WRITE_HIGH(pin) 
#define D_WRITE_0x0(pin)        D_WRITE_LOW(pin)


#define PWM_ENABLE(pin)         EXPAND_WRAPPER(_PWM_ENABLE ,ARDUINOPIN_TO_TCCRID(pin) , ARDUINOPIN_TO_TIMERID(pin) )
#define PWM_DISABLE(pin)        EXPAND_WRAPPER(_PWM_DISABLE ,ARDUINOPIN_TO_TCCRID(pin) , ARDUINOPIN_TO_TIMERID(pin) ) 
#define PWM_SET(pin, val)       EXPAND_WRAPPER(_PWM_SET, ARDUINOPIN_TO_TIMERID(pin), val )

#define SET_OUTPUT(pin)         EXPAND_WRAPPER(_SET_OUTPUT, ARDUINOPIN_TO_PORTID(pin), ARDUINOPIN_TO_PORTMSK(pin) )
#define SET_INPUT(pin)          EXPAND_WRAPPER(_SET_INTPUT, ARDUINOPIN_TO_PORTID(pin), ARDUINOPIN_TO_PORTMSK(pin) )

#define DIGITAL_READ(pin)       EXPAND_WRAPPER(_D_READ, ARDUINOPIN_TO_PORTID(pin), ARDUINOPIN_TO_PORTMSK(pin) )
#define DIGITAL_WRITE(pin, val) D_WRITE_##val(pin)
#define PIN_MODE(pin, mode)     SET_##mode(pin)
#define ANALOG_WRITE( pin, val )  \
    do{                          \
         PWM_ENABLE(pin);        \
         PWM_SET(pin,val);       \
    }                            \
    while(0)                   
    
#define PWM_SET_SAFE(pin, val)  \
    do{                         \
        if (val) {PIN_MODE(pin,OUTPUT);PWM_SET(pin, val);} \
        else PIN_MODE(pin, INPUT); \
    }while(0)

#endif
