CXX=g++
CXXFLAGS += -Wall -Wextra -Werror -pthread
CPPFLAGS += --std=c++11 -isystem

all:
	$(CXX) -g $(CPPFLAGS) $(CXXFLAGS)  -I../src -Wl,-rpath -Wl,../third-party/install/lib -lpthread fusion_seg.cpp ../bin/LoopChainIR.a -lisl -L../third-party/install/lib -o fusion_seg
