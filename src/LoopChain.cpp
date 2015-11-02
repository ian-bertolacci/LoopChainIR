/*******************************************************************************
\file LoopChain.cpp
\autors Ian J. Bertolacci

\purpose
Contains an ordered sequence of loop nests.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "LoopChain.h"

LoopChain::LoopChain(): chain() {

}

void LoopChain::append( LoopNest nest ){
  this->chain.push_back( nest );
}

LoopNest& LoopChain::getNest( size_type index ){
  return this->chain[index];
}

LoopChain::size_type LoopChain::length(){
  return this->chain.size();
}
