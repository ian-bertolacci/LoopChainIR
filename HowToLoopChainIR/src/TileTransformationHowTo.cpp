/******************************************************************************
How To Schedule
Recommended Reading:
+ ScheduleHowTo.cpp
******************************************************************************/
#include "TileTransformation.hpp"
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main(){
  {
    cout << "example preamble" << endl;
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

    */

    // Let's shift and fuse our loops.
    vector<Transformation*> transformations;
    // Create our shift transformation
    // Fuse our two loops
    vector<LoopChain::size_type> fuse_these;
    TileTransformation transformation( 0, "10" );
    transformations.push_back( &transformation );

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
    */
  }
}
