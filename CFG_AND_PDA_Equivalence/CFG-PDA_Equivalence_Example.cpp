#include <iostream>
#include <string>
#include <stack>
#include <limits>

// Removed 'using namespace std;' to avoid namespace pollution

bool simulatePDA(const std::string& input) {
    std::stack<char> pdaStack;
    enum State { q_start, q_read_b };
    State currentState = q_start;

    for (char c : input) {
        switch (currentState) {
            case q_start:
                if (c == 'a') {
                    pdaStack.push('$');
                } else if (c == 'b') {
                    currentState = q_read_b;
                    if (pdaStack.empty()) return false;
                    pdaStack.pop();
                } else {
                    return false; // Invalid character
                }
                break;
            case q_read_b:
                if (c == 'b') {
                    if (pdaStack.empty()) return false;
                    pdaStack.pop();
                } else {
                    return false; // 'a' after 'b'
                }
                break;
        }
    }
    // Final check: only accept if the pdaStack is empty
    return pdaStack.empty();
}

bool parseS(const std::string& input, size_t& index) {
    // Check for the S -> aSb rule
    if (index < input.length() && input[index] == 'a') {
        // 1. Consume the 'a'
        index++;

        // 2. Try to parse the middle 'S' recursively
        if (!parseS(input, index)) {
            return false; // Middle 'S' parse failed
        }

        // 3. Check for and consume the 'b'
        if (index < input.length() && input[index] == 'b') {
            index++;
            return true; // Successfully parsed 'aSb'
        } else {
            return false; // 'a' was not followed by 'b'
        }
    }
    
    // If it's not 'a', it must be the S -> ε (epsilon) rule.
    return true;
}

bool parseCFG(const std::string& input) {
    size_t currentIndex = 0;
    bool success = parseS(input, currentIndex);

    return success && (currentIndex == input.length());
}


int main() {
    std::string userInput;
    int test_cases;
    std::cout << "--- CFG-PDA Equivalence Demo for L = {a^n b^n} ---" << std::endl;
    std::cout << "Input the number of test cases: " << std::endl;
    
    if (!(std::cin >> test_cases)) {
        return 0;
    }
    // Robustly clear the newline from the buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (test_cases-- > 0) {
        std::cout << "\nEnter a string (e.g., aabb, ab, aab, abb, ba): ";
        std::getline(std::cin, userInput);
        
        std::cout << "\nAnalyzing string: \"" << userInput << "\"" << std::endl;
        bool pda_result = simulatePDA(userInput);
        bool cfg_result = parseCFG(userInput);

        std::cout << "1. PDA Simulator Result: " 
                  << (pda_result ? "ACCEPT" : "REJECT") << std::endl;
                
        std::cout << "2. CFG Parser Result:    " 
                  << (cfg_result ? "ACCEPT" : "REJECT") << std::endl;

        std::cout << "\n----------------------------------------" << std::endl;
        if (pda_result == cfg_result) {
            std::cout << "✅ Success! Both methods agree." << std::endl;
            std::cout << "This demonstrates that for this string, the PDA and the CFG are behaving equivalently." << std::endl;
        } else {
            std::cout << "❌ Error! The methods disagree. There is a bug in the code." << std::endl;
        }
    }
    return 0;
}