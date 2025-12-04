#include <iostream>
#include <string>
#include <vector>
#include <limits>

// Enum class to represent the finite set of states for the LBA
enum class State {
    START,       // Q0
    FIND_B,      // Q1
    FIND_C,      // Q2
    REWIND,      // Q3
    CHECK_FINAL, // Q4
    ACCEPT,
    REJECT
};

// Simulates a Linear Bounded Automaton (LBA) for L = {a^n b^n c^n : n >= 1}
// This simulation follows the formal definition:
// - Finite set of states
// - Transition function δ(q, a) = (q', b, D)
// - Head movement limited to +1 (Right) or -1 (Left) per step
class LBA {
public:
    LBA(std::string input) : tape(input), head(0), currentState(State::START) {}

    bool run() {
        // Main Loop: continues until the machine enters an ACCEPT or REJECT state.
        while (currentState != State::ACCEPT && currentState != State::REJECT) {
            
            // Boundary Check: The machine cannot move the head beyond the tape boundaries.
            // In this simulation, we treat 'head == tape.length()' as the position just after the last character.
            if (head < 0 || head > static_cast<int>(tape.length())) {
                currentState = State::REJECT;
                break;
            }

            // Read the character under the head.
            // If head is at tape.length(), we treat it as a special 'End of String' marker (often denoted as blank or null).
            char currentChar = (head < static_cast<int>(tape.length())) ? tape[head] : '\0';

            switch (currentState) {
                case State::START: // Q0: Start state, looks for 'a' to mark or 'Y' if finished with 'a's
                    if (currentChar == 'a') {
                        tape[head] = 'X';           // Write 'X'
                        head++;                     // Move Right
                        currentState = State::FIND_B; 
                    } else if (currentChar == 'Y') {
                        // All 'a's have been processed (replaced by 'X').
                        // Now we check if the rest of the string is valid.
                        head++;                     // Move Right
                        currentState = State::CHECK_FINAL; 
                    } else {
                        // Unexpected character (e.g., start with 'b', 'c', or 'Z')
                        currentState = State::REJECT;
                    }
                    break;

                case State::FIND_B: // Q1: Scan right to find the matching 'b'
                    if (currentChar == 'a') {
                        head++;                     // Move Right
                        // Stay in FIND_B
                    } else if (currentChar == 'Y') {
                        head++;                     // Move Right (Skip already marked 'b's)
                        // Stay in FIND_B
                    } else if (currentChar == 'b') {
                        tape[head] = 'Y';           // Write 'Y'
                        head++;                     // Move Right
                        currentState = State::FIND_C; 
                    } else {
                        // Found 'c', 'Z' too early, or End of String
                        currentState = State::REJECT;
                    }
                    break;

                case State::FIND_C: // Q2: Scan right to find the matching 'c'
                    if (currentChar == 'b') {
                        head++;                     // Move Right
                        // Stay in FIND_C
                    } else if (currentChar == 'Z') {
                        head++;                     // Move Right (Skip already marked 'c's)
                        // Stay in FIND_C
                    } else if (currentChar == 'c') {
                        tape[head] = 'Z';           // Write 'Z'
                        head--;                     // Move Left !! Important: Direction change
                        currentState = State::REWIND; 
                    } else {
                        // Found End of String or unexpected char
                        currentState = State::REJECT;
                    }
                    break;

                case State::REWIND: // Q3: Move left until we find 'X'
                    if (currentChar == 'X') {
                        head++;                     // Move Right (back to the first unmarked character)
                        currentState = State::START; 
                    } else {
                        head--;                     // Move Left
                        // Stay in REWIND
                    }
                    break;

                case State::CHECK_FINAL: // Q4: Verify only 'Y' and 'Z' remain
                    if (currentChar == '\0') {      // End of String reached
                        currentState = State::ACCEPT;
                    } else if (currentChar == 'Y' || currentChar == 'Z') {
                        head++;                     // Move Right
                        // Stay in CHECK_FINAL
                    } else {
                        // Found unmarked 'a', 'b', or 'c' -> Logic error or malformed input
                        currentState = State::REJECT;
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
    std::cout << "============================================" << std::endl;
    std::cout << "   LBA SIMULATION: a^n b^n c^n (n >= 1)" << std::endl;
    std::cout << "============================================" << std::endl;

    std::cout << "\nFORMAL LBA DEFINITION M = (Q, Sigma, Gamma, delta, q0, q_accept, q_reject):" << std::endl;
    std::cout << "  Q (States): { START (q0), FIND_B (q1), FIND_C (q2), REWIND (q3), CHECK_FINAL (q4), ACCEPT, REJECT }" << std::endl;
    std::cout << "  Sigma (Input Alphabet): { a, b, c }" << std::endl;
    std::cout << "  Gamma (Tape Alphabet): { a, b, c, X, Y, Z, \\0 }" << std::endl;
    std::cout << "  q0 (Start State): START" << std::endl;
    std::cout << "  delta (Transition Function Rules):" << std::endl;
    std::cout << "    q0, a -> q1, X, R  (Mark 'a' as 'X')" << std::endl;
    std::cout << "    q1, b -> q2, Y, R  (Mark 'b' as 'Y')" << std::endl;
    std::cout << "    q2, c -> q3, Z, L  (Mark 'c' as 'Z', turn back)" << std::endl;
    std::cout << "    q3, X -> q0, X, R  (Rewind to 'X', then restart)" << std::endl;
    std::cout << "    q0, Y -> q4, Y, R  (All 'a's done, check rest)" << std::endl;
    std::cout << "    q4, \\0 -> ACCEPT   (End of string, valid)" << std::endl;

    std::cout << "\n--------------------------------------------" << std::endl;
    std::cout << "INSTRUCTIONS:" << std::endl;
    std::cout << "  - Enter strings to test." << std::endl;
    std::cout << "  - Valid:   aaabbbccc, abc, aabbcc" << std::endl;
    std::cout << "  - Invalid: aabbc, abbc, aabbccc" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    std::cout << "\nInput how many test cases you want to run: ";
    int test_cases;
    if (!(std::cin >> test_cases)) return 0;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Running " << test_cases << " test cases." << std::endl;
    
    while (test_cases-- > 0) {
        std::cout << "\nInput string (e.g., aaabbbccc): ";
        std::string test;
        std::getline(std::cin, test);
        
        // Echo input for visibility
        std::cout << test << std::endl;

        LBA machine(test);
        
        std::cout << "Result: " 
             << (machine.run() ? "[ ACCEPTED ]" : "[ REJECTED ]") << std::endl;
    }
    return 0;
}
