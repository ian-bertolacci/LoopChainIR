#include "WavefrontTransformation.hpp"
#include <sstream>
#include <iostream>
#include <iterator>

using namespace std;
using namespace LoopChainIR;

WavefrontTransformation::WavefrontTransformation(){ }

std::vector<std::string> WavefrontTransformation::apply( Schedule &schedule ){
  // Default application is to the first non-loops subspace
  return this->apply( schedule, *std::next(schedule.getSubspaceManager().get_iterator_to_loops()) );
}

std::vector<std::string> WavefrontTransformation::apply( Schedule &schedule, Subspace *subspace ){
  std::vector<std::string> transformations;
  ostringstream transformation;
  SubspaceManager& manager = schedule.getSubspaceManager();

  subspace->set_aliased();

  transformation << "{ [" << manager.get_input_iterators() << "]"
                 << " -> " << "[" << manager.get_output_iterators() << "]"
                 << " : \n\t" << subspace->get( 0, true ) << " = ";
  {
    bool is_first = true;
    Subspace::size_type index = 0;
    for( Subspace::iterator it = subspace->begin( false ); it != subspace->end() && index < subspace->size(); ++it, ++index ){
      transformation << (is_first?"":" + ") << *it;
      is_first = false;
    }
  }

  {
    Subspace::iterator unaliased_it = ++(subspace->begin( false ));
    Subspace::iterator aliased_it   = ++(subspace->begin( true ));
    for( ;
        unaliased_it != subspace->end() && aliased_it != subspace->end();
        ++unaliased_it, ++aliased_it )
    {
      transformation << " and " << *aliased_it << " = " << *unaliased_it;
    }
  }

  transformation << "; };";

  transformations.push_back( transformation.str() );


  return transformations;
}
