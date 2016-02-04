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

std::string& DefaultSequentialTransformation::apply( Schedule& schedule ){
  // To trick the compiler into thinking schedule is used.
  schedule.getIteratorsLength();

  // Produce empty transformation.
  std::string* transformation = new std::string();
  return *transformation;
}
