
#ifndef GENERATOR_H_
#define GENERATOR_H_

const uint32_t OneDegree = 0x80000000 / 180l;

class Generator
{
public:
  void OnTick()  // for ramp up/down
  {}
  void xcrementFrequency()
  {}
  void xcrementPhase()
  {}
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
private:
  uint32_t m_PhaseStep = 50l * (0xFFFFFFFF/36231l) -00; //118544 => 1Hz
  uint32_t m_PhaseAccumulator = 0x00000000;
  uint32_t m_PhaseOffsetChB = 0x80000000; // for 180 degrees;
     
};
extern Generator generator;

#endif