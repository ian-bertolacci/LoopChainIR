/*******************************************************************************
\file LoopChain_test.cpp
\autors Ian J. Bertolacci

\purpose
To perform unit testsing on the LoopChain data structure.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "LoopChain.h"
#include <iostream>
#include <utility>

using namespace std;

TEST(LoopChainTest, FourChain) {
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
