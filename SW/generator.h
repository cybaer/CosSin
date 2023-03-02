
#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "sineTable.h"

static const uint32_t Step = 36231l;
static const uint32_t Degree_90 = 0x40000000;
static const uint32_t OneDegree = Degree_90 / 90l;

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
  void OnTick()  // for ramp up/down
  {}
  void xcrementFrequency()
  {}
  void xcrementPhase()
  {}
  void setGeneratorData(GeneratorData* data) 
  { 
    m_Data = data; 
    m_PhaseAccumulator = 0; /// ToDo: morph to target freq
  }

  // --- ISR context --- start
  void calcNewPhase()
  {
    m_PhaseAccumulator += m_Data->m_FrequencySetting;// m_PhaseStep;
  }
  uint8_t getValueA()
  {
    uint8_t address_pointer = m_PhaseAccumulator >> 24;
    return pgm_read_byte(&SINE_256V_8B[address_pointer]);
  }
  uint8_t getValueB()
  {
    uint32_t accuB = m_PhaseAccumulator + m_Data->m_PhaseSetting; //m_PhaseOffsetChB;
    uint8_t address_pointer = accuB >> 24;
    return pgm_read_byte(&SINE_256V_8B[address_pointer]);
  }
  // --- ISR context end ---
  static uint32_t getFreqSetting(uint32_t freq_100times) { return freq_100times * (0xFFFFFFFF/Step) / 100; }
  static uint32_t get1Hz() { return 0xFFFFFFFF/Step; }
private:
  uint32_t m_PhaseStep = 50l * (0xFFFFFFFF/Step); //118544 => 1Hz
  uint32_t m_PhaseAccumulator = 0x00000000;
  uint32_t m_PhaseOffsetChB = 0x40000000; // for 90 degrees;
     
  GeneratorData* m_Data;
};
extern Generator generator;

#endif