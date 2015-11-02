/*******************************************************************************
\file LoopNest.cpp
\autors Ian J. Bertolacci

\purpose
Contains a set of bounds and and the access pattern of a single, arbitarily deep
loop nest.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "LoopNest.h"

LoopNest::LoopNest( Box loop_bounds ): bounds( loop_bounds ){

}

Box& LoopNest::getBounds(){
  return this->bounds;
}
