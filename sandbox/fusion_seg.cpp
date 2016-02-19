
#include "FusionTransformation.hpp"
#include <iostream>
#include <utility>

using namespace std;

int main(){
  LoopChain chain;


  {
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  {
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  vector<Transformation*> schedulers;
  vector<LoopChain::size_type> fuse_these;
  fuse_these.push_back( (LoopChain::size_type) 0 );
  fuse_these.push_back( (LoopChain::size_type) 1 );
  FusionTransformation* a = new FusionTransformation( fuse_these );
  cout << "We ok "<< endl;
  schedulers.push_back( a );
  cout << "we still ok "<< endl;
  Schedule sched( chain );
  sched.apply( schedulers );
  cout << "Transformations: " << endl;
  for( Schedule::iterator it = sched.begin_transformations(); it != sched.end_transformations(); ++it ){
    cout << *it << endl;
  }
  cout << "Domains: " << endl;
  for( Schedule::iterator it = sched.begin_domains(); it != sched.end_domains(); ++it ){
    cout << *it << endl;
  }
  cout << "     " << endl;
  cout << "we might still be ok "<< endl;
  cout << sched.codegen() << endl;
  cout << "we made it "<< endl;

  return 0;
}
