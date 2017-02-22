/*! ****************************************************************************
\file LoopNest.cpp
\authors Ian J. Bertolacci

\brief
Contains a set of bounds and and the access pattern of a single, arbitarily deep
loop nest.

\copyright
Copyright 2015-2016 Colorado State University
*******************************************************************************/

#include "LoopNest.hpp"

using namespace LoopChainIR;

LoopNest::LoopNest( RectangularDomain loop_bounds ): bounds( loop_bounds ){

}

RectangularDomain& LoopNest::getDomain(){
  return this->bounds;
}
