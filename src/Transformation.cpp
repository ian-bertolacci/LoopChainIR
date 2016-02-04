#include "Transformation.hpp"

Schedule& apply( Schedule& schedule, std::vector<Transformation*> schedulers ){
  for( std::vector<Transformation*>::iterator it = schedulers.begin(); it != schedulers.end(); ++it ){
    (*it)->apply( schedule );
  }
  return schedule;
}

Schedule& apply( Schedule& schedule, Transformation* schedulers[], Transformation::size_type num_schedulers ){
  for( int i = 0; i < num_schedulers; i += 1 ){
    (*(schedulers[i])).apply( schedule );
  }
  return schedule;
}

Schedule& apply( LoopChain& loopchain, std::vector<Transformation*> schedulers ){
  Schedule* schedule = new Schedule( loopchain );
  return apply( *schedule, schedulers );
}

Schedule& apply( LoopChain& loopchain, Transformation* schedulers[], Transformation::size_type num_schedulers ){
  Schedule* schedule = new Schedule( loopchain );
  return apply( *schedule, schedulers, num_schedulers );
}
