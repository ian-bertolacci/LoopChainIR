PROJECT_DIR=PROJECT_DIR_PATH_STAMP
include $(PROJECT_DIR)/Makefile

TEST_BINARIES = code_generator \
								raw_order_graph_generator \
								new_order_graph_generator \
								test_exe

$(TEST_BINARIES): %: %.cpp $(EXE)
	$(CXX) $(CPPFLAGS) $(INCFLAGS) $< -l$(LIBNAME) $(LDFLAGS) -L$(LIB) -o $@
