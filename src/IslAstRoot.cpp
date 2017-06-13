/*! ****************************************************************************
\file IslAstRoot.cpp
\authors Ian J. Bertolacci

\brief
Implements the IslAstRoot class

\copyright
Copyright 2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#include <LoopChainIR/IslAstRoot.hpp>
#include <LoopChainIR/util.hpp>

IslAstRoot::IslAstRoot( isl_ast_node* root, isl_ctx* ctx ): root(root), ctx(ctx) {}
