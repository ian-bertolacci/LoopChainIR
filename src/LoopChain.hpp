/*! ****************************************************************************
\file LoopChain.hpp
\autors Ian J. Bertolacci

\brief
Contains an ordered sequence of loop nests.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#ifndef LOOPCHAIN_HPP
#define LOOPCHAIN_HPP

#include "LoopNest.hpp"
/*!
Encapsulates an ordered chain of LoopNests.
*/
class LoopChain {
private:
  std::vector<LoopNest> chain;

public:
  typedef std::vector<LoopNest>::size_type size_type;

  LoopChain();

  /*!
  Appends the LoopNest onto the list.
  \param[in] nest LoopNest to append.
  */
  void append( LoopNest nest );

  /*!
  \returns reference to the LoopNest object at index.
  */
  LoopNest& getNest( size_type index );

  /*!
  \returns number of LoopNest objects in chain.
  */
  size_type length();

  /*
  \returns the maximum dimensionality of all the loop-nests in the chain.
  */
  RectangularDomain::size_type maxDimension();

};

#endif
