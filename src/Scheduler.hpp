#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "Schedule.hpp"
#include "LoopChain.hpp"
/*!
\brief
Abstract object that all scheduler 'things' inherit for polymorphic purposes.
*/
class Scheduler {
public:
  typedef int size_type;

  /*!
  \brief
  Generate ISL code for a transformation, and append it to the transformation
  list of schedule (modifies schedule).

  \returns
  A reference to the schedule.
  */
  virtual Schedule& apply( Schedule& schedule ) = 0;
};

/*!
\brief
Apply a series of Schedulers to the given Schedule.
Modifies schedule.

\returns
a reference to the transformed schedule
*/
Schedule& apply( Schedule& schedule, std::vector<Scheduler*> schedulers );


/*!
\brief
Apply a series of Schedulers to the given Schedule.
Modifies schedule.

\returns
a reference to the transformed schedule
*/
Schedule& apply( Schedule& schedule, Scheduler* schedulers[], Scheduler::size_type num_schedulers );

/*!
\brief
Initialize a Schedule for the loop chain and apply a series of Schedulers to
that Schedule.
Modifies schedule.

\returns
a reference to the transformed schedule
*/
Schedule& apply( LoopChain& loopchain, std::vector<Scheduler*> schedulers );

/*!
\brief
Initialize a Schedule for the loop chain and apply a series of Schedulers to
that Schedule.
Modifies schedule.

\returns
a reference to the transformed schedule
*/
Schedule& apply( LoopChain& loopchain, Scheduler* schedulers[], Scheduler::size_type num_schedulers );

#endif
