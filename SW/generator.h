
#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "sineTable.h"

static const uint32_t Degree_180 = 0x80000000;
static const uint32_t OneDegree = Degree_180 / 180l;

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
  void setGeneratorData(GeneratorData* data) { m_Data = data; }

  // --- ISR context --- start
  void calcNewPhase()
  {
    m_PhaseAccumulator += m_PhaseStep;
  }
  uint8_t getValueA()
  {
    uint8_t address_pointer = m_PhaseAccumulator >> 24;
    return pgm_read_byte(&SINE_256V_8B[address_pointer]);
  }
  uint8_t getValueB()
  {
    uint32_t accuB = m_PhaseAccumulator + m_PhaseOffsetChB;
    uint8_t address_pointer = accuB >> 24;
    return pgm_read_byte(&SINE_256V_8B[address_pointer]);
  }
  // --- ISR context end ---
  static uint32_t getFreqSetting(uint32_t freq_100times) { return freq_100times * (0xFFFFFFFF/36231l) / 100; }
private:
  uint32_t m_PhaseStep = 50l * (0xFFFFFFFF/36231l) -00; //118544 => 1Hz
  uint32_t m_PhaseAccumulator = 0x00000000;
  uint32_t m_PhaseOffsetChB = 0x80000000; // for 180 degrees;
     
  GeneratorData* m_Data;
};
extern Generator generator;

#endif