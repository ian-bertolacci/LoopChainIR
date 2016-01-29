/*******************************************************************************
\file DefaultSequentialSchedule.cpp
\autors Ian J. Bertolacci

\purpose
Generate a simple sequential, non-parallel schedule.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "DefaultSequentialSchedule.hpp"
#include <iostream>

DefaultSequentialSchedule::DefaultSequentialSchedule(){ }

Schedule& DefaultSequentialSchedule::apply( Schedule& schedule ){
  return schedule;
}
