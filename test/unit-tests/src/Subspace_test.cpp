/*! ****************************************************************************
\file Subspace_test.cpp
\authors Ian J. Bertolacci

\brief
To perform unit testsing on the Iterator, Subspace, and SubspaceTree classes.

\copyright
Copyright 2016 University of Arizona
*******************************************************************************/

#include "gtest/gtest.h"
#include <LoopChainIR/Subspace.hpp>
#include <LoopChainIR/util.hpp>
#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;

TEST( Subspace_test, construct_base_1 ){
  ASSERT_NO_THROW({
    Subspace a( "i", 1 );
  });
}

TEST( Subspace_test, construct_base_2 ){
  ASSERT_NO_THROW({
    Subspace a( "i", 10 );
  });
}

TEST( Subspace_test, construct_other_1 ){
  int N = 5;
  Subspace base( "i", N );
  ASSERT_NO_THROW({
    Subspace prefixed( "j", N, base );
  });
}

TEST( Subspace_test, construct_other_2 ){
  int N = 5;
  Subspace base( "i", N );
  ASSERT_NO_THROW({
    Subspace prefixed( "j", N-2, base );
  });
}

TEST( Subspace_test, constructor_other_bad ){
  Subspace base( "i", 5 );
  ASSERT_ANY_THROW({
    Subspace failure( "j", 6, base );
  });
}

TEST( Subspace_test, size_1 ){
  int n = 1;
  Subspace a( "i", n );
  ASSERT_EQ( a.size(), n );
  ASSERT_EQ( a.complete_size(), n+1 );
  ASSERT_EQ( a.size(), a.complete_size() -1 );
}

TEST( Subspace_test, size_2 ){
  int n = 5;
  Subspace a( "i", n );
  ASSERT_EQ( a.size(), n );
  ASSERT_EQ( a.complete_size(), n+1 );
  ASSERT_EQ( a.size(), a.complete_size() -1 );
}

TEST( Subspace_test, size_3 ){
  int n = 0;
  Subspace a( "i", n );
  ASSERT_EQ( a.size(), n );
  ASSERT_EQ( a.complete_size(), n+1 );
  ASSERT_EQ( a.size(), a.complete_size() - 1 );
}

TEST( Subspace_test, size_other_1 ){
  int n = 5;
  Subspace a( "i", n );
  Subspace b( "j", a.size(), a );
  ASSERT_EQ( b.size(), a.size() );
  ASSERT_EQ( b.complete_size(), a.complete_size() );
}

TEST( Subspace_test, size_other_2 ){
  int n = 5;
  Subspace a( "i", n );
  Subspace b( "j", a.size()-1, a );
  ASSERT_EQ( b.size(), a.size()-1 );
  ASSERT_EQ( b.complete_size(), a.complete_size()-1 );
}

TEST( Subspace_test, size_other_3 ){
  int n = 5;
  Subspace a( "i", n );
  Subspace b( "j", 1, a );
  ASSERT_EQ( b.size(), 1 );
  ASSERT_EQ( b.complete_size(), 2 );
}

TEST( Subspace_test, set_test_unset_aliased ){
  Subspace a( "i", 1 );
  // Check default
  ASSERT_FALSE( a.is_aliased() );
  // Check set - change
  a.set_aliased();
  ASSERT_TRUE( a.is_aliased() );
  // Check double-tap no-change
  a.set_aliased();
  ASSERT_TRUE( a.is_aliased() );

  // Check unset - change
  a.unset_aliased();
  ASSERT_FALSE( a.is_aliased() );
  // Check double-tap no-change
  a.set_aliased();
  ASSERT_TRUE( a.is_aliased() );

  // Check set - change
  a.set_aliased();
  ASSERT_TRUE( a.is_aliased() );
}

TEST( Subspace_test, test_values_1 ){
  int N = 1;
  Subspace a( "i", N );

  Subspace::iterator iter = a.begin();
  for( Subspace::size_type i = 0; i < a.size(); ++i, ++iter ){
    ASSERT_EQ( *iter, SSTR( "i_" << i ) );
  }
  ASSERT_EQ( *iter, "i_c" );
}

TEST( Subspace_test, test_values_2 ){
  int N = 5;
  Subspace a( "i", N );

  Subspace::iterator iter = a.begin();
  for( Subspace::size_type i = 0; i < a.size(); ++i, ++iter ){
    ASSERT_EQ( *iter, SSTR( "i_" << i ) );
  }
  ASSERT_EQ( *iter, "i_c" );
}

