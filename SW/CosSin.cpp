/*
 * CosSin.cpp
 *
 *  Created on: 10.01.2023
 *      Author: cybaer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "avrlib/base.h"
#include "avrlib/time.h"
#include "avrlib/gpio.h"
#include "avrlib/adc.h"

#include "generator.h"
#include "HardwareConfig.h"
#include "ui.h"

using namespace avrlib;

// __Compiler Bug__
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
void __cxa_pure_virtual() {};

volatile uint32_t offset = 0;

typedef Gpio<PortB, 3> PIN_PWM_TIMER2A;
//const uint8_t PIN_PWM_TIMER2A = 11; // PWM output OC2A
typedef Gpio<PortD, 3> PIN_PWM_TIMER2B;
//const uint8_t PIN_PWM_TIMER2B = 3;  // PWM output OC2B

volatile uint8_t count = 0;
volatile bool poll = false; 

Generator generator;
Ui ui; 

/* Timer1 interrupt calculates the pointer, gets the value
   for the PWM from the table and outputs it */
ISR(TIMER1_COMPA_vect)
{  
  //Led_4::High(); 
  generator.calcNewPhase();
  OCR2A = generator.getValueA();
  OCR2B = generator.getValueB();

  // ca. 1KHz
  if((count++ & 0x1f) == 0x10) poll = true;
  //Led_4::Low();
}

int main(void)
{
  PIN_PWM_TIMER2A::set_mode(DIGITAL_OUTPUT); 
  PIN_PWM_TIMER2B::set_mode(DIGITAL_OUTPUT); 
  
  cli();         // stop interrupts
  /* Set Timer2 register to FastPWM */
  TCCR2A = 0xA3;
  TCCR2B = 0x01; // no prescaling (f_PWM = 20MHz/256 = 78,125kHz)
  /* Set Timer1 for Timer interrupt */
  TCCR1A = 0;  
  TCCR1B = 0x0A; // turn on CTC mode prescaler = 2
  OCR1A = 68;   // f = 2,5MHz / (OCCR1A + 1) = 36231Hz
  TIMSK1 = 0x02; // enable timer compare interrupt
  sei();         //allow interrupts

  initHW();

  _delay_ms(100);

  while(1)
  {
    if(poll)
    {
      //Led_4::set_value(Mute::value());
      poll = false;
      ui.doEvents();
    }                      
  }
}
