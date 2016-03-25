PROJECT_DIR=..
SRC=$(PROJECT_DIR)/src
BIN=$(PROJECT_DIR)/bin

THIRD_PARTY=$(PROJECT_DIR)/third-party
THIRD_PARTY_SRC=$(THIRD_PARTY)/source
THIRD_PARTY_BUILD=$(THIRD_PARTY)/build
THIRD_PARTY_INSTALL=$(THIRD_PARTY)/install

INITED_FILE=$(THIRD_PARTY)/initialized

DOC_PATH = $(PROJECT_DIR)/doxygen

LIB=$(THIRD_PARTY_INSTALL)/lib
INC=$(THIRD_PARTY_INSTALL)/include

EX_SRC=./src
EX_BIN=./bin

EXAMPLES = RectangularDomainHowTo \
					 LoopNestHowTo \
					 LoopChainHowTo \
					 ScheduleHowTo \
					 FusionTransformationHowTo

LONG_EXAMPLES=$(addprefix $(EX_BIN)/,$(EXAMPLES))

LCIR_LIB=$(BIN)/LoopChainIR.a

all: $(LONG_EXAMPLES)

$(EXAMPLES):  % : $(EX_BIN)/%

$(LONG_EXAMPLES): $(EX_BIN)/% : $(EX_SRC)/%.cpp $(LCIR_LIB)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC) -Wl,-rpath -Wl,$(LIB) $^ -L$(LIB) -lisl -o $@

clean:
	- rm $(EX_BIN)/*
