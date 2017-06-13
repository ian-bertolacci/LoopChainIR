PROJECT_DIR=PROJECT_DIR_PATH_STAMP
include $(PROJECT_DIR)/Makefile

TEST_BINARIES = code_generator \
								raw_order_graph_generator \
								new_order_graph_generator \
								test_exe

$(TEST_BINARIES): %: %.cpp $(EXE)
	$(CXX) $(CXXFLAGS) $< -l$(LIBNAME) $(TEST_LDFLAGS) -L$(LIB) -o $@
