#include <iostream>
#include <cstdio>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <set>
#include <deque>
#include <cassert>

#include <LoopChainIR/util.hpp>
#include <LoopChainIR/ChapelTransformationWalker.hpp>
#include <LoopChainIR/util.hpp>

using namespace std;
using namespace LoopChainIR;

string LoopChainIR::convertISLtoChapel( ISLASTRoot& root ){
  return convertISLtoChapel( &root );
}

string LoopChainIR::convertISLtoChapel( ISLASTRoot* root ){
  return convertISLtoChapel( root->root );
}

string LoopChainIR::convertISLtoChapel( isl_ast_node* isl_root ){
  return (ChapelTransformationWalker( isl_root )).getSource();
}

ChapelTransformationWalker::ChapelTransformationWalker( isl_ast_node* isl_root )
: ChapelTransformationWalker(isl_root, false)
{ }

ChapelTransformationWalker::ChapelTransformationWalker( isl_ast_node* isl_root, bool verbose )
: depth( -1 ), scope_depth(0), verbose( verbose ), print_call(false), isl_root( isl_root ) {
  this->resulting_text = this->visit( isl_root );
}

std::string ChapelTransformationWalker::getSource(){
  return this->resulting_text;
}

std::string ChapelTransformationWalker::visit( isl_ast_expr* node ){
  this->depth += 1;
  string result( "unimplemented" );

  switch( isl_ast_expr_get_type(node) ){
    case isl_ast_expr_error:
      result = this->visit_expr_error( node );
      break;

    case isl_ast_expr_op:
      result = this->visit_expr_op( node );
      break;

    case isl_ast_expr_id:
      result = this->visit_expr_id( node );
      break;

    case isl_ast_expr_int:
      result = this->visit_expr_int( node );
      break;

    default:
      result = this->visit_expr_unknown( node );
      break;
  }

  this->depth -= 1;
  return result;
}

std::string ChapelTransformationWalker::visit( isl_ast_node* node ){
  this->depth += 1;
  string result( "unimplemented" );

  switch( isl_ast_node_get_type(node) ){
    case isl_ast_node_error:
      result = this->visit_node_error( node );
      break;

    case isl_ast_node_for:
      result = this->visit_node_for( node );
      break;

    case isl_ast_node_if:
      result = this->visit_node_if( node );
      break;

    case isl_ast_node_block:
      result = this->visit_node_block( node );
      break;

    case isl_ast_node_mark:
      result = this->visit_node_mark( node );
      break;

    case isl_ast_node_user:
      result = this->visit_node_user( node );
      break;

    default:
      result = this->visit_node_unknown( node );
      break;
  }

  this->depth -= 1;
  return result;
}

std::string ChapelTransformationWalker::visit_expr_op(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_expr_op" << endl;
  this->depth += 1;
  string result( "unimplemented" );

  switch( isl_ast_expr_get_op_type(node) ){
    case isl_ast_op_error:
      result = this->visit_op_error( node );
      break;

    case isl_ast_op_and:
      result = this->visit_op_and( node );
      break;

    case isl_ast_op_and_then:
      result = this->visit_op_and_then( node );
      break;

    case isl_ast_op_or:
      result = this->visit_op_or( node );
      break;

    case isl_ast_op_or_else:
      result = this->visit_op_or_else( node );
      break;

    case isl_ast_op_max:
      result = this->visit_op_max( node );
      break;

    case isl_ast_op_min:
      result = this->visit_op_min( node );
      break;

    case isl_ast_op_minus:
      result = this->visit_op_minus( node );
      break;

    case isl_ast_op_add:
      result = this->visit_op_add( node );
      break;

    case isl_ast_op_sub:
      result = this->visit_op_sub( node );
      break;

    case isl_ast_op_mul:
      result = this->visit_op_mul( node );
      break;

    case isl_ast_op_div:
      result = this->visit_op_div( node );
      break;

    case isl_ast_op_fdiv_q:
      result = this->visit_op_fdiv_q( node );
      break;

    case isl_ast_op_pdiv_q:
      result = this->visit_op_pdiv_q( node );
      break;

    case isl_ast_op_pdiv_r:
      result = this->visit_op_pdiv_r( node );
      break;

    case isl_ast_op_zdiv_r:
      result = this->visit_op_zdiv_r( node );
      break;

    case isl_ast_op_cond:
      result = this->visit_op_cond( node );
      break;

    case isl_ast_op_select:
      result = this->visit_op_select( node );
      break;

    case isl_ast_op_eq:
      result = this->visit_op_eq( node );
      break;

    case isl_ast_op_le:
      result = this->visit_op_le( node );
      break;

    case isl_ast_op_lt:
      result = this->visit_op_lt( node );
      break;

    case isl_ast_op_ge:
      result = this->visit_op_ge( node );
      break;

    case isl_ast_op_gt:
      result = this->visit_op_gt( node );
      break;

    case isl_ast_op_call:
      result = this->visit_op_call( node );
      break;

    case isl_ast_op_access:
      result = this->visit_op_access( node );
      break;

    case isl_ast_op_member:
      result = this->visit_op_member( node );
      break;

    case isl_ast_op_address_of:
      result = this->visit_op_address_of( node );
      break;

    default:
      result = this->visit_op_unknown( node );
      break;
  }

  this->depth -= 1;
  return result;
}


