# Paths
PROJECT_DIR=$(PWD)

BIN=$(PROJECT_DIR)/bin
SRC=$(PROJECT_DIR)/SRC

TEST=$(PROJECT_DIR)/test
TESTBIN=$(TEST)/bin
TESTSRC=$(TEST)/src

THIRD_PARTY=$(PROJECT_DIR)/third-party
THIRD_PARTY_SRC=$(THIRD_PARTY)/source
THIRD_PARTY_BUILD=$(THIRD_PARTY)/build
THIRD_PARTY_INSTALL=$(THIRD_PARTY)/install

LIB=$(THIRD_PARTY_INSTALL)/lib
INC=$(THIRD_PARTY_INSTALL)/include

# Global Variables
MAKE_JOBS=8

CXX=g++
CXXFLAGS += -g -Wall -Wextra -pthread
CPPFLAGS += -isystem $(INC)

# Test Variables
GTEST_DIR=$(THIRD_PARTY_INSTALL)/gtest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
TESTS =

all:

tests: $(TESTBIN)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TESTSRC)/sample1.cc -o $(TESTBIN)/sample1.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TESTSRC)/sample1_unittest.cc -o $(TESTBIN)/sample1_unittest.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(LIB) -lpthread $(TESTBIN)/sample1.o $(TESTBIN)/sample1_unittest.o $(TESTBIN)/gtest_main.a -o $(TESTBIN)/unit_test
	$(TESTBIN)/unit_test

$(TESTBIN)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(TESTBIN)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(TESTBIN)/gtest.a : $(TESTBIN)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(TESTBIN)/gtest_main.a : $(TESTBIN)/gtest-all.o $(TESTBIN)/gtest_main.o
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


clean-third-party:
	- rm -rf $(THIRD_PARTY_INSTALL) $(THIRD_PARTY_BUILD)

clean-test:
	- rm $(TESTBIN)/*

clean: clean-test
	- rm -r $(BIN)/*

nuke: clean-third-party clean
