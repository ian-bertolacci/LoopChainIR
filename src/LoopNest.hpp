/*******************************************************************************
\file LoopNest.hpp
\autors Ian J. Bertolacci

\purpose
Contains a set of bounds and and the access pattern of a single, arbitarily deep
loop nest.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#ifndef LOOPNEST_HPP
#define LOOPNEST_HPP

#include "RectangularDomain.hpp"

class LoopNest{
private:
  RectangularDomain bounds;

public:
  LoopNest( RectangularDomain loop_bounds );
  RectangularDomain& getDomain();
};

#endif