TEST( Subspace_test, test_values_3 ){
  int N = 0;
  Subspace a( "i", N );

  Subspace::iterator iter = a.begin();
  for( Subspace::size_type i = 0; i < a.size(); ++i, ++iter ){
    ASSERT_EQ( *iter, SSTR( "i_" << i ) );
  }
  ASSERT_EQ( *iter, "i_c" );
}

TEST( Subspace_test, test_values_prefix_1 ){
  int N = 10;
  Subspace a( "i", N );
  Subspace b( "t", a.size(), a );

  Subspace::iterator iter = b.begin();
  for( Subspace::size_type i = 0; i < b.size() ; ++i, ++iter ){
    ASSERT_EQ( *iter, SSTR( "t_i_" << i ) );
  }
  ASSERT_EQ( *iter, "t_i_c" );
}

TEST( Subspace_test, test_values_prefix_2 ){
  int N = 10;
  Subspace a( "i", N );
  Subspace b( "t", a.size()-5, a );

  Subspace::iterator iter = b.begin();
  for( Subspace::size_type i = 0; i < b.size() ; ++i, ++iter ){
    ASSERT_EQ( *iter, SSTR( "t_i_" << i ) );
  }
  ASSERT_EQ( *iter, "t_i_c" );
}

TEST( Subspace_test, unstaged_stage_get_stage ){
  Subspace a( "i", 1 );
  ASSERT_EQ( a.get_stage(), timestamp_flags::Unstaged );
  a.set_stage( timestamp_flags::BaseStage );
  ASSERT_EQ( a.get_stage(), timestamp_flags::BaseStage );
  a.set_stage( timestamp_flags::BaseStage + 1 );
  ASSERT_EQ( a.get_stage(), timestamp_flags::BaseStage + 1);
}

TEST( Subspace_test, alias_values ){
  int N = 0;
  Subspace a( "i", N );

  // Check when set
  a.set_aliased();
  {
    Subspace::iterator iter = a.begin();
    for( Subspace::size_type i = 0; i < a.size(); ++i, ++iter ){
      ASSERT_EQ( *iter, SSTR( Subspace::alias_prefix << "i_" << i ) );
    }
    ASSERT_EQ( *iter,  SSTR( Subspace::alias_prefix << "i" << Subspace::const_suffix ) );
  }
  // Check when unset

  a.unset_aliased();
  {
    Subspace::iterator iter = a.begin();
    for( Subspace::size_type i = 0; i < a.size(); ++i, ++iter ){
      ASSERT_EQ( *iter, SSTR( "i_" << i ) );
    }
    ASSERT_EQ( *iter,  SSTR( "i" << Subspace::const_suffix ) );
  }
}

TEST( Subspace_test, get_iterators_1 ){
  Subspace a( "i", 3 );
  ASSERT_EQ( "i_0,i_1,i_2,i_c", a.get_iterators( timestamp_flags::BaseStage, false ) );
}

TEST( Subspace_test, get_iterators_2 ){
  Subspace a( "i", 0 );
  ASSERT_EQ( "i_c", a.get_iterators( timestamp_flags::BaseStage, false ) );
}

TEST( Subspace_test, get_iterators_3 ){
  Subspace a( "i", 2 );
  a.set_stage( timestamp_flags::BaseStage + 1000 );
  ASSERT_EQ( "", a.get_iterators( timestamp_flags::BaseStage, false ) );
}

TEST( Subspace_test, get_iterators_4 ){
  Subspace a( "i", 2 );
  a.set_aliased();
  ASSERT_EQ( "alias_i_0,alias_i_1,alias_i_c", a.get_iterators( timestamp_flags::BaseStage, true ) );
}

TEST( Subspace_test, get_iterators_5 ){
  Subspace a( "i", 2 );
  a.set_aliased();
  ASSERT_EQ( "i_0,i_1,i_c", a.get_iterators( timestamp_flags::BaseStage, false ) );
}

TEST( Subspace_test, get_iterators_6 ){
  Subspace a( "i", 2 );
  a.set_stage( timestamp_flags::BaseStage + 1000 );
  ASSERT_EQ( "", a.get_iterators( timestamp_flags::BaseStage, true ) );
}

