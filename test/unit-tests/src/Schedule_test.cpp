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
#include "util.hpp"
#include <iostream>
#include <utility>

using namespace std;

TEST(ScheduleTest, Construct_1N_1D_chain) {
  LoopChain chain;

  {
    string lower[1] = {"0"};
    string upper[1] = {"N"};
    string symbol[1] = {"N"};
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  Schedule sched( chain );

  ASSERT_NE( SSTR( sched ), "" );
  ASSERT_NE( sched.codegen(), "" );
}

TEST(ScheduleTest, Construct_2N_1D_2D_chain) {
  LoopChain chain;

  {
    string lower[1] = {"0"};
    string upper[1] = {"N"};
    string symbol[1] = {"N"};
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  {
    string lower[2] = {"1","1"};
    string upper[2] = {"M","M"};
    string symbol[1] = {"M"};
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbol, 1 ) ) );
  }

  Schedule sched( chain );

  ASSERT_NE( SSTR( sched ), "" );
  ASSERT_NE( sched.codegen(), "" );
}
