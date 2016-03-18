/******************************************************************************
How To LoopChain
Recommended Reading:
+ LoopNestHowTo.cpp
******************************************************************************/
#include "LoopChain.hpp"
#include <iostream>
#include <utility>

using namespace std;

int main(){
  /*
  A LoopChain is an orderd list of LoopNests.
  No nests in the chain are required to be of the same dimensionality.

  Lets create a loop chain of nests with the following domains:
  1. {1..10}
  2. {0..N, 0..M}
  3. {M + N .. M+(2*N), 0..10, 0..20}

  First we create our LoopChain object.
  We will then append each nest to this.
  */
  LoopChain chain;

  { cout << "Create the first loop on {1..10}" << endl;
    // Create the domain
    string lower[1] = {"1"};
    string upper[1] = {"10"};
    RectangularDomain domain(lower, upper, 1);
    // Create the nest
    LoopNest nest( domain );

    // Now we append the chain to the nest
    chain.append( nest );
  }

  { cout << "Create the second loop on {0..N,0..M}" << endl;
    string lower[2] = {"0","0"};
    string upper[2] = {"N","M"};
    string symbols[2] = {"N","M"};
    // Append nest
    chain.append( LoopNest( RectangularDomain(lower, upper, 2, symbols, 2) ) );
  }

  { cout << "Create the second loop on {M + N .. M+(2*N), 0..10, 0..20} " << endl;
    string lower[3] = {"M + N","0","0"};
    string upper[3] = {"M+(2*N)","10","20"};
    string symbols[2] = {"N","M"};
    // Append nest
    chain.append( LoopNest( RectangularDomain(lower, upper, 3, symbols, 2) ) );
  }

  cout << endl;

  /*
  LoopChain has several member functions:
  void append( LoopNest nest ) appends nest to the loop chain
  LoopNest& getNest( size_type i ) returns reference to the LoopNest object
                                   at index i;
  size_type length() returns number of LoopNest objects in chain.
  size_type maxDimension() returns the maximum dimensionality
                           of all the loop-nests in the chain.
  */
  cout << "There are " << chain.length() << " nests in the chain" << endl;
  cout << chain.maxDimension() << " is the most maximal dimension" << endl;
  cout << endl;

  // Iterate over nests
  for( LoopChain::size_type n = 0; n < chain.length(); n++ ){
    cout << "Loop " << n << endl;
    // Get the domain from the nest at position n
    RectangularDomain& gotDomain = chain.getNest( n ).getDomain();

    // Print domain information
    cout << "\tThere are " << gotDomain.symbolics() << " symbols: ";
    for( RectangularDomain::size_type s = 0; s < gotDomain.symbolics(); s++ ){
      if( s != 0 ) cout << ", ";
      cout <<  gotDomain.getSymbol(s);
    }

    cout << endl;

    cout << "\tDimensions: " << gotDomain.dimensions() << endl;
    for( RectangularDomain::size_type d = 0; d < gotDomain.dimensions(); d++ ){
      cout << "\t\tDimension " << d << ": " << gotDomain.getLowerBound(d) << " .. " << gotDomain.getUpperBound(d) << endl;
    }

    cout << endl << endl;
  }


}
