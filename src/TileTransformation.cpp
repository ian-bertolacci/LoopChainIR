/*! ****************************************************************************
\file TileTransformation.cpp
\autors Ian J. Bertolacci

\brief
Tile a loop nest

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "TileTransformation.hpp"
#include <iostream>
#include <sstream>
#include <iostream>

using namespace LoopChainIR;

/*
TileTransformation::TileTransformation( LoopChain::size_type loop, TileTransformation::mapped_type tile_size )
  : loop(loop), uniform(true), uniform_size(tile_size) {

}
*/

TileTransformation::TileTransformation( LoopChain::size_type loop, TileTransformation::TileMap tile_sizes )
  : loop(loop), tile_sizes( tile_sizes ), uniform( false ) {

  assertWithException( tile_sizes.size() > 0, "Must tile along one or more dimensions." );
}


bool TileTransformation::isUniformSize( ){
  return this->uniform;
}

TileTransformation::mapped_type TileTransformation::getSize( TileTransformation::key_type i ){
  return this->isUniformSize() ? this->uniform_size : this->tile_sizes[ i ];
}

TileTransformation::TileMap TileTransformation::getSizes(){
  return std::map<TileTransformation::key_type, TileTransformation::mapped_type>( this->tile_sizes );
}

std::string& TileTransformation::apply( Schedule& schedule ){
  /*
  HowItWorks: FusionTransformation

  */

  std::ostringstream input_iteration;
  std::ostringstream tiled_output_iteration;
  std::ostringstream untiled_output_iteration;

  std::ostringstream tile_iterators;
  std::ostringstream nontile_iterators;
  std::ostringstream loop_iterators;

  std::ostringstream source;
  std::ostringstream tile_constraints;
  std::ostringstream condition;

  RectangularDomain domain = schedule.getChain().getNest(this->loop).getDomain();

  assertWithException( this->tile_sizes.size() <= domain.dimensions(), "Tiling more dimensions than exist in domain." );

  // to/from iterators
  input_iteration << "l";
  tiled_output_iteration << "l";
  untiled_output_iteration << "l";

  // Create rest of input iteration
  for( RectangularDomain::size_type i = 1; i < schedule.getIteratorsLength(); i += 1 ){
    // Create unique symbol for iterator ("i"+i, eg "i1", "i2", ...)
    loop_iterators << ",i" << i;
  }

  // Create rest of output iteration
  // First the tile iterators
  for( auto tile_dim : this->tile_sizes ){
    // Create unique symbol for tile iterator ("t"+i, eg "t1", "t2", ...)
    tile_iterators << ",t" << tile_dim.first;
    nontile_iterators << ",0";
  }

  input_iteration << loop_iterators.str();
  tiled_output_iteration << tile_iterators.str() << loop_iterators.str();
  untiled_output_iteration << nontile_iterators.str() << loop_iterators.str();

  // Create conditional expression for source loops (eg (f=0 or f=1 or ...))
  source << "(l = " << this->loop << ")";

  bool first = true;
  for( auto tile_dim : this->tile_sizes ){
    if( !first ){
      tile_constraints << ",";
      condition << " and ";
    }
    first = false;

    tile_constraints << "r" << tile_dim.first;

    condition << "0 <= r" << tile_dim.first << " < " << tile_dim.second
              << " and i" << tile_dim.first << " = t" << tile_dim.first << " * " << tile_dim.second
              << " + r" << tile_dim.first;
  }

  // Create our transformation mapping
  std::ostringstream transformation;
  transformation << "{" << "\n"
                 // transformation for targeted loops
                 << "[" << input_iteration.str() << "] -> [" << tiled_output_iteration.str() << "] : " << source.str() << " and Exists(" << tile_constraints.str() << " : "<< condition.str() << ");\n"
                 // Identity transformation for pass-through
                 << "[" << input_iteration.str() << "] -> [" << untiled_output_iteration.str() << "] : !" << source.str() << "\n"
                 << "};";

  return *(new std::string(transformation.str()));
}
