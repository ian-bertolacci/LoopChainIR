PROJECT_DIR=..
SRC=$(PROJECT_DIR)/src
BIN=$(PROJECT_DIR)/bin
LIB=$(PROJECT_DIR)/lib
INCLUDE=$(PROJECT_DIR)/include

THIRD_PARTY=$(PROJECT_DIR)/third-party
THIRD_PARTY_SRC=$(THIRD_PARTY)/source
THIRD_PARTY_BUILD=$(THIRD_PARTY)/build
THIRD_PARTY_INSTALL=$(THIRD_PARTY)/install

INITED_FILE=$(THIRD_PARTY)/initialized

DOC_PATH = $(PROJECT_DIR)/doxygen

T_LIB=$(THIRD_PARTY_INSTALL)/lib
T_INC=$(THIRD_PARTY_INSTALL)/include

EX_SRC=./src
EX_BIN=./bin

EXAMPLES = RectangularDomainHowTo \
					 LoopNestHowTo \
					 LoopChainHowTo \
					 ScheduleHowTo \
					 FusionTransformationHowTo \
					 ShiftTransformationHowTo \
					 TileTransformationHowTo \
					 MultipleTransformations \
					 Example

LONG_EXAMPLES=$(addprefix $(EX_BIN)/,$(EXAMPLES))

LCIR_LIB=$(LIB)/libloopchainIR.a

all: $(LONG_EXAMPLES)

$(EXAMPLES):  % : $(EX_BIN)/%

$(LONG_EXAMPLES): $(EX_BIN)/% : $(EX_SRC)/%.cpp $(LCIR_LIB)
	$(CXX) --std=c++11 -isystem $(T_INC) -I$(INCLUDE) -Wl,-rpath -Wl,$(T_LIB) $^ -L$(T_LIB) -lisl -o $@

clean:
	- rm $(EX_BIN)/*
