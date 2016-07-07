#include "IslAstRoot.hpp"

IslAstRoot::IslAstRoot( isl_ast_node* root, isl_ctx* ctx ): root(root), ctx(ctx) {}
