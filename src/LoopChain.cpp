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
