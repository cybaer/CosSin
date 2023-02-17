#include "HardwareConfig.h"
#include "stateMachine.h"
#include "ui.h"

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


    void Run33State::onEntry(Ui& context) const
    {
      context.setRPM33();
      Led_1::High();
    }
    void Run33State::onExit(Ui& context) const
    {
      Led_1::Low();
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
      Led_2::High();
    }
    void Run45State::onExit(Ui& context) const
    {
      Led_2::Low();
    }
    void Run45State::onPush33(Ui& context) const
    {
      context.setState(Run33State::getInstance());
    }
    void Run45State::onPush45(Ui& context) const
    {
      context.setState(OffState::getInstance());
    }
