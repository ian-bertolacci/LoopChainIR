#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "Schedule.hpp"
#include "LoopChain.hpp"

class Scheduler {
public:
  virtual Schedule& apply( Schedule& schedule ) = 0;
};

Schedule& apply( Schedule& schedule, std::vector<Scheduler*> schedulers );

Schedule& apply( LoopChain& loopchain, std::vector<Scheduler*> schedulers );

#endif
