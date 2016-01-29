/*******************************************************************************
\file DefaultSequentialScheduler.cpp
\autors Ian J. Bertolacci

\purpose
Generate a simple sequential, non-parallel schedule.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "DefaultSequentialScheduler.hpp"
#include <iostream>

DefaultSequentialScheduler::DefaultSequentialScheduler(){ }

Schedule& DefaultSequentialScheduler::apply( Schedule& schedule ){
  return schedule;
}
