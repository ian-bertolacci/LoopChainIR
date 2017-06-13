/******************************************************************************
How To LoopNest
Recommended Reading:
+ RectangularDomainHowTo.cpp
******************************************************************************/
#include <LoopChainIR/LoopNest.hpp>
#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;

int main(){
  /*
  A LoopNest is an encapsulation of a domain.
  As of right now, it doesnt do anything useful or provide any additional
  services that a domain doesnt.
  */

  cout << "Constructing a loop nest on {1..N,1..M*2}" << endl;
  /*
  To construct a LoopNest, you first have to construct a domain.
  */
  string lower[2] = {"1","1"};
  string upper[2] = {"N","M*2"};
  string symbols[2] = {"N","M"};
  RectangularDomain domain(lower,upper,2,symbols,2);

  /*
  Then we simply pass the domain to the constructor:
  LoopNest( RectangularDomain loop_bounds )
  */
  LoopNest nest( domain );

  /*
  LoopNest only has one member function:
  RectangularDomain& getDomain() returns a reference to the domain it was
                                 constructed with.
  */

  RectangularDomain gotDomain = nest.getDomain();

  // Print domain information
  cout << "Dimensions: " << gotDomain.dimensions() << endl;
  for( RectangularDomain::size_type d = 0; d < gotDomain.dimensions(); d++ ){
    cout << "Dimension " << d << ": " << gotDomain.getLowerBound(d) << " .. " << gotDomain.getUpperBound(d) << endl;
  }

  cout << "There are " << gotDomain.symbolics() << " symbols: ";
  bool not_first = false;
  for( string symbol : gotDomain.getSymbols() ){
    if( not_first ) cout << ", ";
    not_first = true;
    cout <<  symbol;
  }
  cout << endl;

}
