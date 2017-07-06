/*! ****************************************************************************
\file ParallelAnnotation_test.cpp
\authors Ian J. Bertolacci

\brief
To perform unit testing on the ParallelAnnotation code generator.

\copyright
Copyright 2015-2016 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include <LoopChainIR/ParallelAnnotation.hpp>
#include <LoopChainIR/Schedule.hpp>
#include <LoopChainIR/TileTransformation.hpp>
#include <LoopChainIR/DefaultSequentialTransformation.hpp>
#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;

TEST( ParallelAnnotation_test, 1N_1D ){
  LoopChain chain;

  chain.append(
    LoopNest(
      RectangularDomain(
        { make_pair("0", "N") },
        {"N"}
      )
    )
  );

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers = { new ParallelAnnotation() };

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST( ParallelAnnotation_test, 1N_2D ){
  LoopChain chain;

  chain.append(
    LoopNest(
      RectangularDomain(
        { make_pair("0", "N"), make_pair("0", "M") },
        {"N", "M"}
      )
    )
  );

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers = { new ParallelAnnotation() };

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST( ParallelAnnotation_test, 2N_2D ){
  LoopChain chain;

  chain.append(
    LoopNest(
      RectangularDomain(
        { make_pair("0", "N"), make_pair("0", "M") },
        {"N", "M"}
      )
    )
  );

  chain.append(
    LoopNest(
      RectangularDomain(
        { make_pair("0", "N"), make_pair("0", "M") },
        {"N", "M"}
      )
    )
  );

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers = { new ParallelAnnotation() };

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST( ParallelAnnotation_test, 1N_2D_tile_parallel_over ){
  LoopChain chain;

  chain.append(
    LoopNest(
      RectangularDomain(
        { make_pair("0", "N"), make_pair("0", "M") },
        {"N", "M"}
      )
    )
  );

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers = {
    new TileTransformation(
      0,
      { make_pair( 0, "8" ), make_pair( 0, "8" ) },
      new ParallelAnnotation(),
      new DefaultSequentialTransformation()
    )
  };

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST( ParallelAnnotation_test, 1N_2D_tile_parallel_within ){
  LoopChain chain;

  chain.append(
    LoopNest(
      RectangularDomain(
        { make_pair("0", "N"), make_pair("0", "M") },
        {"N", "M"}
      )
    )
  );

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers = {
    new TileTransformation(
      0,
      { make_pair( 0, "8" ), make_pair( 0, "8" ) },
      new DefaultSequentialTransformation(),
      new ParallelAnnotation()
    )
  };

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}


TEST( ParallelAnnotation_test, 2N_2D_tile_parallel_over ){
  LoopChain chain;

  chain.append(
    LoopNest(
      RectangularDomain(
        { make_pair("0", "N"), make_pair("0", "M") },
        {"N", "M"}
      )
    )
  );

  chain.append(
    LoopNest(
      RectangularDomain(
        { make_pair("0", "N"), make_pair("0", "M") },
        {"N", "M"}
      )
    )
  );

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers = {
    new TileTransformation(
      0,
      { make_pair( 0, "8" ), make_pair( 0, "8" ) },
      new ParallelAnnotation(),
      new DefaultSequentialTransformation()
    ),
    new TileTransformation(
      1,
      { make_pair( 0, "8" ), make_pair( 0, "8" ) },
      new ParallelAnnotation(),
      new DefaultSequentialTransformation()
    )
  };

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST( ParallelAnnotation_test, 2N_2D_tile_parallel_within ){
  LoopChain chain;

  chain.append(
    LoopNest(
      RectangularDomain(
        { make_pair("0", "N"), make_pair("0", "M") },
        {"N", "M"}
      )
    )
  );

  chain.append(
    LoopNest(
      RectangularDomain(
        { make_pair("0", "N"), make_pair("0", "M") },
        {"N", "M"}
      )
    )
  );

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers = {
    new TileTransformation(
      0,
      { make_pair( 0, "8" ), make_pair( 0, "8" ) },
      new DefaultSequentialTransformation(),
      new ParallelAnnotation()
    ),
    new TileTransformation(
      1,
      { make_pair( 0, "8" ), make_pair( 0, "8" ) },
      new DefaultSequentialTransformation(),
      new ParallelAnnotation()
    )
  };

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}
