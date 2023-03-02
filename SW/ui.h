
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
    
    auto incr = Encoder::Read();
    if(Encoder::clicked()) m_State->onPushEncoder(*this);
    if(incr != 0) m_State->onIncrement(*this, incr);

  }

  bool isSetRPM33() { return m_ActualData == &m_Data[RPM_33]; }
  bool isSetRPM45() { return m_ActualData == &m_Data[RPM_45]; }

  void setRPM0() { generator.setGeneratorData(&m_Data[RPM_0]); }
  void setRPM33() 
  { 
    m_ActualData = &m_Data[RPM_33];
    generator.setGeneratorData(m_ActualData); 
  }
  void setRPM45() 
  { 
    m_ActualData = &m_Data[RPM_45];
    generator.setGeneratorData(m_ActualData); 
  }

  void incrRPM(int8_t incr)
  {
    uint32_t f = m_ActualData->m_FrequencySetting;
    f += incr * Generator::get1Hz();
    m_ActualData->m_FrequencySetting = f;
  }
  void incrPhase(int8_t incr)
  {
    uint32_t Ph = m_ActualData->m_PhaseSetting;
    Ph += incr * OneDegree;
    m_ActualData->m_PhaseSetting = Ph;
  }
  void setState(IState& state)
  {
    m_State->onExit(*this);
    m_State = &state;
    m_State->onEntry(*this);
  }

 
private:
  enum RPM {RPM_0 = 0, RPM_33, RPM_45, RPM_Count};
  GeneratorData m_Data[RPM_Count] = { {0,Degree_90, 0, 0}, 
                                    {Generator::getFreqSetting(3703l), Degree_90, 0, 0}, 
                                    {Generator::getFreqSetting(5000l), Degree_90 - 0*OneDegree, 0, 0} };
                                    //{Generator::getFreqSetting((5000l*4500l)/3333l), Degree_90, 0, 0} };
  GeneratorData* m_ActualData = &m_Data[0];
  IState* m_State;
};

extern Ui ui;

#endif