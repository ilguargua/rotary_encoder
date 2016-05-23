/*
 *      rotary_sm.ino   --  State machine implementation of rotary encoder driver
 *                          Interrupt driven; supports 2 types of interrupts:
 *                            * Polled interrupts; enable by defining TIMER2INT
 *                            * Pin change interrupts: enalbe by defining PINCHANGEINT
 *                              (Do NOT enable both at the same time)
 *
 *          This program is developed from the code at: 
 *              http://www.buxtronix.net/2011/10/rotary-encoders-done-properly.html
 *          Since this code was in the form of an arduino library, and I had unresolvable
 *          references that I could not figure out, I just modified it as a single sketch.
 *          The only library support required is if you want to use interrupt polling,
 *          and then the MsTimer2 library needs to be installed.
*/

#ifndef _ENCODER_H
#define _ENCODER_H

#if ARDUINO >= 100
 #include "Arduino.h"

#else
 #include "WProgram.h"
#endif


#include <avr/pgmspace.h>

// encoder setting
#define DIR_NONE 0x00           // No complete step yet.
#define DIR_CW   0x10           // Clockwise step.
#define DIR_CCW  0x20           // Anti-clockwise step.
// State definitions state table (emits a code at 00 only)
// states are: (NAB) N = 0: clockwise;  N = 1: counterclockwiswe
#define R_START     0x3
#define R_CW_BEGIN  0x1
#define R_CW_NEXT   0x0
#define R_CW_FINAL  0x2
#define R_CCW_BEGIN 0x6
#define R_CCW_NEXT  0x4
#define R_CCW_FINAL 0x5


/*
 * The below state table has, for each state (row), the new state
 * to set based on the next encoder output. From left to right in,
 * the table, the encoder outputs are 00, 01, 10, 11, and the value
 * in that position is the new state to set.
 */
const unsigned char ttable[8][4] PROGMEM = {
    {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},                // R_CW_NEXT
    {R_CW_NEXT,  R_CW_BEGIN,  R_CW_BEGIN,  R_START},                // R_CW_BEGIN
    {R_CW_NEXT,  R_CW_FINAL,  R_CW_FINAL,  R_START | DIR_CW},       // R_CW_FINAL
    {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},                // R_START
    {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},                // R_CCW_NEXT
    {R_CCW_NEXT, R_CCW_FINAL, R_CCW_FINAL, R_START | DIR_CCW},      // R_CCW_FINAL
    {R_CCW_NEXT, R_CCW_BEGIN, R_CCW_BEGIN, R_START},                // R_CCW_BEGIN
    {R_START,    R_START,     R_START,     R_START}                 // ILLEGAL
};


class encoder
{
public:
  encoder(uint8_t a_pin, uint8_t b_pin);
  int8_t check();
//private:
  //uint8_t _a, _b;
  //int8_t value;
  
};

#endif