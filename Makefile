# Paths
PROJECT_DIR ?= $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

BIN=$(PROJECT_DIR)/bin
SRC=$(PROJECT_DIR)/src
UTIL=$(PROJECT_DIR)/util
LIB=$(PROJECT_DIR)/lib

TEST=$(PROJECT_DIR)/test
UNIT_TEST_DIR=$(TEST)/unit-tests
UNIT_TEST_BIN=$(UNIT_TEST_DIR)/bin
UNIT_TEST_SRC=$(UNIT_TEST_DIR)/src
REG_TEST_DIR=$(TEST)/regression-tests

THIRD_PARTY=$(PROJECT_DIR)/third-party
THIRD_PARTY_SRC=$(THIRD_PARTY)/source
THIRD_PARTY_BUILD=$(THIRD_PARTY)/build
THIRD_PARTY_INSTALL=$(THIRD_PARTY)/install

INITED_FILE=$(THIRD_PARTY)/initialized

DOC_PATH=$(PROJECT_DIR)/doxygen

SOURCE_LIB = $(THIRD_PARTY_INSTALL)/lib
SOURCE_INC = $(THIRD_PARTY_INSTALL)/include

# Global Variables
MAKE_JOBS=2

# Compiler and flags
CXX=g++
CXXFLAGS += -g -Wall -Wextra -Werror -pthread
CPPFLAGS += --std=c++11 -isystem $(SOURCE_INC)

# Test Variables
GTEST_DIR=$(THIRD_PARTY_INSTALL)/gtest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

UNIT_TESTS = RectangularDomain_test \
						 LoopNest_test \
						 LoopChain_test \
						 Schedule_test \
						 DefaultSequentialTransformation_test \
						 FusionTransformation_test \
						 ShiftTransformation_test

REG_TESTS = 1N_1D_shift_1.test \
						1N_1D_shift_K.test \
						1N_1D.test \
						1N_2D_shift_1_2.test \
						1N_2D_shift_K_exp.test \
						1N_2D.test \
						1N_3D.test \
						2N_1D_2D.test \
						2N_1D_fuse.test \
						2N_1D_shift_1.test \
						2N_1D_shift_K.test \
						2N_1D.test \
						2N_2D_fuse.test \
						2N_2D.test \
						2N_3D.test \
						2N_3D_fuse.test \
						3N_1D_2D_3D.test \
						3N_3D_2D_1D.test \
						example.test

# Project object files and executable
OBJS = $(BIN)/RectangularDomain.o \
       $(BIN)/LoopChain.o \
			 $(BIN)/LoopNest.o \
			 $(BIN)/Schedule.o \
			 $(BIN)/DefaultSequentialTransformation.o \
			 $(BIN)/FusionTransformation.o \
			 $(BIN)/ShiftTransformation.o \
			 $(BIN)/util.o


# Linkable library
EXE=$(LIB)/libloopchainIR.a

all: $(EXE)

$(EXE): $(OBJS) $(INITED_FILE)
	$(AR) $(ARFLAGS) $@ $^

# Building the Ojbect Files
$(OBJS): $(BIN)/%.o : $(SRC)/%.cpp $(SRC)/%.hpp $(INITED_FILE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC) $< -c -o $@

# Testing
all-tests: unit-tests regression-tests

unit-tests: $(UNIT_TESTS)

regression-tests: $(EXE)
	python $(UTIL)/regression-util.py -r $(UTIL)/resources -p $(PROJECT_DIR) $(addprefix $(REG_TEST_DIR)/,$(REG_TESTS))

$(UNIT_TESTS): $(EXE) $(UNIT_TEST_BIN)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC) -c $(UNIT_TEST_SRC)/$@.cpp -o $(UNIT_TEST_BIN)/$@.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC) -Wl,-rpath -Wl,$(SOURCE_LIB) -lpthread $(UNIT_TEST_BIN)/$@.o $^ -lisl -L$(SOURCE_LIB) -o $(UNIT_TEST_BIN)/$@
	$(UNIT_TEST_BIN)/$@

$(REG_TESTS): $(EXE)
	python $(UTIL)/regression-util.py -r $(UTIL)/resources -p $(PROJECT_DIR) $(REG_TEST_DIR)/$@

#Building the Google Test framework
$(GTEST_SRCS_): $(INITED_FILE)
$(GTEST_HEADERS): $(INITED_FILE)

$(UNIT_TEST_BIN)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            ${GTEST_DIR}/src/gtest-all.cc -o $@

$(UNIT_TEST_BIN)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(UNIT_TEST_BIN)/gtest.a : $(UNIT_TEST_BIN)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(UNIT_TEST_BIN)/gtest_main.a : $(UNIT_TEST_BIN)/gtest-all.o $(UNIT_TEST_BIN)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Building documentation
doc: documentation
documentation: clean-doc
	doxygen

# Initialize the project and install third-party materials
init: initialize
initialize: $(INITED_FILE) $(UNIT_TEST_BIN)/gtest_main.a

$(INITED_FILE): $(THIRD_PARTY)
	mkdir $(THIRD_PARTY_INSTALL) $(THIRD_PARTY_BUILD)
	mkdir $(SOURCE_LIB)
	mkdir $(SOURCE_INC)
	#
	# Unzip and copy gtest
	unzip -q $(THIRD_PARTY_SRC)/gtest-1.7.0.zip -d $(THIRD_PARTY_INSTALL)/.
	mv $(THIRD_PARTY_INSTALL)/gtest-1.7.0 $(GTEST_DIR)
	cp -r $(GTEST_DIR)/include/gtest $(SOURCE_INC)
	#
	# build ISL
	tar -xzf $(THIRD_PARTY_SRC)/isl-0.15.tar.gz -C $(THIRD_PARTY_BUILD)/.
	mv $(THIRD_PARTY_BUILD)/isl-0.15 $(THIRD_PARTY_BUILD)/isl
		 cd $(THIRD_PARTY_BUILD)/isl \
	&& export CPPFLAGS=-ggdb \
	&& export CFLAGS=-ggdb \
	&& ./configure --prefix=$(THIRD_PARTY_INSTALL) \
	&& make -j$(MAKE_JOBS) \
	&& make install
	touch $(INITED_FILE)

# Cleaning
neat:
	- rm $(BIN)/*.o

clean-doc:
	- rm -r $(DOC_PATH)

clean-third-party:
	- rm -rf $(THIRD_PARTY_INSTALL) $(THIRD_PARTY_BUILD)
	- rm $(INITED_FILE)

clean-test:
	- rm -r $(UNIT_TEST_BIN)/* $(REG_TEST_DIR)/*.log $(REG_TEST_DIR)/*.dir

clean: clean-test
	- rm $(BIN)/*
	- rm $(LIB)/*

clean-all: clean-third-party clean clean-doc