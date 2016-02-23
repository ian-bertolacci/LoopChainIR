/*! ****************************************************************************
\file FusionTransformation.cpp
\autors Ian J. Bertolacci

\brief
Fuses a series of loops nests.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "FusionTransformation.hpp"
#include <iostream>
#include <sstream>
#include <iostream>

FusionTransformation::FusionTransformation( std::vector<LoopChain::size_type> loops ) :
  fusion_loops(loops)
  { }


FusionTransformation::FusionTransformation( LoopChain::size_type loops[], int num_loops ) :
  fusion_loops(loops, loops+num_loops)
  { }

std::string& FusionTransformation::apply( Schedule& schedule ){
  std::ostringstream input_iteration;
  std::ostringstream output_iteration;
  std::ostringstream source;
  std::ostringstream target;

  input_iteration << "f";
  output_iteration << "t";

  for( RectangularDomain::size_type i = 1; i < schedule.getIteratorsLength(); i += 1 ){
    input_iteration << ",";

    for( RectangularDomain::size_type m = 0; m < i; m += 1 ){
      input_iteration << "i";
    }
  }

  for( RectangularDomain::size_type i = 1; i < schedule.getIteratorsLength()-1; i += 1 ){
    output_iteration << ",";
    for( RectangularDomain::size_type m = 0; m < i; m += 1 ){
      output_iteration << "i";
    }
  }

  output_iteration << ",f";

  source << "(";
  for( iterator it = this->begin(); it != this->end(); ++it ){
    if( it != this->begin() ) source << " or " ;
    source << "f = " << SSTR( *it ) ;
  }
  source << ")";

  target << "( t = " << this->fusion_loops[0] << " )" ;



  std::string* transformation = new std::string(
                               SSTR( "{" << "\n"
                                  << "[" << input_iteration.str() << "] -> [" << output_iteration.str() << "] : " << target.str() << " and " << source.str() << ";\n"
                                  << "[" << input_iteration.str() << "] -> [" << input_iteration.str() << "] : !" << source.str() << "\n"
                                  << "};"
                                )
                              );
  return *transformation;
}

FusionTransformation::iterator FusionTransformation::begin(){
  return this->fusion_loops.begin();
}

FusionTransformation::iterator FusionTransformation::end(){
  return this->fusion_loops.end();
}
