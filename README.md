# LoopChainIR
The code repository for the internal representation data structure that holds a
loop chain execution schedule.

## Getting Started
The entire build process is controlled through the Makefile at the root of this
project.

After getting this repository (via git clone, zip download, carrier pidgeon,
what have you ), run:

`make initialize`

This will build and position all third-party materials in the correct place.
To ensure that everything is in working order, run:

`make all-tests`

## Project Directory Structure
* bin/ : Where all executables (including intermidiate \*.o and \*.a files)
are compiled to. Known as $(BIN).
* src/ : Where all the soures for the binary files live. Known as $(SRC).
* test/ : Subdirectory for testing. Known as $(TEST)
  + regression-tests : Subdirectory for regression-test files. Known as $(REG_TEST_DIR)
  + unit-tests : Subdirectory for unit-test files. Known as $(UNIT_TEST_DIR)
    - bin/ : Where all executalbes (including intermidiate \*.o and \*.a files)
    _only_ realating to the unit tests are compiled to. Known as $(UNIT_TEST_BIN)
    - src/ : Where all the source for the unit tests live. Known as $(UNIT_TEST_SRC)
* util/ : Where utility resources (such as scripts) live. Known as $(UTIL)
* documentation/: Where doxygen outputs its files. Known as $(DOC_PATH)
* third-party/ : Subdirectory for third-party materials, their source,
their build directories, and their install directories. Known as $(THIRD_PARTY)
  + source/ : The original source distributions of third-party materials. Known
    as $(THIRD_PARTY_SRC)
  + build/: Where third-party materials are extracted to and built. Created
    after `make genesis`. Known as $(THIRD_PARTY_BUILD)
  + install/ : Where third-party materials are installed to. Created after
    `make genesis`. Known as $(THIRD_PARTY_INSTALL)
    - lib/ : Third-party libraries. Known as $(LIB)
    - include/ : Third-party headers. Known as $(INC)

## Make commands
* `initialize` (or `init`): This is the first command that should be run when getting started
  with the project. It builds and places all the third-party tools into good
  organized places.
* `all`: builds the LoopChainIR.a library
* `all-tests`: performs `unit-tests` and `regression-tests`
* `unit-tests`: Runs all unit tests specified by $(UNIT-TESTS)
* `regression-tests`: Runs all regression tests specifed by $(REG_TESTS)
* `documentation` (or `doc`): Runs doxygen, uses the local Doxyfile
* `neat`: removes all \*.o files from $(BIN).
* `clean-third-party`: removes (recursively, **forced**) $(THIRD_PARTY_INSTALL)
  and $(THIRD_PARTY_BUILD).
* `clean-test`: removes all files from $(TESTBIN)
* `clean-doc`: Removes the $(DOC_PATH) directory
* `clean`: performs `clean-test` and removes (recursively) all files under
  $(BIN).
* `clean-all`: performs `clean`, `clean-third-party`, and clean-doc. Mimics a restored project
  state.

## Testing
Tests are run with

`make all-tests`

