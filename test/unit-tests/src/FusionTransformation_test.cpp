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

/*
Apply FusionTransformation to a 2N_1D loop
*/
TEST(FusionTransformationTest, Fuse_2N_1D) {
  // Create loop chain
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

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;

  // Create an ordered list of loops to fuse
  // In this case we are fusing loops 0 and 1.
  // This will result in both loop bodies residing in a single loop, however
  // the body of loop 0 will come before the body of loop 1
  vector<LoopChain::size_type> fuse_these;
  fuse_these.push_back( (LoopChain::size_type) 0 );
  fuse_these.push_back( (LoopChain::size_type) 1 );

  // Create a FusionTransformation object, passing it the list of loops to fuse
  schedulers.push_back( new FusionTransformation( fuse_these ) );

  // Create schedule from loop cahin
  Schedule sched( chain );
  // Apply FusionTransformation
  sched.apply( schedulers );

  // Test
  // Should not result in empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}
