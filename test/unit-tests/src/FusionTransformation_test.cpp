/*! ****************************************************************************
\file FusionTransformation_test.cpp
\autors Ian J. Bertolacci

\brief
To perform unit testsing on the DefaultSequentialTransformation code generator.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "FusionTransformation.hpp"
#include <iostream>
#include <utility>

using namespace std;

TEST(FusionTransformationTest, Fuse_2N_1D) {
  LoopChain chain;

  {
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  {
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  vector<Transformation*> schedulers;

  vector<LoopChain::size_type> fuse_these;
  fuse_these.push_back( (LoopChain::size_type) 0 );
  fuse_these.push_back( (LoopChain::size_type) 1 );

  schedulers.push_back( new FusionTransformation( fuse_these ) );

  Schedule sched( chain );
  sched.apply( schedulers );

  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}
