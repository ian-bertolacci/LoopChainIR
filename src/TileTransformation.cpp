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

TileTransformation::TileTransformation( LoopChain::size_type loop, std::string tile_size ){
  this->loop = loop;
  this->uniform = true;
  this->tile_sizes.push_back( tile_size );
}

std::string& TileTransformation::apply( Schedule& schedule ){
  /*
  HowItWorks: FusionTransformation

  */

  std::ostringstream input_iteration;
  std::ostringstream output_iteration;

  std::ostringstream tile_iterators;
  std::ostringstream loop_iterators;

  std::ostringstream source;
  std::ostringstream condition;

  RectangularDomain domain = schedule.getChain().getNest(this->loop).getDomain();

  // to/from iterators
  input_iteration << "l";
  output_iteration << "l";

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
  }

  input_iteration << loop_iterators.str();
  output_iteration << tile_iterators.str() << loop_iterators.str();

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
    std::string size_exp = this->tile_sizes[ (this->uniform)? 0 : i ];

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
                 << "[" << input_iteration.str() << "] -> [" << output_iteration.str() << "] : " << source.str() << " and " << condition.str() << ";\n"
                 // Identity transformation for pass-through
                 << "[" << input_iteration.str() << "] -> [" << input_iteration.str() << "] : !" << source.str() << "\n"
                 << "};";
  return *(new std::string(transformation.str()));
}
