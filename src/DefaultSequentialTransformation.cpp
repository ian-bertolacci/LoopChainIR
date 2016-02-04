/*! ****************************************************************************
\file DefaultSequentialTransformation.cpp
\autors Ian J. Bertolacci

\brief
Generate a simple sequential, non-parallel schedule.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "DefaultSequentialTransformation.hpp"
#include <iostream>

DefaultSequentialTransformation::DefaultSequentialTransformation(){ }

Schedule& DefaultSequentialTransformation::apply( Schedule& schedule ){
  return schedule;
}
