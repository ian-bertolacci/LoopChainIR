#ifndef PARALLEL_ANNOTATION_HPP
#define PARALLEL_ANNOTATION_HPP

#include <LoopChainIR/Transformation.hpp>
#include <LoopChainIR/Schedule.hpp>
#include <LoopChainIR/Subspace.hpp>

namespace LoopChainIR {
  class ParallelAnnotation : public Transformation {
    public:
      const Subspace::size_type additional_depth;

      ParallelAnnotation( );
      ParallelAnnotation( Subspace::size_type additional_depth );
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
