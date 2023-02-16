
#ifndef UI_H_
#define UI_H_

#include "HardwareConfig.h"
#include "stateMachine.h"
#include "generator.h"

class Ui : public Context
{
public:
  Ui()
  : Context(OffState::getInstance())
  {}

  void doEvents()
  {
    Button_1::Read();
    Button_2::Read();
    
    if(Button_1::lowered()) m_State->onPush33(*this);
    if(Button_2::lowered())  m_State->onPush45(*this);
    
   // auto incr = Encoder::Read();
   // if(Encoder::clicked()) m_State->onPushEncoder(*this);
   // if(incr != 0) m_State->onIncrement(*this, incr);

  }

};

extern Ui ui;

#endif