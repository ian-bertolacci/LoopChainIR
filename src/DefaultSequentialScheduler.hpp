/*! ****************************************************************************
\file DefaultSequentialScheduler.hpp
\autors Ian J. Bertolacci

\brief
Generate a simple sequential, non-parallel schedule from a LoopChain.

\copyright
Copyright 2016 Colorado State University
*******************************************************************************/

#ifndef DEFAULT_SEQUENTIAL_SCHEDULE_HPP
#define DEFAULT_SEQUENTIAL_SCHEDULE_HPP

#include "LoopChain.hpp"
#include "Scheduler.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "stdio.h"

/*!
Schedules a loopchain in the default, sequential manner.

\note
This Scheduler does not actually transform any schedule, since this would only
be performed on a schedule that had not already been transformed, and a
LoopChain that has been converted into a Schedule object has already been
properly transformed into a sequential schedule.
*/
class DefaultSequentialScheduler : public Scheduler {

public:
  DefaultSequentialScheduler( );
  /*!
  \returns Reference to schedule which has been scheduled
  */
  Schedule& apply( Schedule& schedule );
};


#endif
