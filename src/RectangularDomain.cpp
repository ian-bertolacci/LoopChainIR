/*! ****************************************************************************
\file RectangularDomain.cpp
\authors Ian J. Bertolacci

\brief
Contains the bounds on a loop nest.
Currently, bounds are represented simply as a string, containing only the
expression that gives that bound. For example the bounds 0 <= x <= N+M would be
"0" for the lower bound, and "N+M" for the upper bound.

\copyright
Copyright 2015-2016 Colorado State University
*******************************************************************************/

#include "RectangularDomain.hpp"
#include "util.hpp"

using namespace LoopChainIR;


RectangularDomain::RectangularDomain( std::string input_lower_bounds, std::string input_upper_bounds ){

  this->lower_bounds.push_back( input_lower_bounds );
  this->upper_bounds.push_back( input_upper_bounds );

}

RectangularDomain::RectangularDomain( std::string input_lower_bounds, std::string input_upper_bounds, const std::set<std::string> symbols )
: symbols( symbols )
{
  this->lower_bounds.push_back( input_lower_bounds );
  this->upper_bounds.push_back( input_upper_bounds );
}

RectangularDomain::RectangularDomain( std::string input_lower_bounds[], std::string input_upper_bounds[], size_type dimensions ){
  assertWithException( input_lower_bounds != NULL, "Lower bounds array cannot be null" );
  assertWithException( input_upper_bounds != NULL, "Upper bounds array cannot be null" );
  assertWithException( dimensions >= 1, "Cannot have domain with fewer than one dimension" );

  for( size_type d = 0; d < dimensions; d += 1 ){
    this->lower_bounds.push_back( input_lower_bounds[d] );
    this->upper_bounds.push_back( input_upper_bounds[d] );
  }

}

RectangularDomain::RectangularDomain( std::string input_lower_bounds[], std::string input_upper_bounds[], size_type dimensions, std::string symbols[], size_type symbolics ){
  assertWithException( input_lower_bounds != NULL, "Lower bounds array cannot be null" );
  assertWithException( input_upper_bounds != NULL, "Upper bounds array cannot be null" );
  assertWithException( dimensions >= 1, "Cannot have domain with fewer than one dimension" );
  assertWithException( symbols != NULL, "Symbols array cannot be null" );

  for( size_type d = 0; d < dimensions; d += 1 ){
    this->lower_bounds.push_back( input_lower_bounds[d] );
    this->upper_bounds.push_back( input_upper_bounds[d] );
  }

  for( size_type d = 0; d < symbolics; d += 1 ){
    this->symbols.insert( symbols[d] );
  }

}

RectangularDomain::RectangularDomain( std::string input_lower_bounds[], std::string input_upper_bounds[], size_type dimensions, const std::set<std::string> symbols )
: symbols( symbols )
{
  assertWithException( input_lower_bounds != NULL, "Lower bounds array cannot be null" );
  assertWithException( input_upper_bounds != NULL, "Upper bounds array cannot be null" );
  assertWithException( dimensions >= 1, "Cannot have domain with fewer than one dimension" );

  for( size_type d = 0; d < dimensions; d += 1 ){
    this->lower_bounds.push_back( input_lower_bounds[d] );
    this->upper_bounds.push_back( input_upper_bounds[d] );
  }

}

void RectangularDomain::append( RectangularDomain other){
  this->symbols.insert( other.symbols.begin(), other.symbols.end() );
  for( size_type d = 0; d < other.dimensions(); d += 1 ){
    this->lower_bounds.push_back( other.getLowerBound(d) );
    this->upper_bounds.push_back( other.getUpperBound(d) );
  }
}

RectangularDomain::size_type RectangularDomain::dimensions(){
  return this->lower_bounds.size();
}

RectangularDomain::size_type RectangularDomain::symbolics(){
  return this->symbols.size();
}

std::string RectangularDomain::getUpperBound( RectangularDomain::size_type dimension ){
  return this->upper_bounds[dimension];
}

std::string RectangularDomain::getLowerBound( RectangularDomain::size_type dimension ){
  return this->lower_bounds[dimension];
}

std::set<std::string> RectangularDomain::getSymbols( ){
  return this->symbols;
}
