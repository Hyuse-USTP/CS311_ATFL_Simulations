#include <iostream>
#include <string>
#include <vector>
#include <cctype>

// Defines a single production rule for a grammar
struct Rule {
    std::string head; // Left-hand side variable (e.g., "S")
    std::string production; // Right-hand side body (e.g., "aB")
};

// Checks if a char is an uppercase letter (a Non-Terminal/Variable)
bool is_variable_char(char ch) {
    return std::isupper(static_cast<unsigned char>(ch));
}

// Checks if a char is a lowercase letter (a Terminal)
bool is_terminal_char(char ch) {
    return std::islower(static_cast<unsigned char>(ch));
}

bool validateGNF(const std::vector<Rule>& grammar_rules) {
    for (const auto& prod_rule : grammar_rules) {
        // LHS must be a single non-terminal
        if (prod_rule.head.length() != 1 || !is_variable_char(prod_rule.head[0])) {
            return false;
        }

        // GNF rules are of the form A -> a V* 
        // (Start with a terminal, followed by zero or more variables)
        
        if (prod_rule.production.empty()) {
            // GNF strictly doesn't allow epsilon productions, except possibly for Start symbol.
            // For simplicity here, empty is invalid.
            return false; 
        }

        // 1. First char must be a terminal
        if (!is_terminal_char(prod_rule.production[0])) {
            return false;
        }

        // 2. Subsequent chars must be variables
        for (size_t i = 1; i < prod_rule.production.length(); ++i) {
            if (!is_variable_char(prod_rule.production[i])) {
                return false;
            }
        }
    }
    return true; // The grammar is in GNF
}

void printGrammar(const std::vector<Rule>& grammar, int grammar_number) {
    std::cout << "\nGrammar " << grammar_number << ":" << std::endl;
    for (const auto& rule : grammar) {
        std::cout << rule.head << " -> " << rule.production << std::endl;
    }
}

int main() {
    std::cout << "\n--- Greibach Normal Form (GNF) Validator ---" << std::endl;
    
    std::vector<Rule> first_test_case = {
        {"X", "aY"},
        {"Y", "bX"},
        {"X", "a"},
        {"Y", "b"}
    };
    
     std::vector<Rule> second_test_case = {
        {"X", "YA"}, // Invalid: starts with variable
        {"Y", "b"}
    };
    
     std::vector<Rule> third_test_case = {
        {"X", "ab"} // Invalid: ends with terminal (second char 'b')
    };
    
    // Print and validate each grammar
    printGrammar(first_test_case, 1);
    std::cout << "Grammar 1 check: " << (validateGNF(first_test_case) ? "Pass (Is GNF)" : "Fail (Not GNF)") << std::endl;
    
    printGrammar(second_test_case, 2);
    std::cout << "Grammar 2 check: " << (validateGNF(second_test_case) ? "Pass (Is GNF)" : "Fail (Not GNF)") << std::endl;
    
    printGrammar(third_test_case, 3);
    std::cout << "Grammar 3 check: " << (validateGNF(third_test_case) ? "Pass (Is GNF)" : "Fail (Not GNF)") << std::endl;

    return 0;
}

