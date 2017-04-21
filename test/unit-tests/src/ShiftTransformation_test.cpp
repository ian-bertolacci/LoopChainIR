/*! ****************************************************************************
\file ShiftTransformation_test.cpp
\authors Ian J. Bertolacci

\brief
To perform unit testsing on the ShiftTransformation code generator.

\copyright
Copyright 2015-2016 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "ShiftTransformation.hpp"
#include <iostream>
#include <utility>


using namespace std;
using namespace LoopChainIR;

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
  // In this example, only the ShiftTransformation
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
      string lower[1] = { "1" };
    string upper[1] = { "1" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
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
  // In this example, only the ShiftTransformation
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
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
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
  // In this example, only the ShiftTransformation
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
  // In this example, only the ShiftTransformation
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
    string lower[1] = { "1" };
    string upper[1] = { "1" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
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
  // In this example, only the ShiftTransformation
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
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
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
  // In this example, only the ShiftTransformation
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
  // In this example, only the ShiftTransformation
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
    string lower[1] = { "1" };
    string upper[1] = { "1" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
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
  // In this example, only the ShiftTransformation
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
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
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
  // In this example, only the ShiftTransformation
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
  // In this example, only the ShiftTransformation
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
    string lower[1] = { "1" };
    string upper[1] = { "1" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
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
  // In this example, only the ShiftTransformation
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
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
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
  // In this example, only the ShiftTransformation
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
    string lower[2] = { "1", "1" };
    string upper[2] = { "N", "1" };
    string symbols[2] = { "N", "M" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
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
  // In this example, only the ShiftTransformation
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
  // In this example, only the ShiftTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}


/*
Test assertions
*/

/*
Apply shift with fewer extents than the dimensionality of the domain
*/
TEST(ShiftTransformationTest, Assertion_Test_extents_lt_domain) {
  LoopChain chain;

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "N", "1" };
    string symbols[2] = { "N", "M" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  vector<string> extents;
  extents.push_back( "0" );
  // extents.push_back( "1" ); // This is a purposeful error
  // Constructor does not throw exception, that happens in application
  schedulers.push_back( new ShiftTransformation(0, extents) );

  Schedule sched( chain );
  // Exception happens here
  ASSERT_THROW( sched.apply( schedulers ), assert_exception );
}

/*
Test assertions
*/

/*
Apply shift with more extents than the dimensionality of the domain
*/
TEST(ShiftTransformationTest, Assertion_Test_extents_gt_domain) {
  LoopChain chain;

  {
    string lower[2] = { "1", "1" };
    string upper[2] = { "N", "1" };
    string symbols[2] = { "N", "M" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  vector<string> extents;
  extents.push_back( "0" );
  extents.push_back( "1" );
  extents.push_back( "2" ); // This is a purposeful error
  // Constructor does not throw exception, that happens in application
  schedulers.push_back( new ShiftTransformation(0, extents) );

  Schedule sched( chain );
  // Exception happens here
  ASSERT_THROW( sched.apply( schedulers ), assert_exception );
}
