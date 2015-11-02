/*******************************************************************************
\file LoopNest.h
\autors Ian J. Bertolacci

\purpose
Contains a set of bounds and and the access pattern of a single, arbitarily deep
loop nest.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#ifndef LOOPNEST_H
#define LOOPNEST_H

#include "Box.h"

class LoopNest{
private:
  Box bounds;

public:
  LoopNest( Box loop_bounds );
  Box& getBounds();
};

#endif
