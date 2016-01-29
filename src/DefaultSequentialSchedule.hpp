/*! ****************************************************************************
\file DefaultSequentialSchedule.hpp
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
*/
class DefaultSequentialSchedule : public Scheduler {

public:
  DefaultSequentialSchedule( );

  Schedule& apply( Schedule& schedule );
};


#endif
