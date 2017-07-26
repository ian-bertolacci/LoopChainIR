#include <LoopChainIR/ParallelAnnotation.hpp>

using namespace LoopChainIR;
using namespace std;

ParallelAnnotation::ParallelAnnotation( )
: ParallelAnnotation( (Subspace::size_type) 0 )
{ }

ParallelAnnotation::ParallelAnnotation( Subspace::size_type additional_depth )
: additional_depth( additional_depth )
{ }

std::vector<std::string> ParallelAnnotation::apply( Schedule& schedule ){
  this->apply( schedule, *(std::next(schedule.getSubspaceManager().get_iterator_to_loops())) );
  return std::vector<std::string>();
}

/*!
\brief
Generate ISCC code for a transformation, and append it to the transformation
list of schedule (modifies schedule) given a particular subspace.

\returns
The ISCC code as a string
*/
std::vector<std::string> ParallelAnnotation::apply( Schedule& schedule, Subspace* subspace ){
  schedule.addParallelSubspace( subspace, this->additional_depth );
  return std::vector<std::string>();
}
