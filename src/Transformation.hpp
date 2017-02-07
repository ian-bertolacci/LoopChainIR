#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include "Schedule.hpp"
#include "Subspace.hpp"
#include <string>

namespace LoopChainIR {

  // Forward declarations because C++ was a mistake.
  class Schedule;

  /*!
  \brief
  Abstract object that all scheduler 'things' inherit for polymorphic purposes.
  */
  class Transformation {
  public:
    typedef int size_type;

    /*!
    \brief
    Generate ISCC code for a transformation, and append it to the transformation
    list of schedule (modifies schedule).

    \returns
    The ISCC code as a string
    */
    virtual std::string& apply( Schedule& schedule ) = 0;

    /*!
    \brief
    Generate ISCC code for a transformation, and append it to the transformation
    list of schedule (modifies schedule) given a particular subspace.

    \returns
    The ISCC code as a string
    */
    virtual std::string& apply( Schedule& schedule, Subspace* subspace ) = 0;
  };

}
#endif
