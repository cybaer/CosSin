

#include <avr/eeprom.h>
#include "HardwareConfig.h"
#include "stateMachine.h"
#include "ui.h"

CosSinData EEMEM eeData;
CosSinData m_RPMData; 

  Ui::Ui()
  : m_State(&InitState::getInstance())
  {
    //eeprom_write_block(m_Data, &eeData, sizeof(GeneratorData) * RPM_Count);
    setState(OffState::getInstance());
  }

  void Ui::readEEPROMData()
  {
    eeprom_read_block(m_Data, &eeData, sizeof(GeneratorData) * RPM_Count);
  }
  void Ui::writeEEPROMData()
  {
    eeprom_write_block(m_Data, &eeData, sizeof(GeneratorData) * RPM_Count);
  }

  void Ui::doEvents()
  {
    if(m_SmartGenerator.onTick()) Mute::High();

    Button_1::Read();
    Button_2::Read();
    
    if(Button_1::lowered()) m_State->onPush33(*this);
    if(Button_2::lowered())  m_State->onPush45(*this);
    
    auto incr = Encoder::Read();
    if(Encoder::clicked()) m_State->onPushEncoder(*this);
    if(incr != 0) m_State->onIncrement(*this, incr);
  }
