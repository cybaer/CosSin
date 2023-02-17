
#ifndef UI_H_
#define UI_H_

#include "HardwareConfig.h"
#include "stateMachine.h"
#include "generator.h"

class Ui
{
public:
  Ui()
  : m_State(&InitState::getInstance())
  {
    setState(OffState::getInstance());
  }
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

  void setRPM0() { generator.setGeneratorData(&m_Data[RPM_0]); }
  void setRPM33() { generator.setGeneratorData(&m_Data[RPM_33]); }
  void setRPM45() { generator.setGeneratorData(&m_Data[RPM_45]); }

  void setState(IState& state)
  {
    m_State->onExit(*this);
    m_State = &state;
    m_State->onEntry(*this);
  }

 
private:
  enum RPM {RPM_0 = 0, RPM_33, RPM_45, RPM_Count};
  GeneratorData m_Data[RPM_Count] = { {0,Degree_180, 0, 0}, 
                                    {Generator::getFreqSetting(5000), Degree_180, 0, 0}, 
                                    {Generator::getFreqSetting((5000l*4500l)/3333l), Degree_180, 0, 0} };

  IState* m_State;
};

extern Ui ui;

#endif