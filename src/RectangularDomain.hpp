/*! ****************************************************************************
\file RectangularDomain.hpp
\autors Ian J. Bertolacci

\brief
Contains the bounds on a loop nest.
Currently, bounds are represented simply as a string, containing only the
expression that gives that bound. For example the bounds 0 <= x <= N+M would be
"0" for the lower bound, and "N+M" for the upper bound.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#ifndef RECTANGULAR_DOMAIN_HPP
#define RECTANGULAR_DOMAIN_HPP

#include <string>
#include <vector>
#include <utility>

namespace LoopChainIR{

  /*
  Encapsulates the bounds of a rectangular domain.
  */
  class RectangularDomain
  {
  private:
    std::vector<std::string> upper_bounds;
    std::vector<std::string> lower_bounds;
    std::vector<std::string> symbols;

  public:
    typedef std::vector<std::string>::size_type size_type;

    //RectangularDomain();
    /*!
    \param[in] lower_bound string
    \param[in] upper_bound string
    */
    RectangularDomain( std::string lower, std::string upper);

    /*!
    \param[in] input_lower_bounds ordered list of lower bounds
    \param[in] input_upper_bounds ordered list of upper bounds
    \param[in] dimension the dimensionality of the domain
    \param[in] symbols the list of symbols found in the bounds
    \param[in] symbolics number of symbols
    */
    RectangularDomain( std::string input_lower_bounds[], std::string input_upper_bounds[], size_type dimensions, std::string symbols[], size_type symbolics );

    /*!
    \param[in] input_lower_bounds ordered list of lower bounds
    \param[in] input_upper_bounds ordered list of upper bounds
    \param[in] dimension the dimensionality of the domain
    */
    RectangularDomain( std::string input_lower_bounds[], std::string input_upper_bounds[], size_type dimensions );

    /*!
    \param[in] RectangularDomain to append
    */
    void append(RectangularDomain d);

    /*!
    \returns the dimensionality of the domain
    */
    size_type dimensions();

    /*!
    \returns the number of symbolics
    */
    size_type symbolics();

    /*!
    \returns the upper bound of the dimension.
    */
    std::string getUpperBound( size_type dimension );

    /*!
    \returns the lower bound of the dimension.
    */
    std::string getLowerBound( size_type dimension );

    /*!
    \returns the N'th symbol from the list as it was provided to the constructor.
    */
    std::string getSymbol( size_type symbolic );

  };

}
#endif
