/*! ****************************************************************************
\file FusionTransformation.cpp
\autors Ian J. Bertolacci

\brief
Fuses a series of loops nests.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#ifndef FUSION_SCHEDULE_HPP
#define FUSION_SCHEDULE_HPP

#include "LoopChain.hpp"
#include "Transformation.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "stdio.h"

/*!
Fuses two or more loops into one.

*/
class FusionTransformation : public Transformation {
private:
  typedef std::vector<LoopChain::size_type>::iterator iterator;
  std::vector<LoopChain::size_type> fusion_loops;

  std::vector<LoopChain::size_type>::iterator begin();
  std::vector<LoopChain::size_type>::iterator end();

public:

  FusionTransformation( std::vector<LoopChain::size_type> loops );

  FusionTransformation( LoopChain::size_type loops[], int num_loops );

  /*!
  \returns Reference to schedule which has been scheduled
  */
  std::string& apply( Schedule& schedule );
};

#endif
