/*! ****************************************************************************
\file DefaultSequentialTransformation.cpp
\authors Ian J. Bertolacci

\brief
Generate a simple sequential, non-parallel schedule.

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#include "DefaultSequentialTransformation.hpp"
#include "util.hpp"
#include <iostream>

using namespace LoopChainIR;

DefaultSequentialTransformation::DefaultSequentialTransformation(){ }

std::vector<std::string> DefaultSequentialTransformation::apply( Schedule& schedule ){
  return this->apply( schedule, nullptr );
}

std::vector<std::string> DefaultSequentialTransformation::apply( Schedule& schedule __attribute__((unused)), Subspace* subspace __attribute__((unused)) ){
  // Produce empty transformation.
  return std::vector<std::string>();
}
