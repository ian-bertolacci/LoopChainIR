/*******************************************************************************
\file Box.cpp
\autors Ian J. Bertolacci

\purpose
Contains the bounds on a loop nest.
Currently, bounds are represented simply as a string, containing only the
expression that gives that bound. For example the bounds 0 <= x <= N+M would be
"0" for the lower bound, and "N+M" for the upper bound.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "Box.h"

Box::Box( std::string input_lower_bounds[], std::string input_upper_bounds[], size_type dimensions ){
  if( input_lower_bounds == NULL ){
    // error
  }

  if( input_upper_bounds == NULL ){
    // error
  }

  if( dimensions <= 0 ){
    // error
  }

  for( size_type d = 0; d < dimensions; d += 1 ){
    this->lower_bounds.push_back( input_lower_bounds[d] );
    this->upper_bounds.push_back( input_upper_bounds[d] );
  }

}

Box::size_type Box::dimensions(){
  return this->lower_bounds.size();
}
std::string Box::getUpperBound( Box::size_type dimension ){
  return this->upper_bounds[dimension];
}

std::string Box::getLowerBound( Box::size_type dimension ){
  return this->lower_bounds[dimension];
}
