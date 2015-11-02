#ifndef LOOPNEST_H
#define LOOPNEST_H

#include "Box.h"

class LoopNest{
private:
  Box bounds;

public:
  LoopNest( Box loop_bounds );
  Box& getBounds();
};

#endif
