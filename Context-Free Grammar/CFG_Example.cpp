#include <iostream>
#include <string>
#include <limits>
#include <cctype>

// --- Context-Free Grammar (CFG) Definition ---
// This parser implements a grammar that supports Tuples AND Quoted Strings.
//
// Formal Definition G = (V, Sigma, R, S)
//
// 1. Variables (V): { S, E, L, Q }
//    - S: Sequence (Start Symbol)
//    - E: Element (Alphanumeric, Tuple, or String)
//    - L: List (Comma-separated elements)
//    - Q: Quoted String
//
// 2. Terminals (Sigma):
//    - Alphanumeric chars (a-z, 0-9)
//    - Brackets: (, ), [, ], {, }
//    - Separator: ,
//    - Quote: "
//
// 3. Production Rules (R):
//    S -> E S | epsilon              (Sequence)
//    E -> char | Q | ( L_opt )       (Element can be a char, string, or tuple)
//    Q -> " Content "                (Quoted String Wrapper)
//    Content -> AnyChar Content | epsilon  (Where AnyChar != ")
//    L -> E , L | E                  (List with Separator)
//
// 4. Start Symbol: S

// Forward declarations
bool parseS(const std::string& str, size_t& idx);
bool parseE(const std::string& str, size_t& idx);
bool parseL(const std::string& str, size_t& idx);
bool parseQ(const std::string& str, size_t& idx);

// --- Helper for Whitespace ---
void skipWhitespace(const std::string& str, size_t& idx) {
    while (idx < str.length() && (str[idx] == ' ' || str[idx] == '\t' || str[idx] == '\r')) {
        idx++;
    }
}

// --- Rule: S -> E S | epsilon ---
// Represents a sequence of elements side-by-side, e.g., "(a) [b] c"
bool parseS(const std::string& str, size_t& idx) {
    skipWhitespace(str, idx);

    // Base Case: End of string or unexpected closing bracket/comma
    if (idx >= str.length()) return true;
    
    char current = str[idx];
    if (current == ')' || current == ']' || current == '}' || current == ',') {
        return true; 
    }

    // Recursive Step: S -> E S
    if (!parseE(str, idx)) return false;
    return parseS(str, idx);
}

// --- Rule: Q -> " Content " ---
// Parses a quoted string literal.
bool parseQ(const std::string& str, size_t& idx) {
    if (idx >= str.length() || str[idx] != '"') return false;
    idx++; // Consume opening quote

    // Rule: Content -> AnyChar Content | epsilon
    // We consume characters until we hit a closing quote or EOF.
    while (idx < str.length() && str[idx] != '"') {
        // (Optional: Add logic here for escape sequences like \" if desired)
        idx++; 
    }

    // Must end with a closing quote
    if (idx >= str.length()) return false; // Error: Unclosed string
    
    idx++; // Consume closing quote
    return true;
}

// --- Rule: E -> char | Q | ( L_opt ) ... ---
// Represents a single unit. 
bool parseE(const std::string& str, size_t& idx) {
    skipWhitespace(str, idx);
    if (idx >= str.length()) return false; 

    char current = str[idx];

    // Option 1: Quoted String
    if (current == '"') {
        return parseQ(str, idx);
    }
    // Option 2: Alphanumeric Character
    else if (std::isalnum(current)) {
        idx++; 
        return true;
    } 
    // Option 3: Tuples/Brackets
    else if (current == '(' || current == '[' || current == '{') {
        char open = current;
        char close = (open == '(') ? ')' : (open == '[') ? ']' : '}';
        
        idx++; // Consume Open
        
        // Check for empty tuple "()"
        skipWhitespace(str, idx);
        if (idx < str.length() && str[idx] != close) {
            // L_opt -> L
            if (!parseL(str, idx)) return false; 
        }

        skipWhitespace(str, idx);
        if (idx >= str.length() || str[idx] != close) return false;
        idx++; // Consume Close
        return true;
    }

    return false; // Invalid element start
}

// --- Rule: L -> E , L | E ---
// Represents the "List with Separator"
bool parseL(const std::string& str, size_t& idx) {
    // 1. Parse the first Element (E)
    if (!parseE(str, idx)) return false;

    skipWhitespace(str, idx);

    // 2. Check for Separator
    if (idx < str.length() && str[idx] == ',') {
        idx++; // Consume comma
        // Recursion: Must find another List/Element
        return parseL(str, idx); 
    }

    return true; // No comma, end of list
}

// --- Main Verification Function ---
bool verifyTupleGrammar(const std::string& input) {
    size_t index = 0;
    bool valid = parseS(input, index);
    return valid && index == input.length();
}

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "   CFG PARSER: Tuples, Lists & Strings" << std::endl;
    std::cout << "============================================" << std::endl;
    
    std::cout << "\nFORMAL GRAMMAR DEFINITION G = (V, Sigma, R, S):" << std::endl;
    std::cout << "  V (Variables): { S, E, L, Q }" << std::endl;
    std::cout << "  Sigma (Terminals): { a-z, 0-9, \" , (, ), [, ], {, }, , }" << std::endl;
    std::cout << "  R (Production Rules):" << std::endl;
    std::cout << "    1. S -> E S | epsilon" << std::endl;
    std::cout << "    2. E -> char | Q | ( L )" << std::endl;
    std::cout << "    3. Q -> \" Content \"" << std::endl;
    std::cout << "    4. L -> E , L | E" << std::endl;
    
    std::cout << "\n--------------------------------------------" << std::endl;
    std::cout << "INSTRUCTIONS:" << std::endl;
    std::cout << "  - Enter strings to test." << std::endl;
    std::cout << "  - Valid:   (\"hello\", b, [c])" << std::endl;
    std::cout << "  - Invalid: (\"hello, b)   (Missing quote)" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    std::cout << "\nHow many test cases? ";
    int test_cases;
    if (!(std::cin >> test_cases)) return 0;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (test_cases-- > 0) {
        std::cout << "\nInput: ";
        std::string input;
        std::getline(std::cin, input);
        
        // Echo the input so it is visible during file redirection
        std::cout << input << std::endl;
        
        bool result = verifyTupleGrammar(input);
        std::cout << "Result: " << (result ? "[ ACCEPTED ]" : "[ REJECTED ]") << std::endl;
    }

    return 0;
}