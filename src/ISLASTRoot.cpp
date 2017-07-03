/*! ****************************************************************************
\file ISLASTRoot.cpp
\authors Ian J. Bertolacci

\brief
Implements the ISLASTRoot class

\copyright
Copyright 2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#include <LoopChainIR/ISLASTRoot.hpp>
#include <LoopChainIR/util.hpp>

using namespace LoopChainIR;

ISLASTRoot::ISLASTRoot( isl_ast_node* root, isl_ctx* ctx )
: root(root), ctx(ctx)
{ }
