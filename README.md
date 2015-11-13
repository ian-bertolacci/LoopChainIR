# LoopChainIR
The code repository for the internal representation data structure that holds a
loop chain execution schedule.

## Getting Started
The entire build process is controlled through the Makefile at the root of this
project.

After getting this repository (via git clone, zip download, carrier pidgeon,
what have you ), run:

`make genesis`

This will build and position all third-party materials in the correct place.
To ensure that everything is in working order, run:

`make tests`

All tests should, unless stated otherwise.

## Project Directory Structure
* bin/ : Where all executables (including intermidiate \*.o and \*.a files)
are compiled to. Known as $(BIN).
* src/ : Where all the soures for the binary files live. Known as $(SRC).
* test/ : Subdirectory for unit testing. Known as $(TEST)
  + bin/ : Where all executalbes (including intermidiate \*.o and \*.a files)
  _only_ realating to the unit tests are compiled to. Known as $(TEST_BIN)
  + src/ : Where all the source for the unit tests live. Known as $(TEST_SRC)
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
* `geneis`: This is the first command that should be run when getting started
  with the project. It builds and places all the third-party tools into good
  organized places.
* `all`: Currently inoperable. Need to know what is being built (libray?
  executable?)
* `tests`: builds and runs all tests specified by the $(TESTS) variable.
* `neat`: removes (recursively) all \*.o and \*.a files from $(BIN).
* `clean-third-party`: removes (recursively, **forced**) $(THIRD_PARTY_INSTALL)
  and $(THIRD_PARTY_BUILD).
* `clean-test`: removes all files from $(TESTBIN)
* `clean`: performs `clean-test` and removes (recursively) all files under
  $(BIN).
* `nuke`: performs `clean` and `clean-third-party`. Mimics a restored project
  state.

## Unit testing
Testing is conducted through the [Google Test](https://code.google.com/p/googletest/)
unit test framework.

Preferably, each project source file gets its own unit test file (i.e. RectangularDomain.cpp
and RectangularDomain_test.cpp).

Tests are run with

`make tests`

Or individually using the name of the test, for example:

`make RectangularDomain_test`

## Third-Party materials
Included with this project are several third-party materials under the
$(THID_PARTY_SRC) directory.

1. ISL
  + [isl-0.15.tar.gz](http://isl.gforge.inria.fr/isl-0.15.tar.gz)
  + Under MIT License
2. Google Test
  + [gtest-1.7.0.zip](https://code.google.com/p/googletest/downloads/detail?name=gtest-1.7.0.zip)
  + Under BSD 3-Clause License

All third-party materials are provided as they were downloaded, and include
their copyright and license information.
