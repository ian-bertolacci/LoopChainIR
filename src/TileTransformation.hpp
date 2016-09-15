/*! ****************************************************************************
\file TileTransformation.cpp
\autors Ian J. Bertolacci

\brief
Tile a loop nest

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#ifndef TILE_SCHEDULE_HPP
#define TILE_SCHEDULE_HPP

#include "LoopChain.hpp"
#include "Transformation.hpp"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include "stdio.h"

namespace LoopChainIR {
  /*!
  Fuses two or more loops into one.
  */
  class TileTransformation : public Transformation {

  public:
    typedef RectangularDomain::size_type key_type;
    typedef std::string mapped_type;
    typedef std::map<key_type, mapped_type> TileMap;

  private:
    LoopChain::size_type loop;
    TileMap tile_sizes;
    bool uniform;
    mapped_type uniform_size;

  public:
    /*!
    DEPRICATED
    \brief
    Create tiling schedule with single tile size

    \param[in] loop Id of loop to transform;
    \param[in] tile_size Size of tiles for loop, for all dimensions of tile.
    */
    //TileTransformation( LoopChain::size_type loop, mapped_type tile_size );

    /*!
    \brief
    Create tiling schedule with an list of tile size

    \param[in] loop Id of loop to transform;
    \param[in] tile_size Size of tiles for loop, for all dimensions of tile.
    */
    TileTransformation( LoopChain::size_type loop, TileMap tile_sizes );

    /*!
    \brief
    Return true if tile size is uniform across all dimensions, false otherwise.
    */
    bool isUniformSize( );

    /*!
    \brief
    Returns the size of the tile in dimension i
    \param[in] i Dimension of the domain
    */
    mapped_type getSize( TileTransformation::key_type i );


    /*!
    \brief
    Returns a copy of the provided tile size vector
    */
    TileMap getSizes();

    /*!
    \brief
    Generate ISCC code for the shift transformation, and append it to the
    transformation list of schedule (modifies schedule).

    \param[inout] schedule Schedule this transformation is being applied to.

    \returns
    The ISCC code as a string
    */
    std::string& apply( Schedule& schedule );
  };
}
#endif
