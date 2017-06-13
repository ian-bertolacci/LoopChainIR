/******************************************************************************
How To Schedule
Recommended Reading:
+ LoopChainHowTo.cpp
******************************************************************************/
#include <LoopChainIR/Schedule.hpp>
#include <LoopChainIR/DefaultSequentialTransformation.hpp> // Not required for Schedule
#include <iostream>
#include <utility>
#include <vector>


using namespace std;
using namespace LoopChainIR;

int main(){
  /*
  A Schedule is a higher order wraper around a loop chain that can be
  transformed according to Transformation instances, and then generate C code
  for that schedule.

  First lets create a loop chain to schedule.
  */

  LoopChain chain;

  {
    string lower[1] = {"0"};
    string upper[1] = {"N"};
    string symbol[1] = {"N"};
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  {
    string lower[2] = {"1","1"};
    string upper[2] = {"N","M*2"};
    string symbol[2] = {"M","N"};
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbol, 2 ) ) );
  }

  // Now we create the schedule from the chain
  Schedule sched( chain );

  /*
  We can actually look at the underlying ISCC code that is representing the
  loops and transformations.
  */
  cout << "Schedule state:\n" << sched << endl;
  /*
  Domains:
  [N] -> {statement_0[idx_0] : 0 <= idx_0 <= N}
  [M,N] -> {statement_1[idx_0,idx_1] : 1 <= idx_0 <= N and 1 <= idx_1 <= M*2}

  Transformations:
  {statement_0[idx_0] -> [0,idx_0,0,0]; statement_1[idx_0,idx_1] -> [1,idx_0,idx_1,0]; }

  Domains are the index domains of the statements, and the Transformations are
  the functions applied to attain an ordering.

  The transformations:
    statement_0[idx_0] -> [0,idx_0,0,0]
    statement_1[idx_0,idx_1] -> [1,idx_0,idx_1,0]
  essentially place the statements into a global context.
  Instead of each statement being named by only the index of the loop, we
  also name the loop, and their position in the loop.

  This tuple is named as so:
  [ Loop position,
    index of dimension 0,
    index of dim. 1,
    ... ,
    statement position in body ]

  Heres an important bit of nomenclature:
  Statement identifier is the statement_0, statement_1 business.
  It allows us (humans) to talk about what is happening and can be though of
  like C++ macros.
  Statement name is the [i,j,k] business. It allows statements to be placed in
  and ordering along with all other statements, and also transformed.

  In fact, we can generate code from this schedule.
  */
  cout << "Default scheduled code:\n" << sched.codegen() << endl;
  /*
  {
  for (int c1 = 0; c1 <= N; c1 += 1)
    statement_0(c1);
  for (int c1 = 1; c1 <= N; c1 += 1)
    for (int c2 = 1; c2 <= 2 * M; c2 += 1)
      statement_1(c1, c2);
  }

  As a quick introduction into how to apply transformations to a schedule lets
  loop at DefaultSequentialSchedule.
  DefaultSequentialSchedule is the lamest of ducks, since it actually does not
  modify the state of the schedule, especially since we are already in the
  Default Sequential schedule state.

  First we create a vector of Transformation*.
  The order of the list is the order in which they will be applied.
  */

  vector<Transformation*> transformations;
  // Append the Default Sequential transformation to the list. (It's first)
  transformations.push_back( new DefaultSequentialTransformation() );

  // Now we apply the transformations
  sched.apply( transformations );

  // Print everything
  cout << "Schedule state:\n" << sched
       << "\nDefault scheduled code:\n" << sched.codegen()
       << endl;

  /*
  As expected, nothing changed here.

  Additionally, the name of the statment can be prefixed.
  It will always be suffixed with "statement_#"
  */

  Schedule sched_prefix( chain, "my_prefix_" );
  cout << "Schedule state:\n" << sched_prefix
       << "\nCode with prefix \"my_prefix_\":\n" << sched_prefix.codegen()
       << endl;
  /*
  Code with prefix "my_prefix_":
  {
    for (int c1 = 0; c1 <= N; c1 += 1)
      my_prefix_statement_0(c1);
    for (int c1 = 1; c1 <= N; c1 += 1)
      for (int c2 = 1; c2 <= 2 * M; c2 += 1)
        my_prefix_statement_1(c1, c2);
  }

  Generated code can also be output to a file
  */

  sched.codegenToFile( "Schedule_HowTo_codgenToFile.cpp" );
}