list<string> ChapelTransformationWalker::visit_expr_operands(isl_ast_expr* node ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_expr_operands" << endl;
  list<string> operands_list;

  for( int i = 0; i < isl_ast_expr_get_op_n_arg(node); i += 1 ){
    operands_list.push_back( this->visit_op_operand( node, i ) );
  }

  return operands_list;
}

std::string ChapelTransformationWalker::visit_op_operand( isl_ast_expr* node, int pos ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_operand" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) > pos, SSTR( "Invalid isl_ast_expr_op position " << pos << " > " << isl_ast_expr_get_op_n_arg(node) << " (isl_ast_expr_get_op_n_arg(node))") );
  assertWithException( pos >= 0, SSTR("Invalid isl_ast_expr_op position " << pos << " < 0.") );

  string exp = this->visit( isl_ast_expr_get_op_arg( node, pos ) );

  return exp;
}

std::string ChapelTransformationWalker::visit_op_lhs( isl_ast_expr* node ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_lhs" << endl;
  return visit_op_operand(node, 0);
}

std::string ChapelTransformationWalker::visit_op_rhs( isl_ast_expr* node ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_rhs" << endl;
  return visit_op_operand(node, 1);
}

std::string ChapelTransformationWalker::visit_op_unary_operand( isl_ast_expr* node ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_unary_operand" << endl;
  return visit_op_operand(node, 0);
}

std::string ChapelTransformationWalker::visit_op_cond_condition_operand( isl_ast_expr* node ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_cond_condition_operand" << endl;
  return visit_op_operand(node, 0);
}

std::string ChapelTransformationWalker::visit_op_cond_then_operand( isl_ast_expr* node ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_cond_then_operand" << endl;
  return visit_op_operand(node, 1);
}

std::string ChapelTransformationWalker::visit_op_cond_else_operand( isl_ast_expr* node ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_cond_else_operand" << endl;
  return visit_op_operand(node, 2);
}


std::string ChapelTransformationWalker::visit_op_error(isl_ast_expr* node __attribute__((unused))){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_error" << endl;
  assertWithException( this->ENCOUNTERED_ERROR_NODE, "An error node has been encoutered during ChapelTransformationWalker." );
  return string("unimplemented");
}

