#ifndef LOOPCHAIN_H
#define LOOPCHAIN_H

#include "LoopNest.h"

class LoopChain {
private:
  std::vector<LoopNest> chain;

public:
  typedef std::vector<LoopNest>::size_type size_type;

  LoopChain();

  void append( LoopNest nest );
  LoopNest& getNest( size_type index );
  size_type length();

};

#endif
