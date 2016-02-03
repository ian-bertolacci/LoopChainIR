/*! ****************************************************************************
\file FusionScheduler.cpp
\autors Ian J. Bertolacci

\brief
Fuses a series of loops nests.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "FusionScheduler.hpp"
#include <iostream>
#include <sstream>
#include <iostream>

FusionScheduler::FusionScheduler( std::vector<LoopChain::size_type> loops ) :
  fusion_loops(loops)
  { }


FusionScheduler::FusionScheduler( LoopChain::size_type loops[], int num_loops ) :
  fusion_loops(loops, loops+num_loops)
  { }

Schedule& FusionScheduler::apply( Schedule& schedule ){
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



  std::cout << input_iteration.str() << "\n"
            << output_iteration.str() << "\n"
            << source.str() << "\n"
            << target.str() << "\n"
            << std::endl;

  std::string transformation = SSTR( "{" << "\n"
                                  << "[" << input_iteration.str() << "] -> [" << output_iteration.str() << "] : " << target.str() << " and " << source.str() << ";\n"
                                  << "[" << input_iteration.str() << "] -> [" << input_iteration.str() << "] : !" << source.str() << "\n"
                                  << "};"
                                );
  std::cout << transformation << std::endl;
  schedule.append( transformation );
  return schedule;
}

FusionScheduler::iterator FusionScheduler::begin(){
  return this->fusion_loops.begin();
}

FusionScheduler::iterator FusionScheduler::end(){
  return this->fusion_loops.end();
}
