#include "HardwareConfig.h"
#include "stateMachine.h"

    void OffState::onEntry(Context& context) const
    {
      Led_1::Low();
      Led_2::Low();
    }
    void OffState::onPush33(Context& context) const
    {
      context.setState(Run33State::getInstance());
    }
    void OffState::onPush45(Context& context) const
    {
      context.setState(Run45State::getInstance());
    }


    void Run33State::onEntry(Context& context) const
    {
      Led_1::High();
    }
    void Run33State::onExit(Context& context) const
    {
      Led_1::Low();
    }
    void Run33State::onPush33(Context& context) const
    {
      context.setState(OffState::getInstance());
    }
    void Run33State::onPush45(Context& context) const
    {
      context.setState(Run45State::getInstance());
    }

    void Run45State::onEntry(Context& context) const
    {
      Led_2::High();
    }
    void Run45State::onExit(Context& context) const
    {
      Led_2::Low();
    }
    void Run45State::onPush33(Context& context) const
    {
      context.setState(Run33State::getInstance());
    }
    void Run45State::onPush45(Context& context) const
    {
      context.setState(OffState::getInstance());
    }
