/******************************************************************************
How To RectangularDomain
******************************************************************************/
#include "RectangularDomain.hpp"
#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;

int main(){
  /*
  Rectangular domains are simply domains of indices who's bounds are determined
  by numeric evaluation.
  Simply put, there are no conditionals.
  For example:
  + {1..10} is a 1D domain of indices from 1 to 10.
  + {1..10,5..20} is a 2D domain of indices from the set product {1..10} x {5..20}
  + {1..N} is a 1D domain of indices from 1 to N
  + {N + 5 * 7 .. M * 2 + 5} is a 1D domain of indices from M + 30 to 2N + 5
  */

  {
    cout << "Constructing {1..10}" << endl;
    /*
    Lets construct the domain {1..10}
    We need to provide the constructor with 2 arrays of strings.
    One representing the lower bounds, one representing the upper bounds
    */
    string lower[1] = { "0" };
    string upper[1] = { "10" };

    /*
    Now we can construct the RectangularDomain
    Constructor is RectangularDomain( string lower_bounds[],
                                      string upper_bounds[],
                                      size_type dimensions )
    see the documentation for information about size_type
    */
    RectangularDomain domain( lower, upper, 1 );
    /*
    RectangularDomain has several member functions
    size_type dimensions() returns the dimensionality of the domain
    string getUpperBound( size_type i ) returns the string representing the i'th
                                        dimension's upper bound
    string getLowerBound( size_type i ) returns the string representing the i'th
                                        dimension's lower bound

    The rest relate to symbolics (which we'll see later)
    size_type symbolics() returns the number of symbolics
    string getSymbol( size_type i ) returns the i'th symbolic
    */

    // Print number of dimensions
    cout << "Dimensions: " << domain.dimensions() << endl;
    for( RectangularDomain::size_type d = 0; d < domain.dimensions(); d++ ){

      cout << "Dimension " << d << ": "
           // Print this dimensions lower bound
           << domain.getLowerBound(d) << " .. "
           // and upper bound
           << domain.getUpperBound(d) << endl;
    }
    cout << endl;

  }

  {
    cout << "Constructing {1..10,1..20}" << endl;
    /*
    Again we create our lower and upper bounds arrays.
    However, order matters.
    The i'th domension's upper/lower bounds should be plaupper_upper_bounds
    string upper[2] = {"10","20"};

    // Note the 2, since we have 2 dimensions
    RectangularDomain domain( lower, upper, 2 );

    cout << "Dimensions: " << domain.dimensions() << endl;
    for( RectangularDomain::size_type d = 0; d < domain.dimensions(); d++ ){
      cout << "Dimension " << d << ": " << domain.getLowerBound(d) << " .. " << domain.getUpperBound(d) << endl;
    }
  }

  {
    cout << "Constructing {N + 5 * 7 .. M * 2 + 5}" << endl;
    /*
    Bounds can also be arithmetic expressions and use symbolics not known at
    compile time.
    Nothing changes w.r.t. the upper and lower bounds.
    They just contain a string of an expression
    */
    string lower[1] = {"N + 5 * 7"};
    string upper[1] = {"M * 2 + 5"};

    /*
    However now we need to tell the domain what the symbols are using an array.
    The order of the symbols does not matter.
    */
    string symbols[2] = {"N","M"};

    /*
    Now we use the constructor that includes symbols:
    RectangularDomain( string lower_bounds[],
                       string upper_bounds[],
                       size_type dimensions,
                       string symbols[],
                       size_type symbolics )
    */
    RectangularDomain domain( lower, upper, 1, symbols, 2 );

    cout << "Dimensions: " << domain.dimensions() << endl;
    for( RectangularDomain::size_type d = 0; d < domain.dimensions(); d++ ){
      cout << "Dimension " << d << ": " << domain.getLowerBound(d) << " .. " << domain.getUpperBound(d) << endl;
    }

    /*
    Now we can try the member functions for symbols
    */

    // Print the number of symbols
    cout << "There are " << domain.symbolics() << " symbols: ";
    for( RectangularDomain::size_type s = 0; s < domain.symbolics(); s++ ){
      if( s != 0 ) cout << ", ";
      // Print the s'th symbolic
      cout <<  domain.getSymbol(s);
    }
    cout << endl << endl;

  }

}
