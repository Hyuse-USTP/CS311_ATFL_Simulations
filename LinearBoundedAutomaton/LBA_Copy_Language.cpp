#include <iostream>
#include <string>
#include <vector>
#include <limits>

// Enum class to represent the finite set of states for the LBA
enum class State {
    MARK_LEFT,    // Q0: Mark Left Side (A/B)
    FIND_END,     // Q1: Travel to end to mark Right Side
    MARK_RIGHT,   // Q2: Mark Right Side (1/2)
    RETURN_START, // Q3: Return to the first unmarked char on left
    RESET_HEAD,   // Q4: Rewind to start for Phase 2
    COMPARE_LEFT, // Q5: Find next Left char (A/B) to match
    MATCH_A,      // Q6: Find matching 1 for A
    MATCH_B,      // Q7: Find matching 2 for B
    RETURN_LEFT,  // Q8: Return to valid position after match
    ACCEPT,
    REJECT
};

class LBA {
public:
    LBA(std::string input) : tape(input), head(0), currentState(State::MARK_LEFT) {}

    bool run() {
        while (currentState != State::ACCEPT && currentState != State::REJECT) {
            
            // Boundary Check
            if (head < 0 || head > static_cast<int>(tape.length())) {
                currentState = State::REJECT;
                break;
            }

            // Read current char. If head == length, we treat as '\0' (boundary).
            char currentChar = (head < static_cast<int>(tape.length())) ? tape[head] : '\0';

            switch (currentState) {
                // --- PHASE 1: Find Midpoint & Mark Sides ---
                case State::MARK_LEFT:
                    if (currentChar == 'a') {
                        tape[head] = 'A';
                        head++;
                        currentState = State::FIND_END;
                    } else if (currentChar == 'b') {
                        tape[head] = 'B';
                        head++;
                        currentState = State::FIND_END;
                    } else if (currentChar == '1' || currentChar == '2') {
                        // Met the right markers. Phase 1 Done.
                        currentState = State::RESET_HEAD;
                    } else if (currentChar == '\0') {
                        // Empty string is valid ww (w=epsilon)
                        currentState = State::ACCEPT;
                    } else {
                         // Unexpected (e.g., already marked A/B here? Should be handled by RETURN_START logic putting us on unmarked)
                         currentState = State::REJECT;
                    }
                    break;

                case State::FIND_END:
                    if (currentChar == 'a' || currentChar == 'b' || currentChar == '1' || currentChar == '2') {
                        head++; // Move Right
                    } else if (currentChar == '\0') {
                        head--; // Hit end, step back
                        currentState = State::MARK_RIGHT;
                    } else {
                        currentState = State::REJECT;
                    }
                    break;

                case State::MARK_RIGHT:
                    if (currentChar == '1' || currentChar == '2') {
                        head--; // Skip existing right markers
                    } else if (currentChar == 'a') {
                        tape[head] = '1';
                        head--;
                        currentState = State::RETURN_START;
                    } else if (currentChar == 'b') {
                        tape[head] = '2';
                        head--;
                        currentState = State::RETURN_START;
                    } else if (currentChar == 'A' || currentChar == 'B') {
                        // Collision: Left markers met immediately. Odd length.
                        currentState = State::REJECT;
                    } else {
                        currentState = State::REJECT;
                    }
                    break;

                case State::RETURN_START:
                    if (currentChar == 'A' || currentChar == 'B') {
                        head++; // Found the boundary of Left markers, move to first unmarked
                        currentState = State::MARK_LEFT;
                    } else {
                        head--; // Keep moving left
                    }
                    break;

                // --- PHASE 2: Match Left & Right ---
                case State::RESET_HEAD:
                    if (head > 0) {
                        head--; // Rewind to start
                    } else {
                        currentState = State::COMPARE_LEFT;
                    }
                    break;

                case State::COMPARE_LEFT:
                    if (currentChar == '*') {
                        head++; // Skip already matched
                    } else if (currentChar == 'A') {
                        tape[head] = '*';
                        head++;
                        currentState = State::MATCH_A;
                    } else if (currentChar == 'B') {
                        tape[head] = '*';
                        head++;
                        currentState = State::MATCH_B;
                    } else if (currentChar == '\0') {
                        // Reached end successfully
                        currentState = State::ACCEPT;
                    } else if (currentChar == '1' || currentChar == '2') {
                         currentState = State::REJECT;
                    } else {
                        currentState = State::REJECT;
                    }
                    break;

                case State::MATCH_A:
                    if (currentChar == 'A' || currentChar == 'B' || currentChar == '*') {
                        head++; // Skip
                    } else if (currentChar == '1') {
                        tape[head] = '*';
                        head--;
                        currentState = State::RETURN_LEFT;
                    } else if (currentChar == '2' || currentChar == '\0') {
                        // Mismatch or End reached without finding match
                        currentState = State::REJECT;
                    } else {
                        head++;
                    }
                    break;

                case State::MATCH_B:
                    if (currentChar == 'A' || currentChar == 'B' || currentChar == '*') {
                        head++; // Skip
                    } else if (currentChar == '2') {
                        tape[head] = '*';
                        head--;
                        currentState = State::RETURN_LEFT;
                    } else if (currentChar == '1' || currentChar == '\0') {
                        currentState = State::REJECT;
                    } else {
                        head++;
                    }
                    break;

                case State::RETURN_LEFT:
                    if (currentChar == 'A' || currentChar == 'B') {
                        head--;
                    } else if (currentChar == '*') {
                        head++; // Found the split point
                        currentState = State::COMPARE_LEFT;
                    } else {
                        head--; 
                    }
                    break;

                default:
                    currentState = State::REJECT;
                    break;
            }
            
        }

        return currentState == State::ACCEPT;
    }
    
private:
    std::string tape;
    int head;
    State currentState;
};

int main() {
    std::cout << "============================================"
             << std::endl;
    std::cout << "   LBA SIMULATION: Copy Language L = {ww}"
             << std::endl;
    std::cout << "============================================"
             << std::endl;

    std::cout << "\nFORMAL LBA DEFINITION:"
             << std::endl;
    std::cout << "  States: { MARK_LEFT, FIND_END, MARK_RIGHT, RETURN_START, ... }"
             << std::endl;
    std::cout << "  Tape Alphabet: { a, b, A, B, 1, 2, *, \0 }"
             << std::endl;
    std::cout << "  Logic: 1. Mark boundaries (A/B ... 1/2). 2. Match A-1, B-2."
             << std::endl;

    std::cout << "\n--------------------------------------------"
             << std::endl;
    std::cout << "INSTRUCTIONS:"
             << std::endl;
    std::cout << "  - Enter strings to test."
             << std::endl;
    std::cout << "  - Valid:   abcabc, abab, aaaa"
             << std::endl;
    std::cout << "  - Invalid: abca, aaab, abba"
             << std::endl;
    std::cout << "--------------------------------------------"
             << std::endl;

    std::cout << "\nInput how many test cases you want to run: ";
    int test_cases;
    if (!(std::cin >> test_cases)) return 0;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Running " << test_cases << " test cases."
             << std::endl;
    
    while (test_cases-- > 0) {
        std::cout << "\nInput string: ";
        std::string test;
        std::getline(std::cin, test);
        std::cout << test << std::endl;

        LBA machine(test);
        
        std::cout << "Result: " 
             << (machine.run() ? "[ ACCEPTED ]" : "[ REJECTED ]") << std::endl;
    }
    return 0;
}
