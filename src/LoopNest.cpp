#include "LoopNest.h"

LoopNest::LoopNest( Box loop_bounds ): bounds( loop_bounds ){

}

Box& LoopNest::getBounds(){
  return this->bounds;
}
