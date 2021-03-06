/*! ****************************************************************************
\file Schedule_test.cpp
\authors Ian J. Bertolacci

\brief
To perform unit testsing on the Schedule class generator.

\copyright
Copyright 2015-2016 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include <LoopChainIR/Schedule.hpp>
#include <LoopChainIR/util.hpp>
#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;
/*
Create a Schedule from a 1N_1D chain
*/
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
  ASSERT_EQ( sched.getStatementPrefix(), "" );
  ASSERT_EQ( sched.getRootStatementSymbol(), "statement_" );
}

/*
Create a Schedule from a 2N_1D_2D chain
*/
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
  ASSERT_EQ( sched.getStatementPrefix(), "" );
  ASSERT_EQ( sched.getRootStatementSymbol(), "statement_" );
}

/*
Create a Schedule from a 1N_1D chain
*/
TEST(ScheduleTest, Custom_statement_prefix) {
  LoopChain chain;

  {
    string lower[1] = {"0"};
    string upper[1] = {"N"};
    string symbol[1] = {"N"};
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }
  std::string prefix("custom_prefix_");
  Schedule sched( chain, prefix );

  ASSERT_NE( SSTR( sched ), "" );
  ASSERT_NE( sched.codegen(), "" );
  ASSERT_EQ( sched.getStatementPrefix(), prefix );
  ASSERT_EQ( sched.getRootStatementSymbol(), SSTR(prefix << "statement_") );
  ASSERT_NE( sched.codegen().find(prefix), std::string::npos );
}
