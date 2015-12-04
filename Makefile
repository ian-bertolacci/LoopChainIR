# Paths
PROJECT_DIR=$(PWD)

BIN=$(PROJECT_DIR)/bin
SRC=$(PROJECT_DIR)/src

TEST=$(PROJECT_DIR)/test
UNIT_TEST_DIR=$(TEST)/unit-tests
UNIT_TEST_BIN=$(UNIT_TEST_DIR)/bin
UNIT_TEST_SRC=$(UNIT_TEST_DIR)/src
REG_TEST_DIR=$(TEST)/regression-tests
REG_TEST_BIN=$(REG_TEST_DIR)/bin
REG_TEST_DIR=$(REG_TEST_DIR)/src

THIRD_PARTY=$(PROJECT_DIR)/third-party
THIRD_PARTY_SRC=$(THIRD_PARTY)/source
THIRD_PARTY_BUILD=$(THIRD_PARTY)/build
THIRD_PARTY_INSTALL=$(THIRD_PARTY)/install

LIB=$(THIRD_PARTY_INSTALL)/lib
INC=$(THIRD_PARTY_INSTALL)/include

# Global Variables
MAKE_JOBS=8

# Compiler and flags
CXX=g++
CXXFLAGS += -g -Wall -Wextra -pthread
CPPFLAGS += -isystem $(INC) -lisl

# Test Variables
GTEST_DIR=$(THIRD_PARTY_INSTALL)/gtest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

UNIT_TESTS = RectangularDomain_test LoopNest_test LoopChain_test DefaultSequentialSchedule_test

# Project object files and executable
OBJS = $(BIN)/RectangularDomain.o \
       $(BIN)/LoopChain.o \
			 $(BIN)/LoopNest.o \
			 $(BIN)/DefaultSequentialSchedule.o \
			 $(BIN)/util.o

# What thing are we actually making?
EXE=$(BIN)/SomethingSomethingSomething

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(LIB) $^ -o $(EXE)

$(BIN)/RectangularDomain.o: $(SRC)/RectangularDomain.h $(SRC)/RectangularDomain.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(LIB) -I/$(SRC) $(SRC)/RectangularDomain.cpp -c -o $@

$(BIN)/LoopChain.o: $(SRC)/LoopChain.h $(SRC)/LoopChain.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(LIB) -I/$(SRC) $(SRC)/LoopChain.cpp -c -o $@

$(BIN)/LoopNest.o: $(SRC)/LoopNest.h $(SRC)/LoopNest.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(LIB) -I/$(SRC) $(SRC)/LoopNest.cpp -c -o $@

$(BIN)/DefaultSequentialSchedule.o: $(SRC)/DefaultSequentialSchedule.h $(SRC)/DefaultSequentialSchedule.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(LIB) -I/$(SRC) $(SRC)/DefaultSequentialSchedule.cpp -c -o $@

$(BIN)/util.o: $(SRC)/util.h $(SRC)/util.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(LIB) -I/$(SRC) $(SRC)/util.cpp -c -o $@

all-tests: unit-tests regression-tests

unit-tests: $(UNIT_TESTS)

regression-tests:

$(UNIT_TESTS): $(UNIT_TEST_BIN)/gtest_main.a $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC) -c $(UNIT_TEST_SRC)/$@.cpp -o $(UNIT_TEST_BIN)/$@.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC) -L$(LIB) -lpthread $(UNIT_TEST_BIN)/$@.o $(OBJS) $(UNIT_TEST_BIN)/gtest_main.a -o $(UNIT_TEST_BIN)/$@
	$(UNIT_TEST_BIN)/$@

$(UNIT_TEST_BIN)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(UNIT_TEST_BIN)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(UNIT_TEST_BIN)/gtest.a : $(UNIT_TEST_BIN)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(UNIT_TEST_BIN)/gtest_main.a : $(UNIT_TEST_BIN)/gtest-all.o $(UNIT_TEST_BIN)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

genesis: nuke
	mkdir $(THIRD_PARTY_INSTALL) $(THIRD_PARTY_BUILD)
	mkdir $(LIB)
	mkdir $(INC)
	#
	# Unzip and copy gtest
	unzip -q $(THIRD_PARTY_SRC)/gtest-1.7.0.zip -d $(THIRD_PARTY_INSTALL)/.
	mv $(THIRD_PARTY_INSTALL)/gtest-1.7.0 $(GTEST_DIR)
	#
	# build ISL
	tar -xzf $(THIRD_PARTY_SRC)/isl-0.15.tar.gz -C $(THIRD_PARTY_BUILD)/.
	mv $(THIRD_PARTY_BUILD)/isl-0.15 $(THIRD_PARTY_BUILD)/isl
		 cd $(THIRD_PARTY_BUILD)/isl \
	&& ./configure --prefix=$(THIRD_PARTY_INSTALL) \
	&& make -j$(MAKE_JOBS) \
	&& make install

neat:
	- rm -r $(BIN)/*.o $(BIN)/*.a

clean-third-party:
	- rm -rf $(THIRD_PARTY_INSTALL) $(THIRD_PARTY_BUILD)

clean-test:
	- rm $(UNIT_TEST_BIN)/*
	- rm $(REG_TEST_BIN)/*
	- rm $(REG_TEST_SANDBOX)/*

clean: clean-test
	- rm -r $(BIN)/*

nuke: clean-third-party clean
