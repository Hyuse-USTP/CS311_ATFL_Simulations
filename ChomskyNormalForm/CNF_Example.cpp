#include <iostream>
#include <string>
#include <vector>
#include <cctype> // for std::isupper, std::islower

struct Rule {
    std::string head;
    std::string production;
};

bool is_variable_char(char c) {
    return std::isupper(static_cast<unsigned char>(c));
}

bool is_terminal_char(char c) {
    return std::islower(static_cast<unsigned char>(c));
}

bool validateCNF(const std::vector<Rule>& grammar_rules) {
    for (const auto& prod_rule : grammar_rules) {
        // 1. Check Head: Must be single variable (A-Z)
        if (prod_rule.head.length() != 1 || !is_variable_char(prod_rule.head[0])) {
            return false;
        }

        // 2. Check Production Body
        size_t len = prod_rule.production.length();

        if (len == 1) {
            // Rule: A -> a (Terminal)
            if (!is_terminal_char(prod_rule.production[0])) {
                return false;
            }
        } else if (len == 2) {
            // Rule: A -> BC (Two Variables)
            if (!is_variable_char(prod_rule.production[0]) || 
                !is_variable_char(prod_rule.production[1])) {
                return false;
            }
        } else {
            // Special case: S -> e (epsilon) is often allowed in definitions if S is start symbol
            // But strictly, if length is not 1 or 2 (and not epsilon special case), it fails.
            // The original code had a check for "e".
            if (prod_rule.head == "S" && prod_rule.production == "e") {
                continue;
            }
            return false;
        }
    }
    return true;
}

void printGrammar(const std::vector<Rule>& grammar, int grammar_number) {
    std::cout << "\nGrammar " << grammar_number << ":" << std::endl;
    for (const auto& rule : grammar) {
        std::cout << rule.head << " -> " << rule.production << std::endl;
    }
}

int main() {
    std::cout << "\n--- Chomsky Normal Form (CNF) Validator ---" << std::endl;
    
    std::vector<Rule> first_grammar_set = {
        {"X", "YZ"},
        {"Y", "y"},
        {"Z", "z"}
    };
    
    std::vector<Rule> second_grammar_set = {
        {"X", "YZ"},
        {"Y", "y"},
        {"Z", "abc"}
    };

    std::vector<Rule> third_grammar_set = {
        {"A", "Bx"},
        {"B", "b"}
    };
    
    printGrammar(first_grammar_set, 1);
    std::cout << "Grammar 1 check: " << (validateCNF(first_grammar_set) ? "Pass (Is CNF)" : "Fail (Not CNF)") << std::endl;
    
    printGrammar(second_grammar_set, 2);
    std::cout << "Grammar 2 check: " << (validateCNF(second_grammar_set) ? "Pass (Is CNF)" : "Fail (Not CNF)") << std::endl;
    
    printGrammar(third_grammar_set, 3);
    std::cout << "Grammar 3 check: " << (validateCNF(third_grammar_set) ? "Pass (Is CNF)" : "Fail (Not CNF)") << std::endl;

    return 0;
}