std::string ChapelTransformationWalker::visit_op_and( isl_ast_expr* node ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_and" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'and' operation does not have exactly two operands." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " & " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation and :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_and_then(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_and_then" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'and' operation does not have exactly two operands." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " && " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation and_then :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_or(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_or" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'or' operation does not have exactly two operands." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " | " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation or :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_or_else(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_or_else" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'or' operation does not have exactly two operands." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " || " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation or_else :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_max(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_max" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) >= 2, "ISL 'max' operation does not have two or more operands." );

  int last = isl_ast_expr_get_op_n_arg(node)-1;

  list<string> args = this->visit_expr_operands( node );

  string exp( SSTR( "max(" << stringJoin( ", ", args.begin(), args.end() ) << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "max :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_min(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_min" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) >= 2, "ISL 'min' operation does not have two or more operands." );

  list<string> args = this->visit_expr_operands( node );

  string exp( SSTR( "min(" << stringJoin( ", ", args.begin(), args.end() ) << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "min :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

// Unary minus
std::string ChapelTransformationWalker::visit_op_minus(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_minus" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 1, "ISL 'unary minus' operation does not have exactly one operand.");

  // Get child node
  string arg = this->visit_op_unary_operand( node );

  string exp( SSTR( "-(" << arg << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation minus :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_add(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_add" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2," ISL 'add' operation does not have exactly two operands." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " + " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation add :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_sub(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_sub" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'unary minus' operation does not have exactly two operand." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " - " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation sub :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_mul(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_mul" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'multiplcation' operation does not have exactly two operand." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " * " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation mul :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_div(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_div" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'divide' operation does not have exactly two operand." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " / " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation div :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_fdiv_q(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_fdiv_q" << endl;
  /*
  Round towards -infty
  Typically been #define floord(x, y) ( ((((x)%(y))>=0) ? ((x)/(y)) : (((x)/(y))) -1 ) )
  */
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'fdivq' operation does not have exactly two operand." );

  string x = this->visit_op_lhs( node );
  string y = this->visit_op_rhs( node );

  string exp(
     SSTR(
       "( if " << x << " % " <<  y << " >= 0 then "
       << "(" << x << " / " << y << ")"
       << " else " << "((" << x << " / " << y << ") -1)"
       << ")"
     )
  );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation fdiv_q :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_pdiv_q(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_pdiv_q" << endl;
  /* Dividend is non-negative */
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'pdivq' operation does not have exactly two operand." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " / " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation pdiv_q :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_pdiv_r(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_pdiv_r" << endl;
  /* Dividend is non-negative */
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'pdivr' operation does not have exactly two operand." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " % " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation pdiv_r :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_zdiv_r(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_zdiv_r" << endl;
  /* Result only compared against zero */
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'zdivr' operation does not have exactly two operand." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " % " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation zdiv_r :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_cond(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_cond" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 3, "ISL 'ternary' operation does not have exactly three operand." );

  string condition = this->visit_op_cond_condition_operand( node );
  string then_exp = this->visit_op_cond_then_operand( node );
  string else_exp = this->visit_op_cond_else_operand( node );

  string exp( SSTR( "( if " << condition << " then " << then_exp << " else " << else_exp << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation cond :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_select(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_select" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 3, "ISL 'select' operation does not have exactly three operand." );

  string condition = this->visit_op_cond_condition_operand( node );
  string then_exp = this->visit_op_cond_then_operand( node );
  string else_exp = this->visit_op_cond_else_operand( node );

  string exp( SSTR( "( if " << condition << " then " << then_exp << " else " << else_exp << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation select :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_eq(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_eq" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL 'equal-to' operation does not have exactly two operands.");

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " == " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation eq :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_le(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_le" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL '<=' operation does not have exactly two operands." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " <= " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation le :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_lt(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_lt" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2, "ISL '<' operation does not have exactly two operands." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " < " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation lt :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_ge(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_ge" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2,  "ISL '>=' operation does not have exactly two operands." );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " >= " << rhs << ")" ) );;

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation ge :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_gt(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_gt" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2,  "ISL '>' operation does not have exactly two operands."  );

  // Get children nodes
  string lhs = this->visit_op_lhs( node );
  string rhs = this->visit_op_rhs( node );

  string exp( SSTR( "(" << lhs << " > " << rhs << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation gt :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_call(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_call" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) >= 1,  "ISL 'call' operation does not have a function name." );
  // Get function name
  string name( isl_id_get_name( isl_ast_expr_get_id( isl_ast_expr_get_op_arg( node, 0 ) ) ) );
  list<string> operands = this->visit_expr_operands( node );
  string call( SSTR( name << "(" << stringJoin( ", ", ++(operands.begin()), operands.end() ) << ")") );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Call :" << endl << string( 10, '-' ) << call << endl << string( 10, '-' ) << endl;
  }

  return call;
}

std::string ChapelTransformationWalker::visit_op_access(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_access" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2,  "ISL 'access' operation does not have exactly two operands."  );

  // Head will be the running result of an access
  // Begins as a non access expression; as the root array expressions
  string ptr = this->visit_op_operand(node, 0);
  string index = this->visit_op_operand(node, 1);
  string exp( SSTR( "(" << ptr << "[" << index << "])" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "access :" << endl << string( 10, '-' ) << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_member(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_member" << endl;
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 2,  "ISL 'member access' operation does not have exactly two operands."  );

  // Get children nodes
  string data = this->visit_op_lhs( node );
  string member = this->visit_op_rhs( node );

  string exp( SSTR( "(" << data << "." << member << ")" ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "Operation gt :" << endl << string( 10, '-' ) << endl << exp << endl << string( 10, '-' ) << endl;
  }

  return exp;
}

std::string ChapelTransformationWalker::visit_op_address_of(isl_ast_expr* node){
  assertWithException( this->VISIT_TO_NODE_NOT_ALLOWED, "Cannot produce address of code in Chapel." );
  assertWithException( isl_ast_expr_get_op_n_arg(node) == 1,  "ISL 'address of' operation does not have exactly two operands."  );
  return string("unimplemented");
}

std::string ChapelTransformationWalker::visit_op_unknown(isl_ast_expr* node __attribute__((unused)) ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_op_unknown" << endl;
  assertWithException( this->ENCOUNTERED_UNKOWN_NODE, "A node of undetermined type has been visited during ChapelTransformationWalker." );
  return string("unimplemented");
}


std::string ChapelTransformationWalker::visit_expr_id(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_expr_id" << endl;
  string id( isl_id_get_name( isl_ast_expr_get_id(node) ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "id :" << endl << string( 10, '-' ) << endl << id << endl << string( 10, '-' ) << endl;
  }

  return id;
}

std::string ChapelTransformationWalker::visit_expr_int(isl_ast_expr* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_expr_int" << endl;
  isl_val* isl_value = isl_ast_expr_get_val( node );
  long num = isl_val_get_num_si( isl_value );
  long den = isl_val_get_den_si( isl_value );
  assertWithException( den == 1, SSTR( "Denominator of ISL integer is not 1: " << den ) );

  int int_value = (int) num;
  assertWithException( ((long) int_value) == num, SSTR("Cast produced invalid number: " << ((long) int_value) << " vs " << num ) );

  string value( SSTR( int_value ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "int: " << int_value << " :" << endl << string( 10, '-' ) << value << endl << string( 10, '-' ) << endl;
  }

  return value;
}

std::string ChapelTransformationWalker::visit_expr_unknown(isl_ast_expr* node __attribute__((unused))){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_expr_unknown" << endl;
  assertWithException( this->ENCOUNTERED_UNKOWN_NODE, "A node of undetermined type has been visited during ChapelTransformationWalker." );
  return string("unimplemented");
}

std::string ChapelTransformationWalker::visit_expr_error(isl_ast_expr* node __attribute__((unused))){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_expr_error" << endl;
  assertWithException( this->ENCOUNTERED_ERROR_NODE, "An error node has been encoutered during ChapelTransformationWalker." );
  return string("unimplemented");
}


std::string ChapelTransformationWalker::visit_node_error(isl_ast_node* node __attribute__((unused))){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_node_error" << endl;
  assertWithException( this->ENCOUNTERED_ERROR_NODE, "An error node has been encoutered during ChapelTransformationWalker." );
  return string("unimplemented");
}

std::string ChapelTransformationWalker::visit_node_for(isl_ast_node* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_node_for" << endl;
  this->depth += 1;
  string iterator( isl_id_get_name( isl_ast_expr_get_id( isl_ast_node_for_get_iterator(node) ) ) );
  string lower_bound( this->visit( isl_ast_node_for_get_init( node ) ) );

  // First, check that root node of for's condition is an acceptable operator ( <, <=)
  // TODO support >, >=
  isl_ast_op_type cond_operator = isl_ast_expr_get_op_type( isl_ast_node_for_get_cond( node ) );

  assertWithException(
    (set<isl_ast_op_type>({isl_ast_op_le, isl_ast_op_lt})).count( cond_operator ) == 1,
    SSTR( "condition operator is not acceptable (" << cond_operator << "): \"" << this->visit( isl_ast_node_for_get_cond( node ) ) << "\"" )
  );

  // Next, Check that lhs is exaclty an id expression
  //isl_ast_expr_get_type(node)isl_ast_expr_id
  assertWithException(
    isl_ast_expr_get_type( isl_ast_expr_get_op_arg(isl_ast_node_for_get_cond(node), 0) ) == isl_ast_expr_id,
    SSTR( "LHS of conditional in for loop is not an id expression (" << isl_ast_expr_get_type( isl_ast_expr_get_op_arg( isl_ast_node_for_get_cond(node), 0) ) << ")" )
  );

  // Create upper bound on range from rhs of the conditional expression
  string upper_bound( this->visit_op_rhs( isl_ast_node_for_get_cond( node ) ) );
  ostringstream range_stream;
  range_stream << lower_bound << ".." << upper_bound;

  // if expression is an int, and is one, no need to create stride.
  isl_ast_expr* increment_node = isl_ast_node_for_get_inc( node );
  if(    isl_ast_expr_get_type( increment_node ) != isl_ast_expr_int
      || isl_val_get_num_si( isl_ast_expr_get_val( increment_node ) ) != 1 )
  {
    range_stream << " by " << this->visit( isl_ast_node_for_get_inc( node ) );
  }

  string range = range_stream.str();


  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "range :" << endl << string( 10, '-' ) << endl << range << endl << string( 10, '-' ) << endl;
  }

  this->scope_depth += 1;
  string statement = this->visit( isl_ast_node_for_get_body( node ) );
  this->scope_depth -= 1;

  // Check for parallel annotation
  isl_id* maybe_annotation = isl_ast_node_get_annotation( node );
  string loop_keyword =  ( maybe_annotation != nullptr && string( isl_id_get_name( maybe_annotation ) ) == string("parallel annotation") )
    ? "forall"
    : "for" ;

  string for_stmt( SSTR(
  string( this->scope_depth*2, ' ' ) << loop_keyword << " " << iterator << " in " << range << " {\n"
    << statement << "\n" << string( this->scope_depth*2, ' ' ) << "}"
  ) );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "for :" << endl << string( 10, '-' ) << endl << for_stmt << endl << string( 10, '-' ) << endl;
  }

  return for_stmt;
}

std::string ChapelTransformationWalker::visit_node_if(isl_ast_node* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_node_if" << endl;
  string condition = this->visit( isl_ast_node_if_get_cond(node) );

  // build then statement
  this->scope_depth += 1;
  string then_stmt = this->visit( isl_ast_node_if_get_then(node) );
  this->scope_depth -= 1;

  // Build else statement (will be empty if there is no else)
  string else_stmt;
  bool has_else = isl_ast_node_if_has_else( node );
  if( has_else ){
    this->scope_depth += 1;
    else_stmt = this->visit( isl_ast_node_if_get_else(node) );
    this->scope_depth -= 1;
  }

  string if_stmt( SSTR(
  string( this->scope_depth*2, ' ' ) << "if " << condition << "{\n"
    << then_stmt
    << "\n" << string( this->scope_depth*2, ' ' ) <<"}"
    << ( has_else ?
         SSTR( "else {\n" << else_stmt << "\n" << string( this->scope_depth*2, ' ' ) << "}" )
        : "" )
    )
  );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "if :" << endl << string( 10, '-' ) << endl << if_stmt << endl << string( 10, '-' ) << endl;
  }

  return if_stmt;
}

std::string ChapelTransformationWalker::visit_node_block(isl_ast_node* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_node_block" << endl;
  list<string> statements;
  isl_ast_node_list* stmt_list = isl_ast_node_block_get_children(node);
  // collect all statements
  this->scope_depth += 1;
  for( int i = 0; i < isl_ast_node_list_n_ast_node(stmt_list); i += 1 ){
    statements.push_back( this->visit( isl_ast_node_list_get_ast_node(stmt_list, i) ) );
  }
  this->scope_depth -= 1;

  string block( SSTR(
  string( this->scope_depth*2, ' ' ) << "{\n"
    << stringJoin( "\n", statements.begin(), statements.end() ) << string( this->scope_depth*2, ' ' )
    << "\n" << string( this->scope_depth*2, ' ' ) << "}"
    )
  );

  if( this->verbose ){
    cout << string( this->depth*2, ' ' ) << string( 10, '=' ) << "block :" << endl << string( 10, '-' ) << endl << block << endl << string( 10, '-' ) << endl;
  }

  return block;
}

std::string ChapelTransformationWalker::visit_node_mark(isl_ast_node* node __attribute__((unused)) ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_node_mark" << endl;
  assertWithException( this->VISIT_TO_NODE_NOT_IMPLEMENTED, "ChapelTransformationWalker::visit_node_mark Not implemented." );
  return string("unimplemented");
}

std::string ChapelTransformationWalker::visit_node_user(isl_ast_node* node){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_node_user" << endl;
  return SSTR( string( this->scope_depth*2, ' ' ) << this->visit( isl_ast_node_user_get_expr(node) ) << ";");
}

std::string ChapelTransformationWalker::visit_node_unknown(isl_ast_node* node __attribute__((unused)) ){
  if( this->print_call ) cout << string( this->depth*2, ' ' ) << "visit_node_unknown" << endl;
  assertWithException( this->ENCOUNTERED_UNKOWN_NODE, "A node of undetermined type has been visited during ChapelTransformationWalker." );
  return string("unimplemented");
}
