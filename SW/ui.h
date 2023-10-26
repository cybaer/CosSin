
#ifndef UI_H_
#define UI_H_

#include "HardwareConfig.h"
#include "stateMachine.h"
#include "generator.h"

class SmartGenerator
{
  static const int8_t RampTime = 5;
  static const int16_t TicksPerSecond = 36231 / 32;
  static const int16_t Steps = RampTime * TicksPerSecond; 
public:
  void setFrequency(GeneratorData& data)
  {
    m_TargetFrequency = data.m_FrequencySetting;
    m_PhaseSetting = data.m_PhaseSetting;
    
    m_Frequency = generator.getGeneratorData()->m_FrequencySetting;
    m_FrequencyStep = int32_t(m_TargetFrequency - m_Frequency) / Steps;
  }

  bool onTick()
  {
    if(m_Frequency == m_TargetFrequency) return m_Frequency == 0;

    if(m_Frequency != m_TargetFrequency)
    {
      const int32_t deltaF = int32_t(m_TargetFrequency - m_Frequency);
      if(abs(deltaF) > abs(m_FrequencyStep))
      {
        m_Frequency += m_FrequencyStep;
      }
      else
      {
        m_Frequency = m_TargetFrequency;
      }
      m_ActualData[m_DataIdx].m_FrequencySetting = m_Frequency;
      m_ActualData[m_DataIdx].m_PhaseSetting = m_PhaseSetting;
      generator.setGeneratorData(&m_ActualData[m_DataIdx]);
      m_DataIdx = (m_DataIdx + 1) & 0x01;
    }
    return false;
  }
private:
  int32_t m_FrequencyStep = 0;
  uint32_t m_Frequency = 0;
  uint32_t m_PhaseSetting = 0;
  uint32_t m_TargetFrequency;

  GeneratorData m_ActualData[2];
  uint8_t m_DataIdx = 0;
};

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
    if(m_SmartGenerator.onTick()) Mute::High();

    Led_4::set_value(Mute::is_low());
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

  void setRPM0() 
  { 
    m_SmartGenerator.setFrequency(m_Data[RPM_0]); 
  }
  void setRPM33() 
  { 
    m_SmartGenerator.setFrequency(m_Data[RPM_33]);
    //generator.setGeneratorData(m_ActualData); 
  }
  void setRPM45() 
  { 
    m_SmartGenerator.setFrequency(m_Data[RPM_45]);
    //generator.setGeneratorData(m_ActualData); 
  }

  void incrRPM(int8_t incr)
  {
    uint32_t f = m_ActualData->m_FrequencySetting;
    f += incr * Generator::getCentiRPM();
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

  SmartGenerator m_SmartGenerator;
};

extern Ui ui;

#endif