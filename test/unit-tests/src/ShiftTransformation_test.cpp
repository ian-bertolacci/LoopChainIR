/*! ****************************************************************************
\file ShiftTransformation_test.cpp
\autors Ian J. Bertolacci

\brief
To perform unit testsing on the ShiftTransformation code generator.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "ShiftTransformation.hpp"
#include <iostream>
#include <utility>

using namespace std;

/*
Apply a shift of 0
*/

/*
Apply shift of 0 to a loop with no iterations
*/
TEST(ShiftTransformationTest, GEN_1N_1D_0_Empty_Loop) {
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
  ShiftTransformation transformation(0, "0");
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"0"); // one element of '0'
  vector<string> test_symbols;
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_EQ( sched.codegen(), string("{\n}\n") );
}


/*
Apply shift of 0 to a loop with one iteration
*/
TEST(ShiftTransformationTest, GEN_1N_1D_Shift_0_Single_Iteration) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "1" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  ShiftTransformation transformation(0, "0");
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"0"); // one element of '0'
  vector<string> test_symbols;
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

/*
Apply shift of 0 to a loop with N iterations
*/
TEST(ShiftTransformationTest, GEN_1N_1D_Shift_0_N_Iterations) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  ShiftTransformation transformation(0, "0");
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"0"); // one element of '0'
  vector<string> test_symbols;
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}


/*
Apply a shift of a positive value
*/

/*
Apply shift of 5 to a loop with no iterations
*/
TEST(ShiftTransformationTest, GEN_1N_1D_5_Empty_Loop) {
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
  ShiftTransformation transformation(0, "5");
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"5"); // one element of '0'
  vector<string> test_symbols;
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_EQ( sched.codegen(), string("{\n}\n") );
}

/*
Apply shift of 5 to a loop with one iteration
*/
TEST(ShiftTransformationTest, GEN_1N_1D_Shift_5_Single_Iteration) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "1" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  ShiftTransformation transformation(0, "5");
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"5"); // one element of '5'
  vector<string> test_symbols;
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

/*
Apply shift of 5 to a loop with N iterations
*/
TEST(ShiftTransformationTest, GEN_1N_1D_Shift_5_N_Iterations) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  ShiftTransformation transformation(0, "5");
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"5"); // one element of '0'
  vector<string> test_symbols;
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}


/*
Apply a shift of a negative value
*/

/*
Apply shift of -5 to a loop with no iterations
*/
TEST(ShiftTransformationTest, GEN_1N_1D_neg5_Empty_Loop) {
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
  ShiftTransformation transformation(0, "-5");
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"-5"); // one element of '0'
  vector<string> test_symbols;
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_EQ( sched.codegen(), string("{\n}\n") );
}

/*
Apply shift of -5 to a loop with one iteration
*/
TEST(ShiftTransformationTest, GEN_1N_1D_Shift_neg5_Single_Iteration) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "1" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  ShiftTransformation transformation(0, "-5");
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"-5"); // one element of '5'
  vector<string> test_symbols;
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

/*
Apply shift of -5 to a loop with N iterations
*/
TEST(ShiftTransformationTest, GEN_1N_1D_Shift_neg5_N_Iterations) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  ShiftTransformation transformation(0, "-5");
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"-5"); // one element of '0'
  vector<string> test_symbols;
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}


/*
Apply shift of a symbolic
*/

/*
Apply shift of K to a loop with no iterations
*/
TEST(ShiftTransformationTest, GEN_1N_1D_K_Empty_Loop) {
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
  vector<string> symbols(1, "K");
  ShiftTransformation transformation(0, "K", symbols);
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"K"); // one element of 'K'
  vector<string> test_symbols(1,"K"); // one element of 'K'
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_EQ( sched.codegen(), string("{\n}\n") );
}

/*
Apply shift of K to a loop with one iteration
*/
TEST(ShiftTransformationTest, GEN_1N_1D_Shift_K_Single_Iteration) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "1" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  vector<string> symbols(1, "K");
  ShiftTransformation transformation(0, "K", symbols);
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"K"); // one element of 'K'
  vector<string> test_symbols(1,"K"); // one element of 'K'
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

/*
Apply shift of K to a loop with N iterations
*/
TEST(ShiftTransformationTest, GEN_1N_1D_Shift_K_N_Iterations) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  vector<string> symbols(1, "K");
  ShiftTransformation transformation(0, "K", symbols);
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"K"); // one element of 'K'
  vector<string> test_symbols(1,"K"); // one element of 'K'
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

/*
Apply shift of K to a loop with NxM iterations
*/
TEST(ShiftTransformationTest, GEN_1N_2D_Shift_K_NxM_Iterations) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[2] = { "1", "1" };
    string upper[2] = { "N", "1" };
    string symbols[2] = { "N", "M" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  vector<string> symbols(1, "K");
  vector<string> extents;
  extents.push_back( "K" );
  extents.push_back( "10");
  ShiftTransformation transformation(0, extents, symbols);
  schedulers.push_back( &transformation );

  vector<string> test_extents;
  test_extents.push_back( "K" );
  test_extents.push_back( "10");
  vector<string> test_symbols(1, "K");
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

/*
Apply shift of K to a loop with N iterations
*/
TEST(ShiftTransformationTest, GEN_2N_1D_Shift_K_N_Iterations) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  vector<string> symbols(1, "K");
  ShiftTransformation transformation(0, "K", symbols);
  schedulers.push_back( &transformation );

  vector<string> test_extents(1,"K"); // one element of 'K'
  vector<string> test_symbols(1,"K"); // one element of 'K'
  ASSERT_EQ( 0, transformation.getLoopId() );
  ASSERT_EQ( test_extents, transformation.getExtents() );
  ASSERT_EQ( test_symbols, transformation.getSymbols() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}
