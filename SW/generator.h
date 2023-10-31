
#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "sineTable.h"

static const uint32_t Step = 36231l;
static const uint32_t Degree_90 = 0x40000000;
static const uint32_t OneDegree = Degree_90 / 90l;
static const int8_t AddressShift = 24;

struct GeneratorData
{
  uint32_t m_FrequencySetting;
  uint32_t m_PhaseSetting;
  uint32_t m_PhaseStep;
  uint32_t m_PhaseOffset;
};

class Generator
{
public:
  void setGeneratorData(GeneratorData* data) 
  { 
    m_Data = data; 
  }

  GeneratorData* getGeneratorData() { return m_Data; }

  void Reset()
  {
    m_PhaseAccumulator = 0;
  }
  
  // --- ISR context --- start
  void calcNewPhase()
  {
    m_PhaseAccumulator += m_Data->m_FrequencySetting;// m_PhaseStep;
  }
  uint8_t getValueA()
  {
    uint8_t address_pointer = m_PhaseAccumulator >> AddressShift;
    return pgm_read_byte(&SINE_256V_8B[address_pointer]);
  }
  uint8_t getValueB()
  {  
    uint32_t accuB = m_PhaseAccumulator + m_Data->m_PhaseSetting; //m_PhaseOffsetChB;
    uint8_t address_pointer = accuB >> AddressShift;
    return pgm_read_byte(&SINE_256V_8B[address_pointer]);
  }
  // --- ISR context end ---

  static uint32_t getFreqSetting(uint32_t freq_100times) { return freq_100times * get1Hz() / 100l; }
  static uint32_t get1Hz() { return 0xFFFFFFFF / Step; }
  static uint32_t getCentiRPM() { return  get1Hz() / (6000 / 66); }

private:
  uint32_t m_PhaseStep = 50l * (0xFFFFFFFF/Step); //118544 => 1Hz
  uint32_t m_PhaseAccumulator = 0x00000000;
  uint32_t m_PhaseOffsetChB = 0x40000000; // for 90 degrees;
  GeneratorData* m_Data;
};

extern Generator generator;

#endif