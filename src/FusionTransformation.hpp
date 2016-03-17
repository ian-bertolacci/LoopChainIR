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
  /*!
  List loops to be fused.
  Order dictates the order in which the bodies of the loops are prepended.

  e.g. if fusing {0,1} then the body of loop 0 comes before the body of loop 1.
       if fusing {1,0} then the body of loop 1 comes before the body of loop 0.
  */
  FusionTransformation( std::vector<LoopChain::size_type> loops );

  /*!
  List loops to be fused.
  Order dictates the order in which the bodies of the loops are prepended.

  e.g. if fusing {0,1} then the body of loop 0 comes before the body of loop 1.
       if fusing {1,0} then the body of loop 1 comes before the body of loop 0.
  */
  FusionTransformation( LoopChain::size_type loops[], int num_loops );

  /*!
  \returns Reference to schedule which has been transformed.
  */
  std::string& apply( Schedule& schedule );
};

#endif
