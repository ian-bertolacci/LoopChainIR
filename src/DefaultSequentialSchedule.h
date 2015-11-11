#ifndef DEFAULT_SEQUENTIAL_SCHEDULE_H
#define DEFAULT_SEQUENTIAL_SCHEDULE_H

#define UPPER_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER_CHARS "abcdefghijklmnopqrstuvwxyz"
#define ALPHA_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

#include "LoopChain.h"
#include "all_isl.h"
#include "util.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "stdio.h"

class DefaultSequentialSchedule {
private:
  LoopChain chain;

public:
  DefaultSequentialSchedule( LoopChain& chain );

  LoopChain& getChain();

  void codegen( FILE* output_file );
};


#endif
