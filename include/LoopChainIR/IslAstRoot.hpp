/*! ****************************************************************************
\file IslAstRoot.hpp
\authors Ian J. Bertolacci

\brief
Defines the IslAstRoot class

\copyright
Copyright 2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#ifndef ISLASTROOT_HPP
#define ISLASTROOT_HPP

#include <LoopChainIR/all_isl.hpp>

class IslAstRoot{
  public:
    isl_ast_node* root;
    isl_ctx* ctx;

    IslAstRoot( isl_ast_node* root, isl_ctx* ctx );
};

#endif
