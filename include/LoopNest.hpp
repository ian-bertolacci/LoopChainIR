/*! ****************************************************************************
\file LoopNest.hpp
\authors Ian J. Bertolacci

\brief
Contains a set of bounds and and the access pattern of a single, arbitarily deep
loop nest.

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#ifndef LOOPNEST_HPP
#define LOOPNEST_HPP

#include "RectangularDomain.hpp"

namespace LoopChainIR{

  /*!
  Encapsulates a single loop nest of arbitrary depth.
  */
  class LoopNest{
  private:
    RectangularDomain bounds;

  public:

    /*!
    \param[in] loop_bounds Domain of the loop nest
    */
    LoopNest( RectangularDomain loop_bounds );

    /*!
    \returns reference to this LoopNest's RectangularDomain.
    */
    RectangularDomain& getDomain();
  };

}

#endif
