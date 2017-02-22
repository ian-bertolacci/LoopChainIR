/*! ****************************************************************************
\file TileTransformation_test.cpp
\authors Ian J. Bertolacci

\brief
To perform unit testing on the TileTransformation code generator.

\copyright
Copyright 2015-2016 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "TileTransformation.hpp"
#include "ShiftTransformation.hpp"
#include "DefaultSequentialTransformation.hpp"
#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;

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

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  schedulers.push_back( new TileTransformation(0, extents) );

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

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  schedulers.push_back( new TileTransformation(0, extents) );

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

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  schedulers.push_back( new TileTransformation(0, extents) );

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

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  schedulers.push_back( new TileTransformation(1, extents) );

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

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  extents[2] = "10";
  schedulers.push_back( new TileTransformation(0, extents) );

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

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  extents[2] = "10";
  schedulers.push_back( new TileTransformation(0, extents) );

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

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  extents[2] = "10";
  schedulers.push_back( new TileTransformation(1, extents) );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST(TileTransformationTest, GEN_2N_2D_2) {
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

  // Create tiling extents
  {
    TileTransformation::TileMap extents;
    extents[1] = "10";
    extents[2] = "10";
    schedulers.push_back( new TileTransformation(0, extents) );
  }
  {
    TileTransformation::TileMap extents;
    extents[1] = "10";
    extents[2] = "10";
    schedulers.push_back( new TileTransformation(1, extents) );
  }
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

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "2";
  extents[2] = "5";

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

TEST(TileTransformationTest, GEN_1N_2D_fewer_extents) {
  LoopChain chain;

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "N", "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "12345";

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

TEST( TileTransformation_test, 1N_1D_shift_within ){
  LoopChain chain;

  {
    string lower[1] = { "1" };
    string upper[1] = { "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  schedulers.push_back( new TileTransformation(0, extents, new DefaultSequentialTransformation(), new ShiftTransformation( 0, "5" ) ) );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}


TEST( TileTransformation_test, 1N_1D_shift_over ){
  LoopChain chain;

  {
    string lower[1] = { "1" };
    string upper[1] = { "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  schedulers.push_back( new TileTransformation(0, extents, new ShiftTransformation( 0, "5" ), new DefaultSequentialTransformation() ) );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST( TileTransformation_test, 1N_1D_shift_both ){
  LoopChain chain;

  {
    string lower[1] = { "1" };
    string upper[1] = { "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  schedulers.push_back( new TileTransformation(0, extents, new ShiftTransformation( 0, "5" ), new ShiftTransformation( 0, "5" ) ) );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST( TileTransformation_test, 2N_1D_tile_one_shift_within ){
  LoopChain chain;

  {
    string lower[1] = { "1" };
    string upper[1] = { "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }
  {
    string lower[1] = { "1" };
    string upper[1] = { "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;

  // Create tiling extents
  TileTransformation::TileMap extents;
  extents[1] = "10";
  schedulers.push_back( new TileTransformation(0, extents, new DefaultSequentialTransformation(), new ShiftTransformation( 0, "5" ) ) );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST( TileTransformation_test, 1N_2D_nested_1 ){
  LoopChain chain;

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "10", "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;

  // Create tiling extents
  TileTransformation::TileMap extents_1, extents_2;
  extents_1[1] = "2";
  extents_1[2] = "2";

  extents_2[1] = "2";

  schedulers.push_back(
    new TileTransformation(0, extents_1,
      new TileTransformation( 0, extents_2),
      new DefaultSequentialTransformation()
    )
  );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST( TileTransformation_test, 1N_2D_nested_2 ){
  LoopChain chain;

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "10", "10" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;

  // Create tiling extents
  TileTransformation::TileMap extents_1, extents_2;
  extents_1[1] = "2";
  extents_1[2] = "2";

  extents_2[1] = "4";

  schedulers.push_back(
    new TileTransformation(0, extents_2,
      new DefaultSequentialTransformation(),
      new TileTransformation( 0, extents_1 )
    )
  );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}
