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

using namespace LoopChainIR;

DefaultSequentialTransformation::DefaultSequentialTransformation(){ }

std::string& DefaultSequentialTransformation::apply( Schedule& schedule ){
  return this->apply( schedule, nullptr );
}

std::string& DefaultSequentialTransformation::apply( Schedule& schedule __attribute__((unused)), Subspace* subspace __attribute__((unused)) ){
  // Produce empty transformation.
  std::string* transformation = new std::string();
  return *transformation;
}
