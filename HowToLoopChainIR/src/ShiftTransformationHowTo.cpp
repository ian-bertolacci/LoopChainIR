/******************************************************************************
How To Schedule
Recommended Reading:
+ ScheduleHowTo.cpp
******************************************************************************/
#include <LoopChainIR/ShiftTransformation.hpp>
#include <iostream>
#include <utility>
#include <vector>


using namespace std;
using namespace LoopChainIR;

int main(){
  /*
  A shift transformation shifts the domain of a loop by some extent
  (or, in the case of multi-dimensional domains, by a set of extents)

  Lets demonstrate the shifting of a loop by a numeric constant
  */
  {
    cout << "Simple 1N_1D loop example" << endl;
    LoopChain chain;

    {
      string lower[1] = { "1" };
      string upper[1] = { "N" };
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

    Transformations:
    {statement_0[idx_0] -> [0,idx_0,0]; }

    Default scheduled code:
    for (int c1 = 1; c1 <= N; c1 += 1)
      statement_0(c1);
    */

    // Create an ordered list of Transformations
    vector<Transformation*> transformations;
    /*
    Now we can create our ShiftTransformation.
    In this case we are only shifting by a single extent (since its a 1D domain)
    so we don't need to create a vector of extents (something we will see later)
    In this example we are shifting the 0'th (and only) loop by 5.
    */
    ShiftTransformation transformation(0, "5");
    transformations.push_back( &transformation );

    // Apply the transformations
    sched.apply( transformations );

    // Print everything
    cout << "After Shift Transformation:\n"
         << "Schedule state:\n" << sched
         << "\nShift scheduled code:\n" << sched.codegen()
         << endl;

    /*
    After Shift Transformation:
    Schedule state:
    Domains:
    [N] -> {statement_0[idx_0] : 1 <= idx_0 <= N}

    Transformations:
    {statement_0[idx_0] -> [0,idx_0,0]; }
    {
    [t,i0,i1]->[t,i0+(5),i1] : (t = 0);
    [t,i0,i1]->[t,i0,i1] : (t != 0);
    }

    Shift scheduled code:
    for (int c1 = 6; c1 <= N + 5; c1 += 1)
      statement_0(c1 - 5);


    Notice the domains did not change,  and we we still have the transformations
    that gives our statements full names, but we also have the transformations
      [t,i0,i1]->[t,i0+(5),i1] : (t = 0);
      [t,i0,i1]->[t,i0,i1] : (t != 0);

    The first transformation is what does our shift.
      [t,i0,i1]->[t,i0+(5),i1] : (t = 0)
    't' is the loop, and our shift expression is only applied when 't' is our
    target loop (t = 0).

    The second transformation
      [t,i0,i1]->[t,i0,i1] : (t != 0)
    is the Identity transformation that allows all other loops not targeted
    by our transformation to be un-modified and still present in our loop chain.
    In this case, it doesn't do much since we only have one loop in our chain.

    The resulting loop has two changes:
    1. Its iteration space is shifted forward by 5 (from 1..N to 1+5..N+5)
    2. Its statement call is shifted backward by 5 (from c1 to c1-5)
    Thus the statement will iterate over a different domain, but utilizes the
    original domains indices.
    */
  }

  /*
  Lets try shifting an multi-dimensional loop that's embedded in a longer chain
  by a symbolic value
  */

  {
    cout << "=====================================\n"
         << "Simple 3N_1D_2D_3D loop example" << endl;
    LoopChain chain;

    {
      string lower[1] = { "1" };
      string upper[1] = { "N" };
      string symbols[1] = { "N" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
    }

    {
      string lower[2] = { "1", "1" };
      string upper[2] = { "M", "N" };
      string symbols[2] = { "M", "N" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
    }

    {
      string lower[3] = { "1", "1", "1" };
      string upper[3] = { "M", "N", "M" };
      string symbols[2] = { "M", "N" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 3, symbols, 2 ) ) );
    }
    // Create Schedule object from chain
    Schedule sched( chain );

    // Print everything
    cout << "Before Transformation:\n"
         << "Schedule state:\n" << sched
         << "\nDefault scheduled code:\n" << sched.codegen()
         << "-------------------------------" << endl;

    /*
    Before Transformation:
    Schedule state:
    Domains:
    [N] -> {statement_0[idx_0] : 1 <= idx_0 <= N}
    [M,N] -> {statement_1[idx_0,idx_1] : 1 <= idx_0 <= M and 1 <= idx_1 <= N}
    [M,N] -> {statement_2[idx_0,idx_1,idx_2] : 1 <= idx_0 <= M and 1 <= idx_1 <= N and 1 <= idx_2 <= M}

    Transformations:
    {statement_0[idx_0] -> [0,idx_0,0,0,0]; statement_1[idx_0,idx_1] -> [1,idx_0,idx_1,0,0]; statement_2[idx_0,idx_1,idx_2] -> [2,idx_0,idx_1,idx_2,0]; }

    Default scheduled code:
    {
      for (int c1 = 1; c1 <= N; c1 += 1)
        statement_0(c1);
      for (int c1 = 1; c1 <= M; c1 += 1)
        for (int c2 = 1; c2 <= N; c2 += 1)
          statement_1(c1, c2);
      for (int c1 = 1; c1 <= M; c1 += 1)
        for (int c2 = 1; c2 <= N; c2 += 1)
          for (int c3 = 1; c3 <= M; c3 += 1)
            statement_2(c1, c2, c3);
    }
    */

    // Create an ordered list of Transformations
    vector<Transformation*> transformations;
    /*
    Now we can create our ShiftTransformation.
    Lets transform loop 1 (the 2D loop) by 10 in the outer direction, and K
    in the inner direction.
    Because we have multiple extents (which is dictated by the dimensionality,
    not by how many shift extents we may want) we need to create a vector thats
    contains these extents.
    */
    vector<string> extents;
    extents.push_back("10");
    extents.push_back("5");
    extents.push_back("0");

    // Create the transformation
    ShiftTransformation transformation(1, extents );
    transformations.push_back( &transformation );

    // Apply the transformations
    sched.apply( transformations );

    // Print everything
    cout << "After Shift Transformation:\n"
         << "Schedule state:\n" << sched
         << "\nShift scheduled code:\n" << sched.codegen()
         << endl;

    /*
    After Shift Transformation:
    Schedule state:
    Domains:
    [N] -> {statement_0[idx_0] : 1 <= idx_0 <= N}
    [M,N] -> {statement_1[idx_0,idx_1] : 1 <= idx_0 <= M and 1 <= idx_1 <= N}
    [M,N] -> {statement_2[idx_0,idx_1,idx_2] : 1 <= idx_0 <= M and 1 <= idx_1 <= N and 1 <= idx_2 <= M}

    Transformations:
    {statement_0[idx_0] -> [0,idx_0,0,0,0]; statement_1[idx_0,idx_1] -> [1,idx_0,idx_1,0,0]; statement_2[idx_0,idx_1,idx_2] -> [2,idx_0,idx_1,idx_2,0]; }
    [K]->{
    [t,i0,i1,i2,i3]->[t,i0+(10),i1+(K),i2,i3] : (t = 1);
    [t,i0,i1,i2,i3]->[t,i0,i1,i2,i3] : (t != 1);
    }

    Shift scheduled code:
    {
      for (int c1 = 1; c1 <= N; c1 += 1)
        statement_0(c1);
      for (int c1 = 11; c1 <= M + 10; c1 += 1)
        for (int c2 = K + 1; c2 <= N + K; c2 += 1)
          statement_1(c1 - 10, -K + c2);
      for (int c1 = 1; c1 <= M; c1 += 1)
        for (int c2 = 1; c2 <= N; c2 += 1)
          for (int c3 = 1; c3 <= M; c3 += 1)
            statement_2(c1, c2, c3);
    }

    Again we see our two shift transformation statements
      [t,i0,i1,i2,i3]->[t,i0+(10),i1+(K),i2,i3] : (t = 1)
      [t,i0,i1,i2,i3]->[t,i0,i1,i2,i3] : (t != 1)

    The result is that second loop in the chain:
      for (int c1 = 11; c1 <= M + 10; c1 += 1)
        for (int c2 = K + 1; c2 <= N + K; c2 += 1)
          statement_1(c1 - 10, -K + c2);
    Again we see that the loops (both of them) have been shifted forward by
    the extents and the statement is shifted backwards by the extents.
    */
  }

  // There are a few methods worth mentioning
  {
    cout << "=====================================\n"
         << "ShiftTransformation methods" << endl;

    // Lets create a dummy transformation that won't be applied to a schedule.
    vector<string> extents;
    extents.push_back("11");
    extents.push_back("12");

    // Create the transformation
    ShiftTransformation transformation(1, extents );

    // We can get the id of the loop we will be transforming using getLoopId()
    cout << "Loop id: " << transformation.getLoopId() << endl;
    // We can get the extents as a vector using getExtents()
    Tuple gotExtents = transformation.getExtents();
    cout << "Extents: " << gotExtents << endl;

  }

}
