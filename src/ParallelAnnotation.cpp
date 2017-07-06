#include <LoopChainIR/ParallelAnnotation.hpp>

using namespace LoopChainIR;
using namespace std;

ParallelAnnotation::ParallelAnnotation(){ }

std::vector<std::string> ParallelAnnotation::apply( Schedule& schedule ){
  this->apply( schedule, schedule.getSubspaceManager().get_nest() );
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
  schedule.addParallelSubspace( subspace );
  return std::vector<std::string>();
}
