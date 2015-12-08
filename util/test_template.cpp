#include <vector>
#include <string>
#include "util.h"
using namespace std;

/*****************************************************************************/
/********** BEGIN INJECTED CODE FROM BOUNDS CHOOSER **************************/
BOUNDS_CODE_STAMP
/********** END OF INJECTED CODE FROM BOUNDS CHOOSER *************************/
/*****************************************************************************/
/*****************************************************************************/
vector<string> test_generated(){
  vector<string> output;
  {
/*****************************************************************************/
/********** BEGIN INJECTED CODE FROM CODE GENERATOR **************************/
GENERATED_CODE_STAMP
/********** END OF INJECTED CODE FROM CODE GENERATOR *************************/
/*****************************************************************************/
  }
  return output;
}

vector<string> test_comparison(){
  vector<string> output;
  {
/*****************************************************************************/
/********** BEGIN INJECTED CODE FROM HUMAN COMPARISON ************************/
COMPARISON_CODE_STAMP
/********** END OF INJECTED CODE FROM HUMAN COMPARISON ***********************/
/*****************************************************************************/
  }
  return output;
}

int main( ){
  vector<string> generated_iterations = test_generated();
  vector<string> comparison_iterations = test_comparison();
  /*
  assertWithException( generated_iterations.size() == comparison_iterations.size(),
                       "Codes produce unequal number of iterations.\n" <<
                       "Generated: " << generated_iterations.size() << "\n" <<
                       "Comparison: " << comparison_iterations.size() << "\n"
                     );
  */
  if( generated_iterations.size() != comparison_iterations.size() ){
    return (generated_iterations.size() > comparison_iterations.size())? 111 : 112;
  }

  int count = 1;
  for( vector<string>::iterator gen_iter = generated_iterations.begin(),
                                comp_iter = comparison_iterations.begin();
       gen_iter != generated_iterations.end() &&
       comp_iter != comparison_iterations.end();
       ++gen_iter,
       ++comp_iter
    ){
    if( *gen_iter != *comp_iter ) return 113;
    /*
    assertWithException( *gen_iter == *comp_iter,
      "Iteration #" << count << " is different!\n" <<
      "Generated: " << *gen_iter << "\n" <<
      "Comparison: " << *comp_iter << "\n" );
    */
  }

  return 0;
}
