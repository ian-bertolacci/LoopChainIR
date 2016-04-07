PROJECT_DIR=PROJECT_DIR_PATH_STAMP
include $(PROJECT_DIR)/Makefile

TEST_BINARIES = code_generator \
								raw_order_graph_generator \
								new_order_graph_generator \
								test

$(TEST_BINARIES): %: $(EXE) %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC) -c $@.cpp -o $@.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC) -Wl,-rpath -Wl,$(SOURCE_LIB) $@.o $(EXE) -L$(SOURCE_LIB) -L$(LIB) -lisl -o $@
