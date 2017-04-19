/*! ****************************************************************************
\file AutomaticShiftTransformation.hpp
\authors Ian J. Bertolacci

\brief
Shift all loops in preparation for a fusion

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#ifndef AUTOMATIC_SHIFT_TRANSFORMATION_HPP
#define AUTOMATIC_SHIFT_TRANSFORMATION_HPP

#include "LoopChain.hpp"
#include "Transformation.hpp"
#include "ShiftTransformation.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "stdio.h"

namespace LoopChainIR {

  class AutomaticShiftTransformation : public Transformation {
    public:
      AutomaticShiftTransformation();

      /*!
      \brief
      Generate ISCC code for the shift transformation, and append it to the
      transformation list of schedule (modifies schedule).

      \param[inout] schedule Schedule this transformation is being applied to.

      \returns
      The ISCC code as a string
      */
      std::vector<std::string> apply( Schedule& schedule, Subspace* subspace );

      /*!
      \brief
      Generate ISCC code for the shift transformation, and append it to the
      transformation list of schedule (modifies schedule).

      \param[inout] schedule Schedule this transformation is being applied to.

      \returns
      The ISCC code as a string
      */
      std::vector<std::string> apply( Schedule& schedule );

      /*
      \brief
      Computes shift tuples to legally shift the provided loop chain for fusion.
      */
      static std::list<Tuple> computeShiftTuplesForFusion( const LoopChain& chain );

      /*
      \brief
      Synthesizes ShiftTransformations to legally shift the provided loop chain for fusion.
      */
      static std::list<ShiftTransformation*> computeShiftForFusion( const LoopChain& chain );
  };

  class DataspaceMinMax {
    public:
      const std::string name;
    private:
      Tuple maxReads_tuple,
            minReads_tuple,
            maxWrites_tuple,
            minWrites_tuple;
    public:
      DataspaceMinMax( const Dataspace& dataspace );
      DataspaceMinMax( const DataspaceMinMax& that );

      Tuple maxRead() const;
      Tuple minRead() const;
      Tuple maxWrite() const;
      Tuple minWrite() const;

      void postShiftUpdate( const Tuple& extent );
      void selfUnion( const DataspaceMinMax& dataspace_minmax );
      void selfUnion( const Dataspace& dataspace );

      std::string str() const;
      friend std::ostream& LoopChainIR::operator<<( std::ostream& os, const DataspaceMinMax& dataspace );
  };

  class DataspaceMinMaxCollection {
    private:
      std::map< std::string, DataspaceMinMax > dataspaces;
    public:
      typedef std::map< std::string, DataspaceMinMax >::iterator iterator;
      DataspaceMinMaxCollection( );
      DataspaceMinMaxCollection( std::list<DataspaceMinMax> dataspaces );
      DataspaceMinMaxCollection( std::list<Dataspace> dataspaces );
      void postShiftUpdate( const Tuple& extent );
      void selfUnion( const DataspaceMinMax& dataspace_minmax );
      void selfUnion( const Dataspace& dataspace );

      iterator begin();
      iterator end();
      bool contains( std::string name ) const;
      DataspaceMinMax& operator[]( std::string name );
      const DataspaceMinMax& operator[]( std::string name ) const;

      std::string str() const;
      friend std::ostream& LoopChainIR::operator<<( std::ostream& os, const DataspaceMinMaxCollection& collection );
  };

}

#endif
