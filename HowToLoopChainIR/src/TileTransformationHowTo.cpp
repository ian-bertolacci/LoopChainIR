/******************************************************************************
How To Schedule
Recommended Reading:
+ ScheduleHowTo.cpp
******************************************************************************/
#include "TileTransformation.hpp"
#include "FusionTransformation.hpp"
#include <iostream>
#include <utility>
#include <vector>

using namespace std;
using namespace LoopChainIR;

int main(){
  {
    /*
    A tile transformation takes the domain of a loop and splits it into tiles of
    some constant size.

    Lets demonstrate the tiling of a single 2D loop.
    */
    cout << "Simple 1N_2D example" << endl;
    LoopChain chain;
    {
      string lower[2] = { "1", "1" };
      string upper[2] = { "N", "N" };
      string symbols[1] = { "N" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
    }
    // Create Schedule object from chain
    Schedule sched( chain );

    // Print everything
    cout << "Before Transformation:\n"
         << "Schedule state:\n" << sched
         << "\nDefault scheduled code:\n" << sched.codegen()
         << "-------------------------------------" << endl;
    /*
    Before Transformation:
    Schedule state:
    Domains:
    [N] -> {statement_0[idx_0,idx_1] : 1 <= idx_0 <= N and 1 <= idx_1 <= N}

    Transformations:
    {statement_0[idx_0,idx_1] -> [0,idx_0,idx_1,0]; }

    Default scheduled code:
    for (int c1 = 1; c1 <= N; c1 += 1)
      for (int c2 = 1; c2 <= N; c2 += 1)
        statement_0(c1, c2);
    */

    vector<Transformation*> transformations;
    /*
    Tile loop 0 with a tile size of 10.
    The tile size _must_ be a constant integer expression.
    No symbols or evaluated expressions allowed.
    */
    TileTransformation::TileMap extents;
    extents[0] = "10";
    extents[1] = "10";
    TileTransformation transformation( 0, extents );
    transformations.push_back( &transformation );

    // Apply the transformations
    sched.apply( transformations );

    // Print everything
    cout << "After Tile Transformation:\n"
         << "Schedule state:\n" << sched
         << "\nTransformed code:\n" << sched.codegen()
         << endl;

    /*
    After Tile Transformation:
    Schedule state:
    Domains:
    [N] -> {statement_0[idx_0,idx_1] : 1 <= idx_0 <= N and 1 <= idx_1 <= N}

    Transformations:
    {statement_0[idx_0,idx_1] -> [0,idx_0,idx_1,0]; }
    {
    [l,i1,i2,i3] -> [l,t1,t2,i1,i2,i3] : (l = 0) and Exists(r1,r2 : 0 <= r1 < 10 and i1 = t1 * 10 + r1 and 0 <= r2 < 10 and i2 = t2 * 10 + r2);
    [l,i1,i2,i3] -> [l,i1,i2,i3] : !(l = 0)
    };

    Transformed code:
    for (int c1 = 0; c1 <= floord(N, 10); c1 += 1)
      for (int c2 = 0; c2 <= N / 10; c2 += 1)
        for (int c3 = max(1, 10 * c1); c3 <= min(N, 10 * c1 + 9); c3 += 1)
          for (int c4 = max(1, 10 * c2); c4 <= min(N, 10 * c2 + 9); c4 += 1)
            statement_0(c3, c4);

    The resulting loop is now a 4D nest were the outer two loops iterate over
    the tiles, and the inner two loops iterate within the tiles.
    */
  }

  {
    /*
    A tile transformation takes the domain of a loop and splits it into tiles of
    some constant size.

    Lets demonstrate the tiling of a single 2D loop.
    */
    cout << "Simple 1N_2D example" << endl;
    LoopChain chain;
    {
      string lower[2] = { "1", "1" };
      string upper[2] = { "N", "N" };
      string symbols[1] = { "N" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
    }

    {
      string lower[2] = { "1", "1" };
      string upper[2] = { "N", "N" };
      string symbols[1] = { "N" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
    }
    // Create Schedule object from chain
    Schedule sched( chain );

    // Print everything
    cout << "Before Transformation:\n"
         << "Schedule state:\n" << sched
         << "\nDefault scheduled code:\n" << sched.codegen()
         << "-------------------------------------" << endl;
    /*
    Before Transformation:
    Schedule state:
    Domains:
    [N] -> {statement_0[idx_0,idx_1] : 1 <= idx_0 <= N and 1 <= idx_1 <= N}

    Transformations:
    {statement_0[idx_0,idx_1] -> [0,idx_0,idx_1,0]; }

    Default scheduled code:
    for (int c1 = 1; c1 <= N; c1 += 1)
      for (int c2 = 1; c2 <= N; c2 += 1)
        statement_0(c1, c2);
    */

    vector<Transformation*> transformations;
    /*
    Tile loop 0 with a tile size of 10.
    The tile size _must_ be a constant integer expression.
    No symbols or evaluated expressions allowed.
    */
    vector<LoopChain::size_type> fuse_these;
    fuse_these.push_back( (LoopChain::size_type) 0 );
    fuse_these.push_back( (LoopChain::size_type) 1 );
    // Create a FusionTransformation object using the list of loops.
    transformations.push_back( new FusionTransformation( fuse_these ) );

    TileTransformation::TileMap extents1;
    extents1[0] = "10";
    extents1[1] = "10";
    TileTransformation transformation1( 0, extents1 );
    transformations.push_back( &transformation1 );

    // Apply the transformations
    sched.apply( transformations );

    // Print everything
    cout << "After Tile Transformation:\n"
         << "Schedule state:\n" << sched
         << "\nTransformed code:\n" << sched.codegen()
         << endl;

    /*
    After Tile Transformation:
    Schedule state:
    Domains:
    [N] -> {statement_0[idx_0,idx_1] : 1 <= idx_0 <= N and 1 <= idx_1 <= N}

    Transformations:
    {statement_0[idx_0,idx_1] -> [0,idx_0,idx_1,0]; }
    {
    [l,i1,i2,i3] -> [l,t1,t2,i1,i2,i3] : (l = 0) and Exists(r1,r2 : 0 <= r1 < 10 and i1 = t1 * 10 + r1 and 0 <= r2 < 10 and i2 = t2 * 10 + r2);
    [l,i1,i2,i3] -> [l,i1,i2,i3] : !(l = 0)
    };

    Transformed code:
    for (int c1 = 0; c1 <= floord(N, 10); c1 += 1)
      for (int c2 = 0; c2 <= N / 10; c2 += 1)
        for (int c3 = max(1, 10 * c1); c3 <= min(N, 10 * c1 + 9); c3 += 1)
          for (int c4 = max(1, 10 * c2); c4 <= min(N, 10 * c2 + 9); c4 += 1)
            statement_0(c3, c4);

    The resulting loop is now a 4D nest were the outer two loops iterate over
    the tiles, and the inner two loops iterate within the tiles.
    */
  }

}
