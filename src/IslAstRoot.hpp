#ifndef ISLASTROOT_HPP
#define ISLASTROOT_HPP

#include "all_isl.hpp"

class IslAstRoot{
  public:
    isl_ast_node* root;
    isl_ctx* ctx;

    IslAstRoot( isl_ast_node* root, isl_ctx* ctx );
};

#endif
