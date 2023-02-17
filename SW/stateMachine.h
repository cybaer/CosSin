#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

class Ui;

class IState
{
  public:
    IState(void) {}
    virtual void onEntry(Ui&) const {}
    virtual void onExit(Ui&) const {}
    virtual void onPush33(Ui&) const {}
    virtual void onPush45(Ui&) const {}
    virtual void onPushEncoder(Ui&) const {}
    virtual void onIncrement(Ui&, int8_t incr) const {}
};

  /**
   * State machine
   */
class InitState: public IState
{
  public:
    static InitState& getInstance(void)
    {
      static InitState s_instance;
      return s_instance;
    }
};  
class OffState: public IState
{
  public:
    static OffState& getInstance(void)
    {
      static OffState s_instance;
      return s_instance;
    }
    void onEntry(Ui& context) const;
    void onPush33(Ui& context) const;
    void onPush45(Ui& context) const;
};

class Run33State: public IState
{
  public:
    static Run33State& getInstance(void)
    {
      static Run33State s_instance;
      return s_instance;
    }
    void onEntry(Ui& context) const;
    void onExit(Ui& context) const;
    void onPush33(Ui& context) const;
    void onPush45(Ui& context) const;
};

class Run45State: public IState
{
  public:
    static Run45State& getInstance(void)
    {
      static Run45State s_instance;
      return s_instance;
    }
    void onEntry(Ui& context) const;
    void onExit(Ui& context) const;
    void onPush33(Ui& context) const;
    void onPush45(Ui& context) const;
    
};

#endif