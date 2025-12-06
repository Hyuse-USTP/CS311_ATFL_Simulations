/**
 * GNF_Example.cpp
 *
 * Simulation of the Greibach Normal Form (GNF) Construction Algorithm.
 * Implements the standard 4-step conversion using C++ containers optimized for
 * symbol manipulation.
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <sstream>
#include <iterator>

// ==========================================
// Core Data Structures
// ==========================================

enum SymbolType {
    TERMINAL,
    VARIABLE
};

// Represents Terminals and Variables.
struct Symbol {
    std::string name;
    SymbolType type;
    int index; // Used for the A_i ordering constraint.

    // Comparison for std::map and std::set keys
    bool operator<(const Symbol& other) const {
        if (type != other.type) return type < other.type;
        if (name != other.name) return name < other.name;
        return index < other.index;
    }

    bool operator==(const Symbol& other) const {
        return name == other.name && type == other.type && index == other.index;
    }
    
    bool operator!=(const Symbol& other) const {
        return !(*this == other);
    }

    std::string toString() const {
        return name;
    }
};

// Use std::list for production bodies to enable O(1) splicing during substitution.
using ProductionBody = std::list<Symbol>;

// Use std::set to automatically filter duplicate rules and keep them sorted.
using Productions = std::set<ProductionBody>;

// Map Variables to their set of productions.
using Grammar = std::map<Symbol, Productions>;

// ==========================================
// Helper Functions
// ==========================================

void printGrammar(const Grammar& G, const std::string& stageName) {
    std::cout << "--- " << stageName << " ---" << std::endl;
    for (const auto& pair : G) {
        const Symbol& head = pair.first;
        const Productions& bodies = pair.second;
        
        if (bodies.empty()) continue;

        std::cout << head.toString() << " -> ";
        bool firstBody = true;
        for (const auto& body : bodies) {
            if (!firstBody) std::cout << " | ";
            
            bool firstSym = true;
            for (const auto& sym : body) {
                // Add space between symbols for readability (e.g. "A2 A3" vs "A2A3")
                // Terminals might not need space if we want compact "a", but "a A1" looks better.
                if (!firstSym) std::cout << " "; 
                std::cout << sym.toString();
                firstSym = false;
            }
            firstBody = false;
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------" << std::endl << std::endl;
}

// ==========================================
// GNF Construction Algorithm
// ==========================================

class GNFConverter {
private:
    Grammar grammar;
    std::vector<Symbol> orderedVariables;
    int zCounter = 1;

    // Checks for immediate left recursion: A -> A alpha
    bool isLeftRecursive(const Symbol& head, const ProductionBody& body) {
        return !body.empty() && body.front() == head;
    }

public:
    GNFConverter(const Grammar& initialGrammar) : grammar(initialGrammar) {}

    // Step 2: Renaming/Ordering
    // Collects variables and sorts them (Simulating A_1...A_m assignment)
    void step2_Ordering() {
        orderedVariables.clear();
        for (auto& pair : grammar) {
            if (pair.first.type == VARIABLE) {
                orderedVariables.push_back(pair.first);
            }
        }
        // Sort based on index
        std::sort(orderedVariables.begin(), orderedVariables.end(), [](const Symbol& a, const Symbol& b){
            return a.index < b.index;
        });

        printGrammar(grammar, "Step 2: Variables Ordered");
    }

    // Step 3: Lemma 1 & Forward Substitution
    // Transform rules so that if A_i -> A_j alpha, then j > i.
    void step3_ForwardSubstitution() {
        for (size_t i = 0; i < orderedVariables.size(); ++i) {
            Symbol Ai = orderedVariables[i];
            
            for (size_t j = 0; j < i; ++j) {
                Symbol Aj = orderedVariables[j];

                Productions newRules;
                Productions& aiRules = grammar[Ai];
                
                auto it = aiRules.begin();
                while (it != aiRules.end()) {
                    ProductionBody currentBody = *it;
                    
                    // Detect A_i -> A_j alpha
                    if (!currentBody.empty() && currentBody.front() == Aj) {
                        std::list<Symbol> alpha = currentBody; 
                        alpha.pop_front();

                        // Substitute A_j with its bodies
                        if (grammar.find(Aj) != grammar.end()) {
                            const Productions& ajRules = grammar.at(Aj);
                            for (const auto& beta : ajRules) {
                                ProductionBody newBody = beta;
                                std::list<Symbol> alphaCopy = alpha; 
                                newBody.splice(newBody.end(), alphaCopy);
                                newRules.insert(newBody);
                            }
                        }
                        it = aiRules.erase(it);
                    } else {
                        ++it;
                    }
                }
                aiRules.insert(newRules.begin(), newRules.end());
            }
            eliminateLeftRecursion(Ai);
        }
        printGrammar(grammar, "Step 3: Forward Substitution & Recursion Elimination");
    }

    // Step 4: Eliminate Immediate Left Recursion
    // Transforms A -> A alpha | beta  into  A -> beta Z?, Z -> alpha Z?
    void eliminateLeftRecursion(Symbol A) {
        Productions& rules = grammar[A];
        Productions alphaRules; 
        Productions betaRules;

        bool hasRecursive = false;
        for (const auto& body : rules) {
            if (isLeftRecursive(A, body)) {
                hasRecursive = true;
                ProductionBody alpha = body;
                alpha.pop_front();
                alphaRules.insert(alpha);
            } else {
                betaRules.insert(body);
            }
        }

        if (!hasRecursive) return;

        // Create new Variable Z
        Symbol Z;
        Z.name = "Z_" + A.name;
        Z.type = VARIABLE;
        Z.index = 1000 + zCounter++; 
        
        rules.clear();

        // A -> beta | beta Z
        for (const auto& beta : betaRules) {
            rules.insert(beta); 
            
            ProductionBody betaZ = beta;
            betaZ.push_back(Z);
            rules.insert(betaZ); 
        }

        // Z -> alpha | alpha Z
        Productions zRules;
        for (const auto& alpha : alphaRules) {
            zRules.insert(alpha); 
            
            ProductionBody alphaZ = alpha;
            alphaZ.push_back(Z);
            zRules.insert(alphaZ);
        }

        grammar[Z] = zRules;
    }

    // Step 5: Back Substitution
    // Ensure all rules start with a Terminal by propagating backwards from A_m to A_1.
    void step5_BackSubstitution() {
        // Process original variables backwards
        for (int i = orderedVariables.size() - 1; i >= 0; --i) {
            Symbol Ai = orderedVariables[i];
            substituteUntilTerminal(Ai);
        }

        // Process Z variables (they might still point to Variables)
        // We collect them first to avoid iterator invalidation or logic issues
        std::vector<Symbol> zVars;
        for (auto& pair : grammar) {
            bool isOriginal = false;
            for(auto &v : orderedVariables) if(v == pair.first) isOriginal = true;
            if (!isOriginal) zVars.push_back(pair.first);
        }
        
        for (const auto& Z : zVars) {
            substituteUntilTerminal(Z);
        }

        printGrammar(grammar, "Step 5: Back Substitution (Final GNF)");
    }

    // Helper to substitute the head of productions until they start with a Terminal
    void substituteUntilTerminal(Symbol target) {
        Productions& rules = grammar[target];
        Productions newRules;
        
        auto it = rules.begin();
        while (it != rules.end()) {
            if (!it->empty() && it->front().type == VARIABLE) {
                Symbol first = it->front();
                ProductionBody suffix = *it; 
                suffix.pop_front();
                
                if (grammar.count(first)) {
                    for(const auto& repl : grammar.at(first)) {
                        ProductionBody combined = repl;
                        // We must copy suffix because we splice it multiple times
                        std::list<Symbol> suffixCopy = suffix;
                        combined.splice(combined.end(), suffixCopy);
                        newRules.insert(combined);
                    }
                }
                it = rules.erase(it);
            } else {
                ++it;
            }
        }
        rules.insert(newRules.begin(), newRules.end());
    }

    void run() {
        step2_Ordering();
        step3_ForwardSubstitution();
        step5_BackSubstitution();
    }
};

// ==========================================
// Main Execution
// ==========================================

int main() {
    std::cout << "Greibach Normal Form (GNF) Algorithm Simulation" << std::endl;
    std::cout << "===============================================" << std::endl;

    // Test Grammar:
    // A1 -> A2 A3
    // A2 -> A3 A1 | b
    // A3 -> A1 A2 | a
    
    Symbol A1 = {"A1", VARIABLE, 1};
    Symbol A2 = {"A2", VARIABLE, 2};
    Symbol A3 = {"A3", VARIABLE, 3};
    
    Symbol a = {"a", TERMINAL, 0};
    Symbol b = {"b", TERMINAL, 0};

    Grammar G;

    G[A1].insert({A2, A3});
    
    G[A2].insert({A3, A1});
    G[A2].insert({b});

    G[A3].insert({A1, A2});
    G[A3].insert({a});

    printGrammar(G, "Original Grammar Rules");

    GNFConverter converter(G);
    converter.run();

    return 0;
}