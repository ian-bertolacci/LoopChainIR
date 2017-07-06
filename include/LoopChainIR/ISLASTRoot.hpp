/*! ****************************************************************************
\file ISLASTRoot.hpp
\authors Ian J. Bertolacci

\brief
Defines the ISLASTRoot class

\copyright
Copyright 2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#ifndef ISLASTROOT_HPP
#define ISLASTROOT_HPP

#include <LoopChainIR/all_isl.hpp>

namespace LoopChainIR{
  class ISLASTRoot {
    public:
      isl_ast_node* root;
      isl_ctx* ctx;

      ISLASTRoot( isl_ast_node* root, isl_ctx* ctx );
  };
}
#endif
