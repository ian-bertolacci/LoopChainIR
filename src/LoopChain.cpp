/*! ****************************************************************************
\file LoopChain.cpp
\autors Ian J. Bertolacci

\brief
Contains an ordered sequence of loop nests.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "LoopChain.hpp"
#include "algorithm"

using namespace LoopChainIR;

LoopChain::LoopChain()
  : chain()
  { }

LoopChain::LoopChain( const LoopChain& chain )
  : chain(chain.chain)
  { }

void LoopChain::append( LoopNest nest ){
  this->chain.push_back( nest );
}

LoopNest& LoopChain::getNest( LoopChain::size_type index ){
  return this->chain[index];
}

LoopChain::size_type LoopChain::length(){
  return this->chain.size();
}

RectangularDomain::size_type LoopChain::maxDimension(){
  RectangularDomain::size_type maximum = 0;

  for( std::vector<LoopNest>::iterator iter = this->chain.begin(); iter != this->chain.end(); iter++ ){
    maximum = std::max( (*iter).getDomain().dimensions(), maximum );
  }

  return maximum;
}

LoopChain::iterator LoopChain::begin(){
  return this->chain.begin();
}

LoopChain::const_iterator LoopChain::begin() const{
  return this->chain.begin();
}

LoopChain::iterator LoopChain::end(){
  return this->chain.end();
}

LoopChain::const_iterator LoopChain::end() const{
  return this->chain.end();
}
