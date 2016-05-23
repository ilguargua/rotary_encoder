
//#include <EnableInterrupt.h>
#include <MsTimer2.h>

#include "encoder.h"

uint8_t encoder_pinA = 255;
uint8_t encoder_pinB = 255;
volatile uint8_t encoder_state = 0;
volatile int8_t encoder_value = 0;
//void encoder_ISR();

void encoder_ISR()
{
  uint8_t pinstate = (digitalRead( encoder_pinA ) << 1) | digitalRead( encoder_pinB );
  //uint8_t ret = digitalRead(13);
  uint8_t _state = 0;
  //enc_count = 0;
  
  // Determine new state from the pins and state table.
  //state = ttable[state & 0x07][pinstate];
  memcpy_P (&_state, &ttable[encoder_state & 0x07][pinstate], 1);  
  encoder_state = _state;
  if( encoder_state & DIR_CW )    encoder_value++;
  if( encoder_state & DIR_CCW )   encoder_value--;
  //digitalWrite(13, ! ret);
  
}


encoder::encoder(uint8_t a_pin, uint8_t b_pin)
{
  encoder_pinA = a_pin;
  encoder_pinB = b_pin;
  pinMode(a_pin, INPUT);
  pinMode(b_pin, INPUT);
  encoder_state = (digitalRead( encoder_pinA ) << 1) | digitalRead( encoder_pinB );
  MsTimer2::set(5,encoder_ISR);
  MsTimer2::start();
  //enableInterrupt(encoder_pinA,encoder_ISR,CHANGE);
  //enableInterrupt(encoder_pinB,encoder_ISR,CHANGE);
}

int8_t encoder::check()
{
  int8_t ret = encoder_value;
  encoder_value = 0;
  return ret;   
}