TEST( Subspace_test, const_index_test_1 ){
  Subspace a("i",1);
  ASSERT_EQ( a.const_index, 1 );
}

TEST( Subspace_test, const_index_test_2 ){
  Subspace a("i",100);
  ASSERT_EQ( a.const_index, 100 );
}

TEST( Subspace_test, const_index_test_3 ){
  Subspace a("i",0);
  ASSERT_EQ( a.const_index, 0 );
}

TEST( Subspace_test, access_operator_1 ){
  Subspace a( "i", 2 );
  ASSERT_NO_THROW( a[0]; );
  ASSERT_NO_THROW( a[1]; );
  ASSERT_NO_THROW( a[2]; );
  ASSERT_NO_THROW( a[a.const_index]; );

  ASSERT_EQ( "i_0", a[0] );
  ASSERT_EQ( "i_1", a[1] );
  ASSERT_EQ( "i_c", a[2] );
  ASSERT_EQ( "i_c", a[a.const_index] );
}

TEST( SubspaceManager_test, construct ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );

  ASSERT_NO_THROW( SubspaceManager manager( loops, iterators ); );
}

TEST( SubspaceManager_test, iterators_1 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  ASSERT_NE( manager.begin(), manager.end() );
}

TEST( SubspaceManager_test, iterators_2 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  ASSERT_EQ( ++(++(manager.begin())), manager.end() );
}

TEST( SubspaceManager_test, iterators_3 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  SubspaceManager::iterator iter = manager.begin();
  // Currently points to loops
  {
    Subspace::iterator loops_iter = loops->begin();
    Subspace::iterator iter_iter = (*iter)->begin();
    for( ; loops_iter != loops->end() && iter_iter != (*iter)->end();
           ++loops_iter, ++iter_iter ){
      ASSERT_EQ( loops_iter, iter_iter );
      ASSERT_EQ( *loops_iter, *iter_iter );
    }
    ASSERT_EQ( loops_iter, loops->end() );
    ASSERT_EQ( iter_iter, (*iter)->end() );
  }

  ++iter;
  // Currently points to loops
  {
    Subspace::iterator iterators_iter = iterators->begin();
    Subspace::iterator iter_iter = (*iter)->begin();
    for( ; iterators_iter != iterators->end() && iter_iter != (*iter)->end();
           ++iterators_iter, ++iter_iter ){
      ASSERT_EQ( iterators_iter, iter_iter );
      ASSERT_EQ( *iterators_iter, *iter_iter );
    }
    ASSERT_EQ( iterators_iter, iterators->end() );
    ASSERT_EQ( iter_iter, (*iter)->end() );
  }
}

TEST( SubspaceManager_test, get_nest ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* nest = new Subspace( "i", 2 );
  SubspaceManager manager( loops, nest );

  ASSERT_EQ( *nest, *(manager.get_nest()) );
}

TEST( SubspaceManager_test, get_loops ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* nest = new Subspace( "i", 2 );
  SubspaceManager manager( loops, nest );

  ASSERT_EQ( *loops, *(manager.get_loops()) );
}

TEST( SubspaceManager_test, get_iterator_to_nest ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* nest = new Subspace( "i", 2 );
  SubspaceManager manager( loops, nest );

  ASSERT_EQ( *nest, **(manager.get_iterator_to_nest()) );
}

TEST( SubspaceManager_test, get_iterator_to_loops ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* nest = new Subspace( "i", 2 );
  SubspaceManager manager( loops, nest );

  ASSERT_EQ( *loops, **(manager.get_iterator_to_loops()) );
}

TEST( SubspaceManager_test, get_in_out_advance_stages ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  for( int i = 0; i < 100; i += 1 ){
    ASSERT_EQ( manager.get_current_stage(), timestamp_flags::BaseStage + i );
    ASSERT_EQ( manager.get_input_stage(), timestamp_flags::BaseStage + i );
    ASSERT_EQ( manager.get_input_stage(), manager.get_current_stage() );
    ASSERT_EQ( manager.get_output_stage(), timestamp_flags::BaseStage + i + 1 );
    ASSERT_EQ( manager.get_output_stage(), manager.get_input_stage() + 1 );
    ASSERT_EQ( manager.advance_stage(), timestamp_flags::BaseStage + i + 1 );
  }

}

