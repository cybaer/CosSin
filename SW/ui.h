
#ifndef UI_H_
#define UI_H_

#include <avr/eeprom.h>
#include "HardwareConfig.h"
#include "stateMachine.h"
#include "generator.h"

struct CosSinData
{
  GeneratorData Data[3];
};      

static const uint32_t StopFreq = Generator::getFreqSetting(20l);
static const uint32_t StopCount_33 = 1132UL * 60UL * 25UL;
static const uint32_t StopCount_45 = 1132UL * 60UL * 15UL;

class SmartGenerator
{
  static const int8_t RampTime = 5;
  static const int16_t TicksPerSecond = 36231 / 32;
  static const int16_t Steps = RampTime * TicksPerSecond; 

public:
  void setFrequency(GeneratorData* data)
  {
    m_TargetFrequency = data->m_FrequencySetting;
    m_PhaseSetting = data->m_PhaseSetting;

    m_Frequency = generator.getGeneratorData()->m_FrequencySetting;
    
    const int32_t deltaF = int32_t(m_TargetFrequency - m_Frequency);
    if(abs(deltaF) < generator.get1Hz())
    {
      m_FrequencyStep = deltaF;
    }
    else
    {    
      m_FrequencyStep = int32_t(m_TargetFrequency - m_Frequency) / Steps;
    }
  }

  bool onTick()
  {
    if(m_Frequency == m_TargetFrequency && m_Frequency == StopFreq)
    {
      if(m_Stop)
      {
        m_Stop = false;
        generator.Reset();
      }
      return true;
    }

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
        m_Stop = m_Frequency == StopFreq;
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
  uint32_t m_TargetFrequency = StopFreq;

  GeneratorData m_ActualData[2];
  uint8_t m_DataIdx = 0;
  bool m_Stop = true;
};

class Ui
{
public:
  Ui();

  void initGenerator()
  {
    generator.setGeneratorData(&m_Data[RPM_0]);
  }
  void readEEPROMData();
  void writeEEPROMData();

  void readEEPROMStopp();
  void writeEEPROMStopp();

  void doEvents();

  bool isSetRPM33() { return m_RPM == RPM_33; }
  bool isSetRPM45() { return m_RPM == RPM_45; }

  void setRPM0() { m_SmartGenerator.setFrequency(&m_Data[m_RPM = RPM_0]); }
  void setRPM33() 
  {
    m_StopCounter = StopCount_33; 
    m_SmartGenerator.setFrequency(m_ActualData = &m_Data[m_RPM = RPM_33]); 
  }
  void setRPM45() 
  { 
    m_StopCounter = StopCount_45;
    m_SmartGenerator.setFrequency(m_ActualData = &m_Data[m_RPM = RPM_45]); 
  }

  void incrRPM(int8_t incr)
  {
    uint32_t f = m_ActualData->m_FrequencySetting;
    f += incr * Generator::getCentiRPM();
    m_ActualData->m_FrequencySetting = f;
    m_SmartGenerator.setFrequency(m_ActualData);
  }
  void incrPhase(int8_t incr)
  {
    uint32_t Ph = m_ActualData->m_PhaseSetting;
    Ph += incr * OneDegree;
    m_ActualData->m_PhaseSetting = Ph;
    m_SmartGenerator.setFrequency(m_ActualData);
  }
  void setState(IState& state)
  {
    m_State->onExit(*this);
    m_State = &state;
    m_State->onEntry(*this);
  }
 
  bool toggleAutoStop() { return m_AutoStop = !m_AutoStop; }
  bool getAutoStop() { return m_AutoStop; }

private:
  enum RPM {RPM_0 = 0, RPM_33, RPM_45, RPM_Count};
  RPM m_RPM = RPM_0;
  
  GeneratorData m_Data[RPM_Count] = { {StopFreq,Degree_90, 0, 0}, 
                                    {Generator::getFreqSetting(3703l), Degree_90, 0, 0}, 
                                    {Generator::getFreqSetting(5000l), Degree_90 - 0*OneDegree, 0, 0} };

  GeneratorData* m_ActualData = &m_Data[0];
  IState* m_State; 

  SmartGenerator m_SmartGenerator;
  bool m_AutoStop = false;
  uint32_t m_StopCounter = 0;
};

extern Ui ui;
#endif