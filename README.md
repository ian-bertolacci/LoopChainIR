# LoopChainIR
The code repository for the internal representation data structure that holds a
loop chain execution schedule.

## Getting Started
The entire build process is controlled through the Makefile at the root of this
project.

After getting this repository (via git clone, zip download, carrier pigeon,
what have you ), run:

`make initialize`

This will build and position all third-party materials in the correct place.
To ensure that everything is in working order, run:

`make all-tests`

## Project Directory Structure
* bin/ : Where all executables (including intermediate \*.o and \*.a files)
are compiled to. Known as $(BIN).

* src/ : Where all the sources for the binary files live. Known as $(SRC).

* test/ : Subdirectory for testing. Known as $(TEST)

  + regression-tests : Subdirectory for regression-test files. Known as $(REG_TEST_DIR)

  + unit-tests : Subdirectory for unit-test files. Known as $(UNIT_TEST_DIR)

    - bin/ : Where all executables (including intermediate \*.o and \*.a files)
    _only_ relating to the unit tests are compiled to. Known as $(UNIT_TEST_BIN)

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
  with the project. It builds and places all the third-party tools into good organized places.

* `all`: Builds the LoopChainIR.a library

* `all-tests`: Performs `unit-tests` and `regression-tests`

* `unit-tests`: Runs all unit tests specified by $(UNIT-TESTS)

* `regression-tests`: Runs all regression tests specified by $(REG_TESTS)

* `documentation` (or `doc`): Runs doxygen, uses the local Doxyfile

* `neat`: Removes all \*.o files from $(BIN).

* `clean-third-party`: Removes (recursive, **forced**) $(THIRD_PARTY_INSTALL) and $(THIRD_PARTY_BUILD).

* `clean-test`: Removes (recursive) all files from $(UNIT_TEST_BIN), and \*.dir and \*.log files from $(REG_TEST_DIR)

* `clean-doc`: Removes (recursive) the $(DOC_PATH) directory

* `clean`: Performs `clean-test` and removes (recursively) all files under
  $(BIN).

* `clean-all`: Performs `clean`, `clean-third-party`, and `clean-doc`. Mimics a restored project state.

## Documentation
The LoopChainIR API is documented using Doxygen.

To generate the HTML documentation, run

```
make documentation
```
or
```
make doc
```

The output can be found at $(DOC)

## Testing
A complete run of all tests (both regression and unit) is run with

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

If a test fails, it's log file is dropped next to the test file suffixed with '.log'.
This can be useful to determine what failed, how to fix it, and in the case of a software error is useful to us.

#### Regression Test Files
A regression test has several sections:
* `test name` : Name of the test. Prefers no white-space.

* `loop chain` : An ordered list of each loop nest, where each loop nest is expressed as a terse domain.

  Example:
  ```
  loop chain:
  (i){0..9}
  (i){0..9}
  (i,j){0..M-1,0..N-1}
  :end
  ```
  The iterators (symbols in the parentheses) are comma separated and can be any valid C variable symbol.
  The bounds (expressions in the brackets) as comma separated list of ranges.
  Each range is expressed `lower_bound .. upper_bound`, and bounds can contain simple expressions (using + - * / % operators and parentheses, no function calls)
  The iterators and bounds are matched up by their index (i.e. the first iterator is bounded by the first range, and the second iterator by the second, and so on.)

* `schedule` : There are three schedule {things}
  + Original: default sequential schedule for a loop chain.
    code: `original`  
    regex: `original`   

  + Fusion schedule: Fuse a series of loops.  
    code: `fuse {loop index} {loop index} [{loop index} ...]`  
    regex: `"fuse\s+(?P<list>(?:\d+\s*){2,})`  
    Example: `fuse 0 1` fuses loops 0 and 1 (the first and second loops).

  + Shift: shift the domain of a loop by some extent(s).
    Extents can be constant integers, symbols, and valid C expressions using only arithmetic operators.
    There must be the same number of extents as dimensions of the domain of the loop.  
    code: `shift {loop index} ({extent} [,{extent} ...])`  
    regex: `shift\s+(?P<loopid>\d+)\s+\(\s*(?P<extents>.+)\s*\)`  
    Example: `shift 0 (1,K,n+3)` shifts the first loop in the chain, which is 3D,by 1, K, and n+3.  

  In theory (but not yet tested) these schedules can be composed.
  They are (or will be) applied in the order specified.

* `dependencies` : The dependencies that _must_ be satisfied for _any_ transformation on the loop.
  These are used to ensure that a transformation produced a loop satisfying the dependencies of the original loop.
  This is expressed in traditional [ISCC](http://compsys-tools.ens-lyon.fr/iscc/barvinok.pdf) syntax.
  However, unlike in ISCC, each iteration must be expressed in full-form syntax `[loop nest, iterator, (loop nest, iterator,)* statement]`.
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

  Please note that each dependency is treated by the framework as a separate entity, and is split by newlines.
  This means that a dependency expression currently cannot span multiple lines.

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

* `exemplar` : (Optional) (Currently InOp) Actual original C/C++ code that should be tested by the source-to-source transformation tool (which doesn't exist yet).
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

#### regression-util.py
The regression-util.py script lives in $(UTIL) and is the driver behind the regression testing framework.

To run a test (or list of tests) by hand, simply calls

```
python $(UTIL)/regression-util.py path/to/file.test path/to/file2.test
```

There are options for the script:
* --save_env (or -se): Save the testing environment after a test completes.
  By default, test environments are erased when test completes, regardless of failure state.

* --save_env_on_fail (or -sf): Save the testing environment after a test fails.
  By default, test environments are erased when test completes, regardless of failure state.

* --save_log (or -sl): Save the log file, even if test is successful.
  By default, logs are only saved to file if a test fails.

* --resources_path PATH (or -r): Give the script a path to $(UTIL)/resources.
  By default, the script assumes that it lives in the same directory as the resources folder and will construct the path from its call.

* --project_path PATH (or -p): Give the script a path to the LoopChainIR project directory.
  By default, the script assumes that it will be called from the LoopChainIr project root directory, and thus the path would be equivalent to ".".

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
