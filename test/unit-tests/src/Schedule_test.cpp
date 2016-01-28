/*******************************************************************************
\file Schedule_test.cpp
\autors Ian J. Bertolacci

\purpose
To perform unit testsing on the Schedule class generator.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "Schedule.hpp"
#include <iostream>
#include <utility>

using namespace std;

TEST(ScheduleTest, Construct_1N_1D_chain) {
  LoopChain chain;

  string lower[1] = {"0"};
  string upper[1] = {"N"};
  string symbol[1] = {"N"};

  RectangularDomain domain( lower, upper, 1, symbol, 1 );

  chain.append( LoopNest( domain ) );

  Schedule sched( chain );

  cout << sched << endl;
  cout << sched.codegen() << endl;
}

TEST(ScheduleTest, Construct_2N_1D_2D_chain) {
  LoopChain chain;

  {
  string lower[1] = {"0"};
  string upper[1] = {"N"};
  string symbol[1] = {"N"};

  RectangularDomain domain( lower, upper, 1, symbol, 1 );

  chain.append( LoopNest( domain ) );
  }
  {
  string lower[2] = {"1","1"};
  string upper[2] = {"M","M"};
  string symbol[1] = {"M"};

  RectangularDomain domain( lower, upper, 2, symbol, 1 );

  chain.append( LoopNest( domain ) );
  }

  Schedule sched( chain );

  cout << sched << endl;
  cout << sched.codegen() << endl;
}
