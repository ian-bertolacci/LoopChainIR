PROJECT_DIR=PROJECT_DIR_PATH_STAMP
include $(PROJECT_DIR)/Makefile

TEST_BINARIES = code_generator \
								graph_generator \
								test

$(TEST_BINARIES): %: $(EXE) %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC) -c $@.cpp -o $@.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC) -Wl,-rpath -Wl,$(LIB) $@.o $(EXE) -L$(LIB) -lisl -o $@
