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

TileTransformation::TileTransformation( LoopChain::size_type loop, std::string tile_size )
  : loop(loop), uniform(true){
  this->tile_sizes.push_back( tile_size );
}

TileTransformation::TileTransformation( LoopChain::size_type loop, std::vector<std::string> tile_sizes )
  : loop(loop), tile_sizes( tile_sizes ), uniform(tile_sizes.size() == 1) { }


bool TileTransformation::isUniformSize( ){
  return this->uniform;
}

std::string TileTransformation::getSize( TileTransformation::size_type i ){
  return this->tile_sizes[ this->isUniformSize()?0:i ];
}

std::vector<std::string> TileTransformation::getSizes(){
  return std::vector<std::string>( this->tile_sizes );
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
  std::ostringstream condition;

  RectangularDomain domain = schedule.getChain().getNest(this->loop).getDomain();

  bool check = this->uniform || (this->tile_sizes.size() == domain.dimensions());
  assertWithException( check, "No uniform tile size provided nor are there an equal number of tile sizes and dimensions." );

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
  for( RectangularDomain::size_type i = 1; i <= domain.dimensions(); i += 1 ){
    // Create unique symbol for tile iterator ("t"+i, eg "t1", "t2", ...)
    tile_iterators << ",t" << i;
    nontile_iterators << ",0";
  }

  input_iteration << loop_iterators.str();
  tiled_output_iteration << tile_iterators.str() << loop_iterators.str();
  untiled_output_iteration << nontile_iterators.str() << loop_iterators.str();

  // Create conditional expression for source loops (eg (f=0 or f=1 or ...))
  source << "(l = " << this->loop << ")";

  condition << "Exists(" ;

  for( RectangularDomain::size_type i = 1; i <= domain.dimensions(); i += 1 ){
    if( i != 1 ){
      condition << ",";
    }

    condition << "r" << i;
  }

  condition << " : " ;

  for( RectangularDomain::size_type i = 1; i <= domain.dimensions(); i += 1 ){
    std::string size_exp = this->tile_sizes[ (this->uniform)? 0 : i-1 ];

    if( i != 1 ){
      condition << " and ";
    }
    condition << "0 <= r" << i << " < " << size_exp
              << " and i" << i << " = t" << i << " * " << size_exp
              << " + r" << i;
  }

  condition << ")";


  // Create our transformation mapping
  std::ostringstream transformation;
  transformation << "{" << "\n"
                 // transformation for targeted loops
                 << "[" << input_iteration.str() << "] -> [" << tiled_output_iteration.str() << "] : " << source.str() << " and " << condition.str() << ";\n"
                 // Identity transformation for pass-through
                 << "[" << input_iteration.str() << "] -> [" << untiled_output_iteration.str() << "] : !" << source.str() << "\n"
                 << "};";
  return *(new std::string(transformation.str()));
}
