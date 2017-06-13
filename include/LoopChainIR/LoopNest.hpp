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

#include <list>
#include <LoopChainIR/RectangularDomain.hpp>
#include <LoopChainIR/Accesses.hpp>

namespace LoopChainIR{

  /*!
  Encapsulates a single loop nest of arbitrary depth.
  */
  class LoopNest{
  private:
    RectangularDomain bounds;
    std::list<Dataspace> dataspaces;

  public:

    /*!
    \param[in] loop_bounds Domain of the loop nest
    */
    LoopNest( RectangularDomain loop_bounds );
    LoopNest( RectangularDomain loop_bounds, std::list<Dataspace> dataspaces );


    /*!
    \returns reference to this LoopNest's RectangularDomain.
    */
    RectangularDomain& getDomain();

    /*!
    \returns a copy of the LoopNest's Dataspaces.
    */
    std::list<Dataspace> getDataspaces() const;

    /*!
    \brief Replaces the original dataspaces with the given ones.
    */
    void replaceDataspaces( std::list<Dataspace> dataspaces );

    /*!
    \brief Shifts all the accesses by some extent.
    */
    void shiftDataspaces( Tuple extent );
  };

}

#endif
