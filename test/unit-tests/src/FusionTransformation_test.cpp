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
using namespace LoopChainIR;

/*
Apply FusionTransformation to a 2N_1D loop
*/
TEST(FusionTransformationTest, Fuse_all_2N_1D) {
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

/*
Apply FusionTransformation to a 2N_2D loop
*/
TEST(FusionTransformationTest, Fuse_all_2N_2D) {
  // Create loop chain
  LoopChain chain;

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "N", "100" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
  }

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "M", "67" };
    string symbols[1] = { "M" };
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

/*
Apply FusionTransformation to a 3N_1D loop
*/
TEST(FusionTransformationTest, Fuse_2_3N_1D) {
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

/*
Apply FusionTransformation to a 3N_2D loop
*/
TEST(FusionTransformationTest, Fuse_2_3N_2D) {
  // Create loop chain
  LoopChain chain;

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "N", "100" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
  }

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "M", "67" };
    string symbols[1] = { "M" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "M", "67" };
    string symbols[1] = { "M" };
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

/*
Apply FusionTransformation to a 3N_1D loop
*/
TEST(FusionTransformationTest, Fuse_3_3N_1D) {
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
  fuse_these.push_back( (LoopChain::size_type) 2 );

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

/*
Apply FusionTransformation to a 3N_2D loop
*/
TEST(FusionTransformationTest, Fuse_3_3N_2D) {
  // Create loop chain
  LoopChain chain;

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "N", "100" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
  }

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "M", "67" };
    string symbols[1] = { "M" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "M", "67" };
    string symbols[1] = { "M" };
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
  fuse_these.push_back( (LoopChain::size_type) 2 );

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
