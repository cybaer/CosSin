#ifndef HARDWARECONFIG_H_
#define HARDWARECONFIG_H_

#include <avrlib/base.h>
#include <avrlib/gpio.h>
#include <avrlib/devices/rotary_encoder.h>

// __Compiler Bug__
__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);
extern "C" void __cxa_pure_virtual();

using namespace avrlib;

typedef Inverter<Gpio<PortD, 1> > Mute;

typedef DebouncedSwitch< Gpio<PortD, 5> > Button_2;
typedef DebouncedSwitch< Gpio<PortD, 6> > Button_1;

typedef Gpio<PortB, 2> Encoder_A;
typedef Gpio<PortB, 0> Encoder_B;
typedef Gpio<PortB, 1> Encoder_S;
typedef RotaryEncoder<Encoder_A, Encoder_B, Encoder_S> Encoder;

typedef Gpio<PortC, 4> Led_1;
typedef Gpio<PortC, 5> Led_2;
typedef Gpio<PortC, 3> Led_3;
typedef Gpio<PortC, 2> Led_4;

inline void initOutputs()
{
  Mute::set_mode(DIGITAL_OUTPUT);
  Mute::High();
  Led_1::set_mode(DIGITAL_OUTPUT);
  Led_1::Low();
  Led_2::set_mode(DIGITAL_OUTPUT);
  Led_2::Low();
  Led_3::set_mode(DIGITAL_OUTPUT);
  Led_3::Low();
  Led_4::set_mode(DIGITAL_OUTPUT);
  Led_4::Low();
}

inline void initHW()
{
  initOutputs();

  Button_1::Init();
  Button_2::Init();
  Encoder::Init();
}

#endif