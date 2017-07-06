#ifndef PARALLEL_ANNOTATION_HPP
#define PARALLEL_ANNOTATION_HPP

#include <LoopChainIR/Transformation.hpp>
#include <LoopChainIR/Schedule.hpp>

namespace LoopChainIR {
  class ParallelAnnotation : public Transformation {
    public:
      ParallelAnnotation();
      /*!
      \brief
      Generate ISCC code for a transformation, and append it to the transformation
      list of schedule (modifies schedule).

      \returns
      The ISCC code as a string
      */
      std::vector<std::string> apply( Schedule& schedule );

      /*!
      \brief
      Generate ISCC code for a transformation, and append it to the transformation
      list of schedule (modifies schedule) given a particular subspace.

      \returns
      The ISCC code as a string
      */
      std::vector<std::string> apply( Schedule& schedule, Subspace* subspace );
  };
}
#endif
