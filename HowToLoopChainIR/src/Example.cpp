#include <LoopChainIR/LoopChain.hpp>
#include <LoopChainIR/LoopNest.hpp>
#include <LoopChainIR/Accesses.hpp>
#include <LoopChainIR/ShiftTransformation.hpp>
#include <LoopChainIR/FusionTransformation.hpp>
#include <LoopChainIR/AutomaticShiftTransformation.hpp>
#include <LoopChainIR/TileTransformation.hpp>

#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;

int main( int argc, char* argv[] ){
  LoopChain chain;

  chain.append(
    /*
      Create a single loop-nest encoding
      for( int i = 1; i <= N; ++i )
        B[i] = A[i-1] + A[i] + A[i+1];
    */
    LoopNest(
      // Create the nest's domain
      RectangularDomain(
        // Bounds as pairs [lower, upper].
        { make_pair( "1", "N" ) },
        // Set of symbolic constants found in any of the bounds.
        {"N"}
      ),
      // List of the nest's read and write accesses.
      // Order is unimportant.
      {
        // Encode accesses of dataspace 'A' (a.k.a. the array 'A').
        Dataspace(
          // Name
          "A",
          // Reads A[i-1], A[i], A[i+1]
          TupleCollection({ Tuple({-1}), Tuple({0}), Tuple({1}) }),
          // Writes nothing. Create an empty collection of 1-D tuples
          TupleCollection( 1 )
        ),
        // Encode accesses of dataspace 'B' (a.k.a. the array 'B').
        Dataspace(
          // Name
          "B",
          // Reads nothing. Create an empty collection of 1-D tuples
          TupleCollection( 1 ),
          // Writes B[i]
          TupleCollection({ Tuple({0}) })
        )
      }
    )
  );

  chain.append(
    /*
      Create a single loop-nest encoding
      for( int i = 1; i <= N; ++i )
        A[i] = B[i-1] + B[i] + B[i+1];
    */
    LoopNest(
      // Create the nest's domain
      RectangularDomain(
        // Bounds as pairs [lower, upper].
        { make_pair( "1", "N" ) },
        // Set of symbolic constants found in any of the bounds.
        {"N"}
      ),
      // List of the nest's read and write accesses.
      // Order is unimportant.
      {
        // Encode accesses of dataspace 'A' (a.k.a. the array 'A').
        Dataspace(
          // Name
          "A",
          // Reads nothing. Create an empty collection of 1-D tuples
          TupleCollection( 1 ),
          // Writes C[i].
          TupleCollection({ Tuple({0}) })
        ),
        // Encode accesses of dataspace 'B' (a.k.a. the array 'B').
        Dataspace(
          // Name
          "B",
          // Reads B[i-1], B[i], B[i+1]
          TupleCollection({ Tuple({-1}), Tuple({0}), Tuple({1}) }),
          // nothing. Create an empty collection of 1-D tuples
          TupleCollection( 1 )
        )
      }
    )
  );

  {
    // Create a schedule from the loop-chain
    Schedule schedule( chain );

    // Print state initial state.
    // Schedule can generate code immediately
    cout  << "========================================================" << endl
          << "Before Transfomations" << endl
          << "---------------------------------------------" << endl
          << "C Code" << endl
          << "---------------------------------------------" << endl
          << schedule.codegen() << endl
          << "---------------------------------------------" << endl
          << "ISCC code" << endl
          << "---------------------------------------------" << endl
          << schedule.codegenToISCC() << endl
          << "========================================================" << endl;

    // Create shift and fusion transfomations.
    vector<Transformation*> transformations = {
      // First create shift transformation for the second loop-nest (loop 1)
      new ShiftTransformation(
        // Loop index
        (LoopChain::size_type) 1,
        // Extent of shift is 1
        Tuple({ 1 })
      ),
      // Next create fusion transformation
      new FusionTransformation(
        // Ordered list of loop indexes to fuse.
        (vector<LoopChain::size_type>){ 0, 1 }
      )
    };

    // Apply transformations to schedule
    schedule.apply( transformations );

    // Print state initial state.
    // Schedule can generate code immediately
    cout  << "========================================================" << endl
          << "After manual-shift and fuse ransfomations" << endl
          << "---------------------------------------------" << endl
          << "C Code" << endl
          << "---------------------------------------------" << endl
          << schedule.codegen() << endl
          << "---------------------------------------------" << endl
          << "ISCC code" << endl
          << "---------------------------------------------" << endl
          << schedule.codegenToISCC() << endl
          << "========================================================" << endl;
  }

  {
    // Create a schedule from the loop-chain
    Schedule schedule( chain );

    // Create shift and fusion transfomations, and put in list.
    vector<Transformation*> transformations = {
      // First create shift transformation for the second loop-nest (loop 1)
      new AutomaticShiftTransformation(),
      // Next create fusion transformation
      new FusionTransformation(
        // Ordered list of loop indexes to fuse.
        (vector<LoopChain::size_type>){ 0, 1 }
      )
    };

    // Apply transformations to schedule
    schedule.apply( transformations );

    // Print state initial state.
    // Schedule can generate code immediately
    cout  << "========================================================" << endl
          << "After automatic-shift and fuse transfomations" << endl
          << "--------------------------------------------------------" << endl
          << "C Code" << endl
          << "--------------------------------------------------------" << endl
          << schedule.codegen() << endl
          << "--------------------------------------------------------" << endl
          << "ISCC code" << endl
          << "--------------------------------------------------------" << endl
          << schedule.codegenToISCC() << endl
          << "========================================================" << endl;
  }

  {
    // Create a schedule from the loop-chain
    Schedule schedule( chain );

    // Create shift and fusion transfomations, and put in list.
    vector<Transformation*> transformations = {
      // First create shift transformation for the second loop-nest (loop 1)
      new AutomaticShiftTransformation(),
      // Next create fusion transformation
      new FusionTransformation(
        // Ordered list of loop indexes to fuse.
        (vector<LoopChain::size_type>){ 0, 1 }
      ),
      new TileTransformation(
        // Loop id
        0,
        {
          // Tile dimension 0 of the nest by 8 iterations
          make_pair( 0, "8" )
        }
      )
    };

    // Apply transformations to schedule
    schedule.apply( transformations );

    // Print state initial state.
    // Schedule can generate code immediately
    cout  << "========================================================" << endl
          << "After automatic-shift, fuse, and tile (8) transfomations" << endl
          << "--------------------------------------------------------" << endl
          << "C Code" << endl
          << "--------------------------------------------------------" << endl
          << schedule.codegen() << endl
          << "--------------------------------------------------------" << endl
          << "ISCC code" << endl
          << "--------------------------------------------------------" << endl
          << schedule.codegenToISCC() << endl
          << "========================================================" << endl;
  }


  return 0;
}
