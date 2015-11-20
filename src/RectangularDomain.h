/*******************************************************************************
\file RectangularDomain.h
\autors Ian J. Bertolacci

\purpose
Contains the bounds on a loop nest.
Currently, bounds are represented simply as a string, containing only the
expression that gives that bound. For example the bounds 0 <= x <= N+M would be
"0" for the lower bound, and "N+M" for the upper bound.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#ifndef RECTANGULAR_DOMAIN_H
#define RECTANGULAR_DOMAIN_H

#include <string>
#include <vector>
#include <utility>

class RectangularDomain
{
private:
  std::vector<std::string> upper_bounds;
  std::vector<std::string> lower_bounds;
  std::vector<std::string> symbols;

public:
  typedef std::vector<std::string>::size_type size_type;

  RectangularDomain( std::string input_lower_bounds[], std::string input_upper_bounds[], size_type dimensions, std::string symbols[], size_type symbolics );
  RectangularDomain( std::string input_lower_bounds[], std::string input_upper_bounds[], size_type dimensions );
  size_type dimensions();
  size_type symbolics();

  std::string getUpperBound( size_type dimension );
  std::string getLowerBound( size_type dimension );
  std::string getSymbol( size_type symbolic );

};

#endif