TEST( SubspaceManager_test, insert_left_1 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  Subspace* test = new Subspace( "test", 1 );
  manager.insert_left( test, manager.begin() );
  ASSERT_EQ( **(manager.begin()), *test );
}

TEST( SubspaceManager_test, insert_left_2 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  Subspace* test = new Subspace( "test", 1 );
  manager.insert_left( test, std::next(manager.begin()) );
  ASSERT_EQ( **(std::next(manager.begin())), *test );
}

TEST( SubspaceManager_test, insert_left_3 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  Subspace* test = new Subspace( "test", 1 );
  manager.insert_left( test, manager.end() );
  ASSERT_EQ( **(std::prev(manager.end())), *test );
}

TEST( SubspaceManager_test, insert_right_1 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  Subspace* test = new Subspace( "test", 1 );
  manager.insert_right( test, manager.begin() );
  ASSERT_EQ( **(std::next(manager.begin())), *test );
}

TEST( SubspaceManager_test, insert_right_2 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  Subspace* test = new Subspace( "test", 1 );
  manager.insert_right( test, std::next(manager.begin()) );
  ASSERT_EQ( **(std::next(std::next(manager.begin()))), *test );
}

TEST( SubspaceManager_test, insert_right_3 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  Subspace* test = new Subspace( "test", 1 );
  manager.insert_right( test, std::prev(manager.end()) );
  ASSERT_EQ( **(std::prev(manager.end())), *test );
}

TEST( SubspaceManager_test, insert_get_iterator_to_nest ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* nest = new Subspace( "i", 2 );
  SubspaceManager manager( loops, nest );
  Subspace* test = new Subspace( "t", 1 );

  manager.insert_left( test, manager.get_iterator_to_nest() );
  ASSERT_EQ( *test, **( std::prev(manager.get_iterator_to_nest()) ) );
  ASSERT_EQ( manager.get_iterators( timestamp_flags::AllStages, true), "loops_c,t_0,t_c,i_0,i_1,i_c" );
}

TEST( SubspaceManager_test, insert_get_iterator_to_loops ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* nest = new Subspace( "i", 2 );
  SubspaceManager manager( loops, nest );
  Subspace* test = new Subspace( "t", 1 );

  manager.insert_left( test, manager.get_iterator_to_loops() );
  ASSERT_EQ( *test, **( std::prev(manager.get_iterator_to_loops()) ) );
  ASSERT_EQ( manager.get_iterators( timestamp_flags::AllStages, true), "t_0,t_c,loops_c,i_0,i_1,i_c" );
}

TEST( SubspaceManager_test, get_iterators_1 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  ASSERT_EQ( manager.get_iterators( manager.get_current_stage(), true ), "loops_c,i_0,i_1,i_c" );
}

TEST( SubspaceManager_test, get_iterators_2 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );

  ASSERT_EQ( manager.get_iterators( timestamp_flags::AllStages, true ), "loops_c,i_0,i_1,i_c" );
}

TEST( SubspaceManager_test, get_iterators_3 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );
  manager.insert_left( new Subspace( "test", 2 ), manager.begin() );

  ASSERT_EQ( manager.get_iterators( timestamp_flags::AllStages, true ), "test_0,test_1,test_c,loops_c,i_0,i_1,i_c" );
}

TEST( SubspaceManager_test, get_iterators_4 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );
  manager.insert_left( new Subspace( "test", 2 ), manager.end() );

  ASSERT_EQ( manager.get_iterators( timestamp_flags::AllStages, true ), "loops_c,i_0,i_1,i_c,test_0,test_1,test_c" );
}

TEST( SubspaceManager_test, get_iterators_5 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );
  manager.insert_left( new Subspace( "test", 2 ), manager.end() );

  ASSERT_EQ( manager.get_iterators( timestamp_flags::AllStages, true ), "loops_c,i_0,i_1,i_c,test_0,test_1,test_c" );
}

TEST( SubspaceManager_test, aliased_get_iterators_1 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );
  Subspace* test = new Subspace( "test", 2 );
  manager.insert_left( test, manager.begin() );
  test->set_aliased();

  ASSERT_EQ( manager.get_iterators( timestamp_flags::AllStages, true ), "alias_test_0,alias_test_1,alias_test_c,loops_c,i_0,i_1,i_c" );
}

