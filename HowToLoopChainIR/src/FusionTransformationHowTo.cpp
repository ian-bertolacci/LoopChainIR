/******************************************************************************
How To Schedule
Recommended Reading:
+ ScheduleHowTo.cpp
******************************************************************************/
#include <LoopChainIR/FusionTransformation.hpp>
#include <iostream>
#include <utility>
#include <vector>


using namespace std;
using namespace LoopChainIR;

int main(){
  /*
  A fusion transformation takes two or more loops in a chain and fuses them into
  one, with the loop body the concationation of the loop bodies in the order
  that they originally appeared in the loop chain.

  Lets demonstrate the fusion of two loops within a simple 3N_1D loop chain.
  */
  {
    cout << "Simple 3N_1D loop example" << endl;

    LoopChain chain;
    string lower[1] = {"0"};
    string upper[1] = {"N"};
    string symbol[1] = {"N"};
    // Loop 1
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
    // Loop 2
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
    // Loop 3
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
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
    [N] -> {statement_0[idx_0] : 0 <= idx_0 <= N}
    [N] -> {statement_1[idx_0] : 0 <= idx_0 <= N}
    [N] -> {statement_2[idx_0] : 0 <= idx_0 <= N}

    Transformations:
    {statement_0[idx_0] -> [0,idx_0,0]; statement_1[idx_0] -> [1,idx_0,0]; statement_2[idx_0] -> [2,idx_0,0]; }

    Default scheduled code:
    {
      for (int c1 = 0; c1 <= N; c1 += 1)
        statement_0(c1);
      for (int c1 = 0; c1 <= N; c1 += 1)
        statement_1(c1);
      for (int c1 = 0; c1 <= N; c1 += 1)
        statement_2(c1);
    }

    */

    // Create our Transformations vector
    vector<Transformation*> transformations;

    /*
    Now we can create the FusionTransformation.
    First we create a list of loops we want to fuse.
    The vector is of LoopChain::size_type, which are integers that index loops
    in the chain.
    We want to fuse loops 0 and 1.
    The order is important, since we want the body of loop 0 to come before the
    body of loop 1.
    */
    vector<LoopChain::size_type> fuse_these;
    fuse_these.push_back( (LoopChain::size_type) 0 );
    fuse_these.push_back( (LoopChain::size_type) 1 );
    // Create a FusionTransformation object using the list of loops.
    transformations.push_back( new FusionTransformation( fuse_these ) );

    // Now we apply the transformation
    sched.apply( transformations );

    // Print everything
    cout << "After Fusion Transformation:\n"
         << "Schedule state:\n" << sched
         << "\nFusion scheduled code:\n" << sched.codegen()
         << endl;
    /*
    After Fusion Transformation:
    Schedule state:
    Domains:
    [N] -> {statement_0[idx_0] : 0 <= idx_0 <= N}
    [N] -> {statement_1[idx_0] : 0 <= idx_0 <= N}
    [N] -> {statement_2[idx_0] : 0 <= idx_0 <= N}

    Transformations:
    {statement_0[idx_0] -> [0,idx_0,0]; statement_1[idx_0] -> [1,idx_0,0]; statement_2[idx_0] -> [2,idx_0,0]; }
    {
    [f,i1,i2] -> [t,i1,f] : ( t = 0 ) and (f = 0 or f = 1);
    [f,i1,i2] -> [f,i1,i2] : !(f = 0 or f = 1)
    };

    Fusion scheduled code:
    {
      for (int c1 = 0; c1 <= N; c1 += 1) {
        statement_0(c1);
        statement_1(c1);
      }
      for (int c1 = 0; c1 <= N; c1 += 1)
        statement_2(c1);
    }

    Notice, the domains did not change, and we we still have the transformations
    that gives our statements full names, but we also have the transformations
      [f,i,ii] -> [t,i,f] : ( t = 0 ) and (f = 0 or f = 1)
      [f,i,ii] -> [f,i,ii] : !(f = 0 or f = 1)

    The first transformation is what does the fusion.
      [f,i,ii] -> [t,i,f] : ( t = 0 ) and (f = 0 or f = 1)
    f and t, can be through of as 'from' and 'to'
    both loops 0 and 1 are going from their loops to being loop 0
    thats the [f] -> [t] part.
    However we also need to order the statements in the new loop body.
    thats the [f,i,ii] -> [t,i,f] part.
    The index 'i' stays the same, but 'ii' (the position in the loop body) is
    now dictated by the original loop they were in.
    So the body of loop 0 (f=0) is now statement 0 in the new loop body:
      [0,i,0] -> [0,i,0]
    and the body of loop 1 (f=1) is now statement 1 in the new loop body:
      [1,i,0] -> [0,i,1]

    The second transformation
      [f,i,ii] -> [f,i,ii] : !(f = 0 or f = 1)
    is to pass through loops that are not being fused.
    If any loop (f) is not loop 0 or 1, then the statement keeps the same name.

    */
  }

  {
    /*
    We can also fuse loops that dont have the exact same domain.
    */
    cout << "=====================================\n"
         << "Dissimilar 2N_1D loop example" << endl;

    LoopChain chain;
    {
      string lower[1] = {"0"};
      string upper[1] = {"N"};
      string symbol[1] = {"N"};
      // Loop 1
      chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
    }
    {
      string lower[1] = {"-10"};
      string upper[1] = {"M"};
      string symbol[1] = {"M"};
      // Loop 1
      chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
    }
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
    [N] -> {statement_0[idx_0] : 0 <= idx_0 <= N}
    [M] -> {statement_1[idx_0] : -10 <= idx_0 <= M}

    Transformations:
    {statement_0[idx_0] -> [0,idx_0,0]; statement_1[idx_0] -> [1,idx_0,0]; }

    Default scheduled code:
    {
      for (int c1 = 0; c1 <= N; c1 += 1)
        statement_0(c1);
      for (int c1 = -10; c1 <= M; c1 += 1)
        statement_1(c1);
    }
    */

    // Create our Transformations vector
    vector<Transformation*> transformations;

    // Fuse the two loops
    vector<LoopChain::size_type> fuse_these;
    fuse_these.push_back( (LoopChain::size_type) 0 );
    fuse_these.push_back( (LoopChain::size_type) 1 );
    // Create a FusionTransformation object using the list of loops.
    transformations.push_back( new FusionTransformation( fuse_these ) );

    // Now we apply the transformation
    sched.apply( transformations );

    // Print everything
    cout << "After Fusion Transformation:\n"
         << "Schedule state:\n" << sched
         << "\nFusion scheduled code:\n" << sched.codegen()
         << endl;
    /*
    After Fusion Transformation:
    Schedule state:
    Domains:
    [N] -> {statement_0[idx_0] : 0 <= idx_0 <= N}
    [M] -> {statement_1[idx_0] : -10 <= idx_0 <= M}

    Transformations:
    {statement_0[idx_0] -> [0,idx_0,0]; statement_1[idx_0] -> [1,idx_0,0]; }
    {
    [f,i1,i2] -> [t,i1,f] : ( t = 0 ) and (f = 0 or f = 1);
    [f,i1,i2] -> [f,i1,i2] : !(f = 0 or f = 1)
    };

    Fusion scheduled code:
    if (M <= -11) {
      for (int c1 = 0; c1 <= N; c1 += 1)
        statement_0(c1);
    } else {
      for (int c1 = -10; c1 <= M; c1 += 1) {
        if (c1 >= 0 && N >= c1)
          statement_0(c1);
        statement_1(c1);
      }
      for (int c1 = max(0, M + 1); c1 <= N; c1 += 1)
        statement_0(c1);
    }

    Notice, none of our transformations changed.
    But ISL was able to schedule the code and fuse the loops with conditionals.
    */
  }

}
