/*******************************************************************************
\file DefaultSequentialSchedule_test.cpp
\autors Ian J. Bertolacci

\purpose
To perform unit testsing on the DefaultSequentialSchedule code generator.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "DefaultSequentialSchedule.hpp"
#include <iostream>
#include <utility>

using namespace std;

TEST(DefaultSequentialScheduleTest, Construct_Get_FourChain) {
  LoopChain chain;

  string lower[1];
  string upper[1];
  string symbols[1];

  lower[0] = "0";
  upper[0] = "N";
  symbols[0] = "N";
  RectangularDomain domain_1( lower, upper, 1, symbols, 1 );

  lower[0] = "1";
  upper[0] = "M";
  symbols[0] = "M";
  RectangularDomain domain_2( lower, upper, 1, symbols, 1 );

  lower[0] = "2";
  upper[0] = "J";
  symbols[0] = "J";
  RectangularDomain domain_3( lower, upper, 1, symbols, 1 );

  lower[0] = "3";
  upper[0] = "K";
  symbols[0] = "K";
  RectangularDomain domain_4( lower, upper, 1, symbols, 1 );

  chain.append( LoopNest( domain_1 ) );
  chain.append( LoopNest( domain_2 ) );
  chain.append( LoopNest( domain_3 ) );
  chain.append( LoopNest( domain_4 ) );

  EXPECT_EQ( chain.length(), 4 );

  RectangularDomain got_domain = chain.getNest(0).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("0"));
  EXPECT_EQ( got_domain.getUpperBound(0), string("N"));

  got_domain = chain.getNest(1).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("1"));
  EXPECT_EQ( got_domain.getUpperBound(0), string("M"));

  got_domain = chain.getNest(2).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("2"));
  EXPECT_EQ( got_domain.getUpperBound(0), string("J"));

  got_domain = chain.getNest(3).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("3"));
  EXPECT_EQ( got_domain.getUpperBound(0), string("K"));
}

TEST(DefaultSequentialScheduleTest, Gen_OneChain) {
  LoopChain chain;

  string lower[1];
  string upper[1];
  string symbol[1];

  lower[0] = "0";
  upper[0] = "N";
  symbol[0] = "N";
  RectangularDomain domain( lower, upper, 1, symbol, 1 );

  chain.append( LoopNest( domain ) );

  DefaultSequentialSchedule sched( chain );

  sched.codegen( stdout );
}

TEST(DefaultSequentialScheduleTest, Gen_OneChain_TwoD) {
  LoopChain chain;

  string lower[2] = { "0", "K" };
  string upper[2] = { "N", "M" };
  string symbol[3] = { "K", "N", "M" };

  RectangularDomain domain( lower, upper, 2, symbol, 3 );

  chain.append( LoopNest( domain ) );

  DefaultSequentialSchedule sched( chain );

  sched.codegen( stdout );
}

TEST(DefaultSequentialScheduleTest, Gen_FourChain) {
  LoopChain chain;

  string lower[1];
  string upper[1];
  string symbols[1];

  lower[0] = "0";
  upper[0] = "N";
  symbols[0] = "N";
  RectangularDomain domain_1( lower, upper, 1, symbols, 1 );

  lower[0] = "1";
  upper[0] = "M";
  symbols[0] = "M";
  RectangularDomain domain_2( lower, upper, 1, symbols, 1 );

  lower[0] = "2";
  upper[0] = "J";
  symbols[0] = "J";
  RectangularDomain domain_3( lower, upper, 1, symbols, 1 );

  lower[0] = "3";
  upper[0] = "K";
  symbols[0] = "K";
  RectangularDomain domain_4( lower, upper, 1, symbols, 1 );

  chain.append( LoopNest( domain_1 ) );
  chain.append( LoopNest( domain_2 ) );
  chain.append( LoopNest( domain_3 ) );
  chain.append( LoopNest( domain_4 ) );

  DefaultSequentialSchedule sched( chain );

  sched.codegen( stdout );
}

TEST(DefaultSequentialScheduleTest, SC_Loops_Default) {
  LoopChain chain;

  string symbols[6] = { "Lx","Ux","Ly","Uy","Lz","Uz" };

  string d1_lower[3] = { "Lx", "Ly", "Lz" };
  string d1_upper[3] = { "Ux+1", "Uy", "Uz" };
  RectangularDomain domain_1( d1_lower, d1_upper, 3, symbols, 6 );

  string d2_lower[3] = { "Lx", "Ly", "Lz" };
  string d2_upper[3] = { "Ux", "Uy+1", "Uz" };
  RectangularDomain domain_2( d2_lower, d2_upper, 3, symbols, 6 );

  string d3_lower[3] = { "Lx", "Ly", "Lz" };
  string d3_upper[3] = { "Ux", "Uy", "Uz+1" };
  RectangularDomain domain_3( d3_lower, d3_upper, 3, symbols, 6 );

  string d4_lower[3] = { "Lx", "Ly", "Lz" };
  string d4_upper[3] = { "Ux", "Uy", "Uz" };
  RectangularDomain domain_4( d4_lower, d4_upper, 3, symbols, 6 );

  chain.append( LoopNest( domain_1 ) );
  chain.append( LoopNest( domain_2 ) );
  chain.append( LoopNest( domain_3 ) );
  chain.append( LoopNest( domain_4 ) );

  DefaultSequentialSchedule sched( chain );

  sched.codegen( stdout );
}
