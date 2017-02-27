/*! ****************************************************************************
\file DefaultSequentialTransformation.hpp
\authors Ian J. Bertolacci

\brief
Generate a simple sequential, non-parallel schedule from a LoopChain.

\copyright
Copyright 2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#ifndef DEFAULT_SEQUENTIAL_SCHEDULE_HPP
#define DEFAULT_SEQUENTIAL_SCHEDULE_HPP

#include "LoopChain.hpp"
#include "Transformation.hpp"
#include "Schedule.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "stdio.h"

namespace LoopChainIR {

  /*!
  Schedules a loopchain in the default, sequential manner.

  \note
  This Transformation does not actually transform any schedule, since this would
  only be performed on a schedule that had not already been transformed, and a
  LoopChain that has been converted into a Schedule object has already been
  properly transformed into a sequential schedule.
  */
  class DefaultSequentialTransformation : public Transformation {

  public:
    DefaultSequentialTransformation( );

    /*!
    \returns Reference to schedule which has been transformed.
    */
    std::vector<std::string> apply( Schedule& schedule, Subspace* subspace);

    /*!
    \returns Reference to schedule which has been transformed.
    */
    std::vector<std::string> apply( Schedule& schedule );
  };

}

#endif