### Unit Testing
Testing is conducted through the [Google Test](https://code.google.com/p/googletest/)
unit test framework.

Preferably, each project source file gets its own unit test file (i.e. RectangularDomain.cpp
and RectangularDomain_test.cpp).

Tests are run with

`make unit-tests`

Or individually using the name of the test, for example:

`make RectangularDomain_test`

### Regression Testing
Regression testing is conducted by an in-house script (see $(UTIL)/regression-util.py).

Test files are placed in $(REG_TEST_DIR).

Tests are run with

`make regression-tests`

Or individually using the name of the test.

A regression test has several sections:
* `test name` : Name of the test. Perfers no white-space.

* `loop chain` : An ordered list of each loop nest, where each loop nest is expressed as a terse domain.

  Example:
  ```
  loop chain:
  (i){0..9}
  (i){0..9}
  (i,j){0..M-1,0..N-1}
  :end
  ```
  The iterators (symbols in the parentheses) are comma seperated and can be any valid C variable symbol.
  The bounds (expressions in the brackets) as comma seperated list of ranges.
  Each range is expressed `lower_bound .. upper_bound`, and bounds can contain simple expressions (using + - * / % operators and parentheses, no function calls)
  The iterators and bounds are mached up by their index (i.e. the first iterator is bounded by the first range, and the second iterator by the second, and so on.)

* `schedule` : (Currently InOp, since we only have the default schedule) An ordered list of transformations to be applied to the loop chain.
  Transformations occur in the order listed.

* `dependencies` : The dependencies that _must_ be satisfied for _any_ transformation on the loop.
  These are used to ensure that a transformation produced a loop satisfying the dependencies of the original loop.
  Each iteration must be expressed in full-form syntax `[loop nest, iterator, (loop nest, iterator,)* statement]`.
  Further, they must be padded so that all iterations are the same length
  For example, the statement 'inside' the first loop is expressed in full-form as `[0,i,0]`, since its the 0'th loop, has iteration i, and is the 0'th statement.
  With padding it becomes `[0,i,0,0,0]`

  Code Example, continuing with loop chain example:
  ```
  dependencies:
  { [0,i,0,0,0] -> [0,i+1,0,0,0] : 0 <= i < 10 }
  { [1,i,0,0,0] -> [1,i+1,0,0,0] : 0 <= i < 10 }
  [N,M] -> { [2,i,0,j,0] -> [2,i,0,j+1,0] : 0 <= i < M and 0 <= j < N }
  [N,M] -> { [2,i,0,j,0] -> [2,i+1,0,j,0] : 0 <= i < M and 0 <= j < N }
  [N,M] -> { [0,i,0,0,0] -> [2,i',0,j,0] : 0 <= i < 10 and 0 <= i < M and 0 <= j < N }
  :end
  ```
  The first line indicates that iteration i comes before iteration i+1 in the first loop.
  The second line indicates that iteration i comes before iteration i+1 in the second loop.
  The third line indicates that iteration j comes before iteration j+1 in the third loop.
  The fourth line indicates that iteration i comes before iteration i+1 in the second loop.
  The fifth line indicates that all iterations in the first loop come before any iterations in the second loop.

* `new ordering` : The dependencies that _must_ be satisfied for the _particular_ transformation.
  This is written in the same way as the dependencies section.
  If the transformation being tested is the `original` schedule, then the text will probably be identical.
  Code Example, continuing with loop chain example, performing a loop fusion on the first two loops:
  ```
  new ordering:
  { [1,i,0,0,0] -> [0,i+1,0,0,0] : 0 <= i < 10}

  { [0,i,0,0,0] -> [0,i+1,0,0,0] : 0 <= i < 10 }
  { [1,i,0,0,0] -> [1,i+1,0,0,0] : 0 <= i < 10 }
  [N,M] -> { [1,i,0,j,0] -> [1,i,0,j+1,0] : 0 <= i < M and 0 <= j < N }
  [N,M] -> { [1,i,0,j,0] -> [1,i+1,0,j,0] : 0 <= i < M and 0 <= j < N }
  [N,M] -> { [0,i,0,0,0] -> [1,i',0,j,0] : 0 <= i < 10 and 0 <= i < M and 0 <= j < N }
  :end
  ```
  The only change from the dependency code is the first line, which indicates that iteration i in the second loop comes before iteration i+1 in the first loop.

* `exemplar` : (Optional) (Currently InOp) Actual original C/C++ code that should be tested by the source-to-source transformation tool (which doesnt exist yet).
  Code Example:
  ```
  exemplar:
  for( int i = 0; i < 10; i += 1 ){
    A[i] = Static[i];
  }
  for( int i = 0; i <= 9; i++ ){
    B[i] = f( A[i] );
  }
  for( int i = 0; i < M; ++i ){
    for( int j = 0; j <= N-1; j = j + 1 ){
      C[i,j] = A[i%10] / B[i%10];
    }
  }
  :end
  ```

The full test (using all the examples) would look something like this:
```
test name: example_test

loop chain:
(i){0..9}
(i){0..9}
(i,j){0..M-1,0..N-1}
:end

dependencies:
{ [0,i,0,0,0] -> [0,i+1,0,0,0] : 0 <= i < 10 }
{ [1,i,0,0,0] -> [1,i+1,0,0,0] : 0 <= i < 10 }
[N,M] -> { [2,i,0,j,0] -> [2,i,0,j+1,0] : 0 <= i < M and 0 <= j < N }
[N,M] -> { [2,i,0,j,0] -> [2,i+1,0,j,0] : 0 <= i < M and 0 <= j < N }
[N,M] -> { [0,i,0,0,0] -> [2,i',0,j,0] : 0 <= i < 10 and 0 <= i < M and 0 <= j < N }
:end

schedule:
fuse
:end

new ordering:
{ [1,i,0,0,0] -> [0,i+1,0,0,0] : 0 <= i < 10}

{ [0,i,0,0,0] -> [0,i+1,0,0,0] : 0 <= i < 10 }
{ [1,i,0,0,0] -> [1,i+1,0,0,0] : 0 <= i < 10 }
[N,M] -> { [1,i,0,j,0] -> [1,i,0,j+1,0] : 0 <= i < M and 0 <= j < N }
[N,M] -> { [1,i,0,j,0] -> [1,i+1,0,j,0] : 0 <= i < M and 0 <= j < N }
[N,M] -> { [0,i,0,0,0] -> [1,i',0,j,0] : 0 <= i < 10 and 0 <= i < M and 0 <= j < N }
:end

exemplar:
for( int i = 0; i < 10; i += 1 ){
  A[i] = Static[i];
}
for( int i = 0; i <= 9; i++ ){
  B[i] = f( A[i] );
}
for( int i = 0; i < M; ++i ){
  for( int j = 0; j <= N-1; j = j + 1 ){
    C[i,j] = A[i%10] / B[i%10];
  }
}
:end
```

## Third-Party materials
Included with this project are several third-party materials under the
$(THIRD_PARTY_SRC) directory.

1. ISL
  + [isl-0.15.tar.gz](http://isl.gforge.inria.fr/isl-0.15.tar.gz)
  + Under MIT License
  + Requires GMP (Not provided).
2. Google Test
  + [gtest-1.7.0.zip](https://code.google.com/p/googletest/downloads/detail?name=gtest-1.7.0.zip)
  + Under BSD 3-Clause License

All third-party materials are provided as they were downloaded, and include
their copyright and license information.
