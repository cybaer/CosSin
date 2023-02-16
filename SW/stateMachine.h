#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

class Context;

class IState
{
  public:
    IState(void) {}
    virtual void onEntry(Context&) const {}
    virtual void onExit(Context&) const {}
    virtual void onPush33(Context&) const {}
    virtual void onPush45(Context&) const {}
    virtual void onPushEncoder(Context&) const {}
    virtual void onIncrement(Context&, int8_t incr) const {}
};


class Context
{
public:
  Context(IState& state)
  : m_State(&state)
  {}

  void setState(IState& state)
  {
    m_State->onExit(*this);
    m_State = &state;
    m_State->onEntry(*this);
  }

  IState* m_State;
};

  /**
   * State machine
   */
class OffState: public IState
{
  public:
    static OffState& getInstance(void)
    {
      static OffState s_instance;
      return s_instance;
    }
    void onEntry(Context& context) const;
    void onPush33(Context& context) const;
    void onPush45(Context& context) const;
};

class Run33State: public IState
{
  public:
    static Run33State& getInstance(void)
    {
      static Run33State s_instance;
      return s_instance;
    }
    void onEntry(Context& context) const;
    void onExit(Context& context) const;
    void onPush33(Context& context) const;
    void onPush45(Context& context) const;
};

class Run45State: public IState
{
  public:
    static Run45State& getInstance(void)
    {
      static Run45State s_instance;
      return s_instance;
    }
    void onEntry(Context& context) const;
    void onExit(Context& context) const;
    void onPush33(Context& context) const;
    void onPush45(Context& context) const;
    
};

#endif