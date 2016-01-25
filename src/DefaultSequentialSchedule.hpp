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
#include "all_isl.hpp"
#include "util.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "stdio.h"

/*!
Schedules a loopchain in the default, sequential manner.
*/
class DefaultSequentialSchedule {
private:
  //! LoopChain being scheduled.
  LoopChain chain;

public:
  /*!
  \param[in] chain a reference to the LoopChain to be scheduled.
  */
  DefaultSequentialSchedule( LoopChain& chain );

  /*!
  \returns Reference to the LoopChain object to be scheduled.
  */
  LoopChain& getChain();

  /*!
  Performs scheduling and codegen, writing output to output_file
  \param[in] output_file File pointer to output.
  */
  void codegen( FILE* output_file );
};


#endif
