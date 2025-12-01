#include <iostream>
#include <string>
#include <stack>
#include <limits>

// Removed 'using namespace std;' to avoid namespace pollution

// Iterative solution using a stack to verify balanced parentheses
// Logic: Push '(' onto stack, pop when ')' is encountered.
// If stack is empty when popping, or non-empty at the end -> Not Balanced.
bool verifyBalancedString(const std::string& char_stream) {
    std::stack<char> parensStack;

    for (char c : char_stream) {
        if (c == '(') {
            parensStack.push(c);
        } else if (c == ')') {
            if (parensStack.empty()) {
                return false; // Closing paren without matching opening paren
            }
            parensStack.pop();
        }
        // Ignore other characters or handle as invalid? 
        // The prompt implies strict "composed of '(' and ')'", 
        // but usually, parser ignores whitespace. Let's fail on invalid chars to be strict.
        else if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
             // Optional: return false on invalid chars if strict
        }
    }

    // Stack must be empty for it to be balanced
    return parensStack.empty();
}

int main() {
    std::cout << "\n--- CFG Parser (Balanced Parens) ---" << std::endl;
    std::cout << "This CFG parser checks if the input string has balanced parentheses." << std::endl;
    std::cout << "For epsilon (empty string), just press Enter" << std::endl;
    
    std::cout << "\nHow many test cases do you want to run? ";
    int test_cases;
    if (!(std::cin >> test_cases)) return 0;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    while (test_cases-- > 0){
        std::cout << "\nInput a string composed of '(' and ')' to check the balance of parentheses in said string." << std::endl;
        std::cout << "String: ";
        std::string test;
        std::getline(std::cin, test);
        std::cout << "Testing '" << test << "': " 
             << (verifyBalancedString(test) ? "Balanced" : "Not Balanced") << std::endl;
    }
    return 0;
}

