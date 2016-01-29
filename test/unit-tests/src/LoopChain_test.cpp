/*******************************************************************************
\file LoopChain_test.cpp
\autors Ian J. Bertolacci

\purpose
To perform unit testsing on the LoopChain data structure.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "LoopChain.hpp"
#include <iostream>
#include <utility>

using namespace std;

TEST(LoopChainTest, Test_Getters_4N_1D) {
  LoopChain chain;

  {
    string lower[1] = { "0" };
    string upper[1] = { "J" };
    string symbol[1] = { "J" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  {
    string lower[1] = { "1" };
    string upper[1] = { "K" };
    string symbol[1] = { "K" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  {
    string lower[1] = { "2" };
    string upper[1] = { "L" };
    string symbol[1] = { "L" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  {
    string lower[1] = { "3" };
    string upper[1] = { "M" };
    string symbol[1] = { "M" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  EXPECT_EQ( chain.length(), 4 );

  RectangularDomain got_domain = chain.getNest(0).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("0") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("J") );
  EXPECT_EQ( got_domain.getSymbol(0), "J" );

  got_domain = chain.getNest(1).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("1") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("K") );
  EXPECT_EQ( got_domain.getSymbol(0), "K" );


  got_domain = chain.getNest(2).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("2") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("L") );
  EXPECT_EQ( got_domain.getSymbol(0), "L" );


  got_domain = chain.getNest(3).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("3") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("M") );
  EXPECT_EQ( got_domain.getSymbol(0), "M" );
}


TEST(LoopChainTest, Test_Getters_2N_1D_2D) {
  LoopChain chain;

  {
    string lower[1] = { "0" };
    string upper[1] = { "J" };
    string symbol[1] = { "J" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  {
    string lower[2] = { "1","2" };
    string upper[2] = { "K","3" };
    string symbol[1] = { "K" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbol, 1 ) ) );
  }

  EXPECT_EQ( chain.length(), 2 );

  RectangularDomain got_domain = chain.getNest(0).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("0") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("J") );
  EXPECT_EQ( got_domain.getSymbol(0), "J" );

  got_domain = chain.getNest(1).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 2 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("1") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("K") );
  EXPECT_EQ( got_domain.getLowerBound(1), string("2") );
  EXPECT_EQ( got_domain.getUpperBound(1), string("3") );
  EXPECT_EQ( got_domain.getSymbol(0), "K" );
}

TEST(LoopChainTest, Test_Getters_2N_2D_1D) {
  LoopChain chain;

  {
    string lower[2] = { "1","2" };
    string upper[2] = { "K","3" };
    string symbol[1] = { "K" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbol, 1 ) ) );
  }

  {
    string lower[1] = { "0" };
    string upper[1] = { "J" };
    string symbol[1] = { "J" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  EXPECT_EQ( chain.length(), 2 );

  RectangularDomain got_domain = chain.getNest(0).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 2 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("1") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("K") );
  EXPECT_EQ( got_domain.getLowerBound(1), string("2") );
  EXPECT_EQ( got_domain.getUpperBound(1), string("3") );
  EXPECT_EQ( got_domain.getSymbol(0), "K" );

  got_domain = chain.getNest(1).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("0") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("J") );
  EXPECT_EQ( got_domain.getSymbol(0), "J" );
}

TEST(LoopChainTest, Test_Getters_3N_1D_2D_3D) {
  LoopChain chain;

  {
    string lower[1] = { "0" };
    string upper[1] = { "J" };
    string symbol[1] = { "J" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  {
    string lower[2] = { "1","2" };
    string upper[2] = { "K","3" };
    string symbol[1] = { "K" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbol, 1 ) ) );
  }

  {
    string lower[3] = { "L","M","N" };
    string upper[3] = { "K","3","5" };
    string symbol[4] = { "K","L","M","N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 3, symbol, 4 ) ) );
  }

  EXPECT_EQ( chain.length(), 3 );

  RectangularDomain got_domain = chain.getNest(0).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 1 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("0") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("J") );
  EXPECT_EQ( got_domain.getSymbol(0), "J" );

  got_domain = chain.getNest(1).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 2 );
  EXPECT_EQ( got_domain.symbolics(), 1 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("1") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("K") );
  EXPECT_EQ( got_domain.getLowerBound(1), string("2") );
  EXPECT_EQ( got_domain.getUpperBound(1), string("3") );
  EXPECT_EQ( got_domain.getSymbol(0), "K" );

  got_domain = chain.getNest(2).getDomain();

  EXPECT_EQ( got_domain.dimensions(), 3 );
  EXPECT_EQ( got_domain.symbolics(), 4 );
  EXPECT_EQ( got_domain.getLowerBound(0), string("L") );
  EXPECT_EQ( got_domain.getUpperBound(0), string("K") );
  EXPECT_EQ( got_domain.getLowerBound(1), string("M") );
  EXPECT_EQ( got_domain.getUpperBound(1), string("3") );
  EXPECT_EQ( got_domain.getLowerBound(2), string("N") );
  EXPECT_EQ( got_domain.getUpperBound(2), string("5") );
  EXPECT_EQ( got_domain.getSymbol(0), "K" );
  EXPECT_EQ( got_domain.getSymbol(1), "L" );
  EXPECT_EQ( got_domain.getSymbol(2), "M" );
  EXPECT_EQ( got_domain.getSymbol(3), "N" );
}
