CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Define targets (executables)
TARGETS = CFG_AND_PDA_Equivalence/CFG-PDA_Equivalence_Example \
          ChomskyNormalForm/CNF_Example \
          Context-Free\ Grammar/CFG_Example \
          GreibachNormalForm/GNF_Example \
          LinearBoundedAutomaton/LBA_Example \
          LinearBoundedAutomaton/LBA_Copy_Language

# Default target: build all
all: $(TARGETS)

# Rule to build CFG-PDA Equivalence
CFG_AND_PDA_Equivalence/CFG-PDA_Equivalence_Example: CFG_AND_PDA_Equivalence/CFG-PDA_Equivalence_Example.cpp
	$(CXX) $(CXXFLAGS) -o "$@" "$<"

# Rule to build CNF Example
ChomskyNormalForm/CNF_Example: ChomskyNormalForm/CNF_Example.cpp
	$(CXX) $(CXXFLAGS) -o "$@" "$<"

# Rule to build CFG Example (handles space in path)
Context-Free\ Grammar/CFG_Example: Context-Free\ Grammar/CFG_Example.cpp
	$(CXX) $(CXXFLAGS) -o "$@" "$<"

# Rule to build GNF Example
GreibachNormalForm/GNF_Example: GreibachNormalForm/GNF_Example.cpp
	$(CXX) $(CXXFLAGS) -o "$@" "$<"

# Rule to build LBA Example
LinearBoundedAutomaton/LBA_Example: LinearBoundedAutomaton/LBA_Example.cpp
	$(CXX) $(CXXFLAGS) -o "$@" "$<"

# Rule to build LBA Copy Language Example
LinearBoundedAutomaton/LBA_Copy_Language: LinearBoundedAutomaton/LBA_Copy_Language.cpp
	$(CXX) $(CXXFLAGS) -o "$@" "$<"

# Clean up binaries
clean:
	rm -f CFG_AND_PDA_Equivalence/CFG-PDA_Equivalence_Example
	rm -f ChomskyNormalForm/CNF_Example
	rm -f Context-Free\ Grammar/CFG_Example
	rm -f GreibachNormalForm/GNF_Example
	rm -f LinearBoundedAutomaton/LBA_Example
	rm -f LinearBoundedAutomaton/LBA_Copy_Language

# PHONY targets for convenience
.PHONY: all clean run_pda run_cnf run_cfg run_gnf run_lba run_lba_copy

# Helper to run the PDA simulation
run_pda: CFG_AND_PDA_Equivalence/CFG-PDA_Equivalence_Example
	./CFG_AND_PDA_Equivalence/CFG-PDA_Equivalence_Example

# Helper to run the CNF simulation
run_cnf: ChomskyNormalForm/CNF_Example
	./ChomskyNormalForm/CNF_Example

# Helper to run the CFG simulation
run_cfg: Context-Free\ Grammar/CFG_Example
	./Context-Free\ Grammar/CFG_Example

# Helper to run the GNF simulation
run_gnf: GreibachNormalForm/GNF_Example
	./GreibachNormalForm/GNF_Example

# Helper to run the LBA simulation
run_lba: LinearBoundedAutomaton/LBA_Example
	./LinearBoundedAutomaton/LBA_Example

# Helper to run the LBA Copy Language simulation
run_lba_copy: LinearBoundedAutomaton/LBA_Copy_Language
	./LinearBoundedAutomaton/LBA_Copy_Language