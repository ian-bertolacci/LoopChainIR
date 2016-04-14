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
#include <iostream>
#include <sstream>
#include "stdio.h"

/*!
Fuses two or more loops into one.
*/
class TileTransformation : public Transformation {
private:

  LoopChain::size_type loop;
  std::vector<std::string> tile_sizes;
  bool uniform;

public:
  typedef std::vector<std::string>::size_type size_type;

  /*!
  \brief
  Create tiling schedule with single tile size

  \param[in] loop Id of loop to transform;
  \param[in] tile_size Size of tiles for loop, for all dimensions of tile.
  */
  TileTransformation( LoopChain::size_type loop, std::string tile_size );

  /*!
  \brief
  Create tiling schedule with an list of tile size

  \param[in] loop Id of loop to transform;
  \param[in] tile_size Size of tiles for loop, for all dimensions of tile.
  */
  TileTransformation( LoopChain::size_type loop, std::vector<std::string> tile_sizes );

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
  std::string getSize( TileTransformation::size_type i );


  /*!
  \brief
  Returns a copy of the provided tile size vector
  */
  std::vector<std::string> getSizes();

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

#endif
