/*! ****************************************************************************
\file TileTransformation_test.cpp
\autors Ian J. Bertolacci

\brief
To perform unit testing on the TileTransformation code generator.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "TileTransformation.hpp"
#include <iostream>
#include <utility>

using namespace std;


TEST(TileTransformationTest, GEN_1N_1D_Empty_Loop) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "0" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  schedulers.push_back( new TileTransformation(0, "10") );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_EQ( sched.codegen(), string("{\n}\n") );
}


TEST(TileTransformationTest, GEN_1N_1D) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  schedulers.push_back( new TileTransformation(0, "10") );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST(TileTransformationTest, GEN_2N_1D_0) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  schedulers.push_back( new TileTransformation(0, "10") );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST(TileTransformationTest, GEN_2N_1D_1) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  schedulers.push_back( new TileTransformation(1, "10") );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST(TileTransformationTest, GEN_1N_2D) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[2] = { "1", "1" };
    string upper[2] = { "10", "20" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  schedulers.push_back( new TileTransformation(0, "10") );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST(TileTransformationTest, GEN_2N_2D_0) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[2] = { "1", "1" };
    string upper[2] = { "10", "20" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
  }

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[2] = { "1", "1" };
    string upper[2] = { "10", "20" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  schedulers.push_back( new TileTransformation(0, "10") );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST(TileTransformationTest, GEN_2N_2D_1) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[2] = { "1", "1" };
    string upper[2] = { "10", "20" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
  }

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[2] = { "1", "1" };
    string upper[2] = { "10", "20" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  schedulers.push_back( new TileTransformation(1, "10") );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST(TileTransformationTest, GEN_1N_2D_multi_extent) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[2] = { "1", "1" };
    string upper[2] = { "10", "20" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  vector<string> extents;
  extents.push_back("2");
  extents.push_back("5");

  // Append the Default Sequential transformation to the list. (It's first)
  schedulers.push_back( new TileTransformation(0, extents ) );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}
