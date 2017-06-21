/*! ****************************************************************************
\file util.cpp
\authors Ian J. Bertolacci

\brief
Implements utility functions defined in the util.hpp

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#include <LoopChainIR/util.hpp>

void assertWithException( bool condition, const std::string& message ){
  if( !condition ){
    throw assert_exception( message );
  }
}
