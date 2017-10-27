#ifndef SAGETRANSFORMATIONWALKER_HPP
#define SAGETRANSFORMATIONWALKER_HPP

#include <LoopChainIR/all_isl.hpp>
#include <LoopChainIR/util.hpp>
#include <LoopChainIR/ISLASTRoot.hpp>

#include <list>
#include <map>
#include <string>

namespace LoopChainIR {

  std::string convertISLtoChapel( ISLASTRoot* root );
  std::string convertISLtoChapel( ISLASTRoot& root );
  std::string convertISLtoChapel( isl_ast_node* isl_root );

  class ChapelTransformationWalker {
    public:
    protected:
      const bool VISIT_TO_NODE_NOT_IMPLEMENTED = false;
      const bool VISIT_TO_NODE_NOT_ALLOWED = false;
      const bool ENCOUNTERED_UNKOWN_NODE = false;
      const bool ENCOUNTERED_ERROR_NODE = false;

      int depth;
      int scope_depth;
      const bool verbose;
      const bool print_call;
      isl_ast_node* isl_root;
      std::string resulting_text;

    public:
      ChapelTransformationWalker( isl_ast_node* isl_root );
      ChapelTransformationWalker( isl_ast_node* isl_root, bool verbose );

      std::string getSource();

    protected:
      // Generic visit switcher methods
      std::string visit( isl_ast_node* node );
      std::string visit( isl_ast_expr* node );

      // Operation visit switch method
      std::string visit_expr_op(isl_ast_expr* node);

      // Operands visitor methods
      std::list<std::string> visit_expr_operands(isl_ast_expr* node );
      std::string visit_op_operand( isl_ast_expr* node, int pos );
      std::string visit_op_lhs( isl_ast_expr* node );
      std::string visit_op_rhs( isl_ast_expr* node );
      std::string visit_op_unary_operand( isl_ast_expr* node );
      std::string visit_op_cond_condition_operand( isl_ast_expr* node );
      std::string visit_op_cond_then_operand( isl_ast_expr* node );
      std::string visit_op_cond_else_operand( isl_ast_expr* node );

      // Visit operation node methods
      std::string visit_op_error(isl_ast_expr* node);
      std::string visit_op_and(isl_ast_expr* node);
      std::string visit_op_and_then(isl_ast_expr* node);
      std::string visit_op_or(isl_ast_expr* node);
      std::string visit_op_or_else(isl_ast_expr* node);
      std::string visit_op_max(isl_ast_expr* node);
      std::string visit_op_min(isl_ast_expr* node);
      std::string visit_op_minus(isl_ast_expr* node);
      std::string visit_op_add(isl_ast_expr* node);
      std::string visit_op_sub(isl_ast_expr* node);
      std::string visit_op_mul(isl_ast_expr* node);
      std::string visit_op_div(isl_ast_expr* node);
      std::string visit_op_fdiv_q(isl_ast_expr* node);
      std::string visit_op_pdiv_q(isl_ast_expr* node);
      std::string visit_op_pdiv_r(isl_ast_expr* node);
      std::string visit_op_zdiv_r(isl_ast_expr* node);
      std::string visit_op_cond(isl_ast_expr* node);
      std::string visit_op_select(isl_ast_expr* node);
      std::string visit_op_eq(isl_ast_expr* node);
      std::string visit_op_le(isl_ast_expr* node);
      std::string visit_op_lt(isl_ast_expr* node);
      std::string visit_op_ge(isl_ast_expr* node);
      std::string visit_op_gt(isl_ast_expr* node);
      std::string visit_op_call(isl_ast_expr* node);
      std::string visit_op_access(isl_ast_expr* node);
      std::string visit_op_member(isl_ast_expr* node);
      std::string visit_op_address_of(isl_ast_expr* node);

      std::string visit_op_unknown(isl_ast_expr* node);

      // Visit literal expression methods
      std::string visit_expr_id(isl_ast_expr* node);
      std::string visit_expr_int(isl_ast_expr* node);

      std::string visit_expr_unknown(isl_ast_expr* node);
      std::string visit_expr_error(isl_ast_expr* node);

      // Visit statement node methods
      std::string visit_node_for(isl_ast_node* node);
      std::string visit_node_if(isl_ast_node* node);
      std::string visit_node_block(isl_ast_node* node);
      std::string visit_node_mark(isl_ast_node* node);

      std::string visit_node_user(isl_ast_node* node);

      std::string visit_node_unknown(isl_ast_node* node);
      std::string visit_node_error(isl_ast_node* node);

  };
}

#endif
