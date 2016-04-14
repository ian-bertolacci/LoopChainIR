/******************************************************************************
How To Schedule
Recommended Reading:
+ ScheduleHowTo.cpp
******************************************************************************/
#include "ShiftTransformation.hpp"
#include "FusionTransformation.hpp"
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main(){
  {
    cout << "Shift-Fuse example on 2N_1D example" << endl;
    LoopChain chain;
    {
      string lower[1] = { "1" };
      string upper[1] = { "N" };
      string symbols[1] = { "N" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
    }
    {
      // This loop is a shifted version of the previous one.
      // How handy!
      string lower[1] = { "1+9" };
      string upper[1] = { "N+9" };
      string symbols[1] = { "N" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
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
    [N] -> {statement_0[idx_0] : 1 <= idx_0 <= N}
    [N] -> {statement_1[idx_0] : 1+9 <= idx_0 <= N+9}

    Transformations:
    {statement_0[idx_0] -> [0,idx_0,0]; statement_1[idx_0] -> [1,idx_0,0]; }

    Default scheduled code:
    {
      for (int c1 = 1; c1 <= N; c1 += 1)
        statement_0(c1);
      for (int c1 = 10; c1 <= N + 9; c1 += 1)
        statement_1(c1);
    }
    */

    // Let's shift and fuse our loops.
    vector<Transformation*> transformations;
    // Create our shift transformation
    {
      // We are shifting the second loop backwards by 9
      // could also shift the first loop forwards by 9
      ShiftTransformation* transformation = new ShiftTransformation(1, "-9");
      transformations.push_back( transformation );
    }
    {
      // Fuse our two loops
      vector<LoopChain::size_type> fuse_these;
      fuse_these.push_back( (LoopChain::size_type) 0 );
      fuse_these.push_back( (LoopChain::size_type) 1 );
      FusionTransformation* transformation = new FusionTransformation( fuse_these );
      transformations.push_back( transformation );
    }

    // Apply the transformations
    sched.apply( transformations );

    // Print everything
    cout << "After Shift Transformation:\n"
         << "Schedule state:\n" << sched
         << "\nTransformed code:\n" << sched.codegen()
         << endl;

    /*
    After Shift Transformation:
    Schedule state:
    Domains:
    [N] -> {statement_0[idx_0] : 1 <= idx_0 <= N}
    [N] -> {statement_1[idx_0] : 1+9 <= idx_0 <= N+9}

    Transformations:
    {statement_0[idx_0] -> [0,idx_0,0]; statement_1[idx_0] -> [1,idx_0,0]; }
    {
    [t,i0,i1]->[t,i0+(-9),i1] : (t = 1);
    [t,i0,i1]->[t,i0,i1] : (t != 1);
    }
    {
    [f,i1,i2] -> [t,i1,f] : ( t = 0 ) and (f = 0 or f = 1);
    [f,i1,i2] -> [f,i1,i2] : !(f = 0 or f = 1)
    };

    Transformed code:
    for (int c1 = 1; c1 <= N; c1 += 1) {
      statement_0(c1);
      statement_1(c1 + 9);
    }

    You can see that our two loop statements are now within a single loop over
    the domain {1..N};
    */
  }
}
