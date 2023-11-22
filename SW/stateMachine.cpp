#include "HardwareConfig.h"
#include "stateMachine.h"
#include "ui.h"

void InitState::onExit(Ui& context) const
{
  context.readEEPROMData();
  context.initGenerator();
  Led_1::Low();
  Led_2::Low();
}

void OffState::onEntry(Ui& context) const
{
  context.setRPM0();
  Led_1::Low();
  Led_2::Low();
}
void OffState::onPush33(Ui& context) const
{
  context.setState(Run33State::getInstance());
}
void OffState::onPush45(Ui& context) const
{
  context.setState(Run45State::getInstance());
}

void RunState::onPushEncoder(Ui& context) const
{
  context.setState(ChangeFreqState::getInstance());
}
void RunState::onStopCounter(Ui& context) const
{
  context.setState(OffState::getInstance());
}

void Run33State::onEntry(Ui& context) const
{
  Mute::Low();
  context.setRPM33();
  Led_2::Low();
  Led_1::High();
  
}
void Run33State::onPush33(Ui& context) const
{
  context.setState(OffState::getInstance());
}
void Run33State::onPush45(Ui& context) const
{
  context.setState(Run45State::getInstance());
}

void Run45State::onEntry(Ui& context) const
{
  context.setRPM45();
  Led_1::Low();
  Led_2::High();
  Mute::Low();
}
void Run45State::onPush33(Ui& context) const
{
  context.setState(Run33State::getInstance());
}
void Run45State::onPush45(Ui& context) const
{
  context.setState(OffState::getInstance());
}

void ChangeFreqState::onEntry(Ui& context) const
{
  Led_3::High();
}
void ChangeFreqState::onExit(Ui& context) const
{
  Led_3::Low();
}
void ChangeFreqState::onPushEncoder(Ui& context) const
{
  context.setState(ChangePhaseState::getInstance());
}
void ChangeFreqState::onIncrement(Ui& context, int8_t incr) const
{
  context.incrRPM(incr);
}

void ChangePhaseState::onEntry(Ui& context) const
{
  Led_4::High();
}
void ChangePhaseState::onExit(Ui& context) const
{
  context.writeEEPROMData();
  Led_4::Low();
}
void ChangePhaseState::onPushEncoder(Ui& context) const
{
  if(context.isSetRPM33()) context.setState(Run33State::getInstance());
  else if(context.isSetRPM45()) context.setState(Run45State::getInstance());
  else context.setState(OffState::getInstance());
}
void ChangePhaseState::onIncrement(Ui& context, int8_t incr) const
{
  context.incrPhase(incr);
}
 