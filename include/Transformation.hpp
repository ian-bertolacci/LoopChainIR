/*! ****************************************************************************
\file Transformation.hpp
\authors Ian J. Bertolacci

\brief
Defines the abstract class Transformation

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include "Schedule.hpp"
#include "Subspace.hpp"
#include <string>
#include <vector>

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
    virtual std::vector<std::string> apply( Schedule& schedule ) = 0;

    /*!
    \brief
    Generate ISCC code for a transformation, and append it to the transformation
    list of schedule (modifies schedule) given a particular subspace.

    \returns
    The ISCC code as a string
    */
    virtual std::vector<std::string> apply( Schedule& schedule, Subspace* subspace ) = 0;
  };

}
#endif
