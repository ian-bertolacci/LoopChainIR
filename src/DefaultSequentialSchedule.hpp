/*******************************************************************************
\file DefaultSequentialSchedule.h
\autors Ian J. Bertolacci

\purpose
Generate a simple sequential, non-parallel schedule from a LoopChain.

\copyright
Copyright 2015 Colorado State University
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

class DefaultSequentialSchedule {
private:
  LoopChain chain;

public:
  DefaultSequentialSchedule( LoopChain& chain );

  LoopChain& getChain();

  void codegen( FILE* output_file );
};


#endif
