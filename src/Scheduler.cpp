#include "Scheduler.hpp"

Schedule& apply( Schedule& schedule, std::vector<Scheduler*> schedulers ){
  for( std::vector<Scheduler*>::iterator it = schedulers.begin(); it != schedulers.end(); ++it ){
    (*it)->apply( schedule );
  }
  return schedule;
}

Schedule& apply( LoopChain& loopchain, std::vector<Scheduler*> schedulers ){
  Schedule* schedule = new Schedule( loopchain );
  return apply( *schedule, schedulers );
}
