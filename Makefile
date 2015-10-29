# Paths
PROJECT_DIR=$(PWD)
BIN=$(PROJECT_DIR)/bin
SRC=$(PROJECT_DIR)/SRC
TEST=$(PROJECT_DIR)/test
THIRD_PARTY=$(PROJECT_DIR)/third-party
THIRD_PARTY_SRC=$(THIRD_PARTY)/source
THIRD_PARTY_BUILD=$(THIRD_PARTY)/build
THIRD_PARTY_INSTALL=$(THIRD_PARTY)/install
LIB=$(THIRD_PARTY_INSTALL)/lib
INC=$(THIRD_PARTY_INSTALL)/include

# Global Variables
MAKE_JOBS=8

CXX=g++

# Test varialbes
TESTS =



genesis: nuke
	mkdir $(THIRD_PARTY_INSTALL) $(THIRD_PARTY_BUILD)
	#
	# build ISL
	tar -xzf $(THIRD_PARTY_SRC)/isl-0.15.tar.gz -C $(THIRD_PARTY_BUILD)/.
	mv $(THIRD_PARTY_BUILD)/isl-0.15 $(THIRD_PARTY_BUILD)/isl
		 cd $(THIRD_PARTY_BUILD)/isl \
	&& ./configure --prefix=$(THIRD_PARTY_INSTALL) \
	&& make -j$(MAKE_JOBS) \
	&& make install
	#
	# Build gtest
	unzip -q $(THIRD_PARTY_SRC)/gtest-1.7.0.zip -d $(THIRD_PARTY_BUILD)/.
	mv $(THIRD_PARTY_BUILD)/gtest-1.7.0 $(THIRD_PARTY_BUILD)/gtest
		 cd $(THIRD_PARTY_BUILD)/gtest \
	&& ./configure \
	&& make -j$(MAKE_JOBS)

	cp -r $(THIRD_PARTY_BUILD)/gtest/lib/* $(LIB)/.
	cp -r $(THIRD_PARTY_BUILD)/gtest/include/gtest $(INC)/.

clean-third-party:
	- rm -rf $(THIRD_PARTY_INSTALL) $(THIRD_PARTY_BUILD)

clean-test:


clean: clean-test
	- rm -r $(BIN)/*

nuke: clean-third-party clean
