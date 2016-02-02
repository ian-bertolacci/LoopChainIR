#include "Scheduler.hpp"

Schedule& apply( Schedule& schedule, std::vector<Scheduler*> schedulers ){
  for( std::vector<Scheduler*>::iterator it = schedulers.begin(); it != schedulers.end(); ++it ){
    (*it)->apply( schedule );
  }
  return schedule;
}

Schedule& apply( Schedule& schedule, Scheduler* schedulers[], Scheduler::size_type num_schedulers ){
  for( int i = 0; i < num_schedulers; i += 1 ){
    (*(schedulers[i])).apply( schedule );
  }
  return schedule;
}

Schedule& apply( LoopChain& loopchain, std::vector<Scheduler*> schedulers ){
  Schedule* schedule = new Schedule( loopchain );
  return apply( *schedule, schedulers );
}

Schedule& apply( LoopChain& loopchain, Scheduler* schedulers[], Scheduler::size_type num_schedulers ){
  Schedule* schedule = new Schedule( loopchain );
  return apply( *schedule, schedulers, num_schedulers );
}