TEST( SubspaceManager_test, aliased_get_iterators_2 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );
  SubspaceManager manager( loops, iterators );
  Subspace* test = new Subspace( "test", 2 );
  manager.insert_left( test, manager.begin() );
  test->set_aliased();

  ASSERT_EQ( manager.get_iterators( timestamp_flags::AllStages, false ), "test_0,test_1,test_c,loops_c,i_0,i_1,i_c" );
}

TEST( SubspaceManager_test, input_output_1 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );

  SubspaceManager manager( loops, iterators );

  Subspace* test = new Subspace( "test", 2 );
  manager.insert_left( test, manager.begin() );

  ASSERT_EQ( manager.get_input_iterators( ), "loops_c,i_0,i_1,i_c" );
  ASSERT_EQ( manager.get_output_iterators(), "test_0,test_1,test_c,loops_c,i_0,i_1,i_c" );
  manager.advance_stage();
  ASSERT_EQ( manager.get_input_iterators( ), "test_0,test_1,test_c,loops_c,i_0,i_1,i_c" );
  ASSERT_EQ( manager.get_output_iterators(), "test_0,test_1,test_c,loops_c,i_0,i_1,i_c" );
}

TEST( SubspaceManager_test, input_output_3){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* iterators = new Subspace( "i", 2 );

  SubspaceManager manager( loops, iterators );

  Subspace* test = new Subspace( "test", 2 );
  manager.insert_left( test, manager.begin() );

  ASSERT_EQ( manager.get_input_iterators( ), "loops_c,i_0,i_1,i_c" );
  ASSERT_EQ( manager.get_output_iterators(), "test_0,test_1,test_c,loops_c,i_0,i_1,i_c" );
  manager.next_stage();
  manager.get_loops()->set_aliased();
  ASSERT_EQ( manager.get_input_iterators( ), "test_0,test_1,test_c,loops_c,i_0,i_1,i_c" );
  ASSERT_EQ( manager.get_output_iterators(), "test_0,test_1,test_c,alias_loops_c,i_0,i_1,i_c" );
  manager.next_stage();
  ASSERT_EQ( manager.get_input_iterators( ), "test_0,test_1,test_c,loops_c,i_0,i_1,i_c" );
  ASSERT_EQ( manager.get_output_iterators(), "test_0,test_1,test_c,loops_c,i_0,i_1,i_c" );
}

TEST( SubspaceManager_test, demo_1 ){
  Subspace* loops = new Subspace( "loops", 0 );
  Subspace* nest = new Subspace( "i", 2 );

  SubspaceManager manager( loops, nest );

  ASSERT_EQ( manager.get_input_iterators( ), "loops_c,i_0,i_1,i_c" );
  ASSERT_EQ( manager.get_output_iterators(), "loops_c,i_0,i_1,i_c" );

  manager.next_stage();

  Subspace* tiles = new Subspace( "t", 2, *manager.get_nest() );
  manager.insert_left( tiles, manager.get_iterator_to_nest() );
  ASSERT_EQ( manager.get_input_iterators( ), "loops_c,i_0,i_1,i_c" );
  ASSERT_EQ( manager.get_output_iterators(), "loops_c,t_i_0,t_i_1,t_i_c,i_0,i_1,i_c" );

  manager.next_stage();

  Subspace* chunk = new Subspace( "t", 1, *tiles );
  manager.insert_left( chunk, manager.get_iterator_to_subspace(tiles) );
  ASSERT_EQ( manager.get_input_iterators( ), "loops_c,t_i_0,t_i_1,t_i_c,i_0,i_1,i_c" );
  ASSERT_EQ( manager.get_output_iterators(), "loops_c,t_t_i_0,t_t_i_c,t_i_0,t_i_1,t_i_c,i_0,i_1,i_c" );

  manager.next_stage();
  manager.get_nest()->set_aliased();
  manager.get_loops()->set_aliased();
  ASSERT_EQ( manager.get_input_iterators( ), "loops_c,t_t_i_0,t_t_i_c,t_i_0,t_i_1,t_i_c,i_0,i_1,i_c" );
  ASSERT_EQ( manager.get_output_iterators(), "alias_loops_c,t_t_i_0,t_t_i_c,t_i_0,t_i_1,t_i_c,alias_i_0,alias_i_1,alias_i_c" );
}
