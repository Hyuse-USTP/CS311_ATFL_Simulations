#include <iostream>
#include <string>
#include <vector>
#include <limits>

// Removed 'using namespace std;'

// Simulates a Linear Bounded Automaton (LBA) for L = {a^n b^n c^n : n >= 1}
// This simulation mimics the tape head movement and state transitions.
class LBA {
public:
    LBA(std::string input) : tape(input), head(0) {}

    bool run() {
        // We treat the string as the tape. 
        // The LBA is bounded, so we cannot go beyond tape.length().
        
        // Algorithm:
        // 1. Find left-most 'a', mark as 'X'.
        // 2. Move right to find matching 'b', mark as 'Y'.
        // 3. Move right to find matching 'c', mark as 'Z'.
        // 4. Move left back to 'X'.
        // 5. Repeat.
        // 6. If we finish and tape is all X, Y, Z, accept.

        while (true) {
            // Reset head to start (simplification of "moving left until boundary")
            head = 0;

            // Step 1: Find first unmarked 'a'
            while (head < tape.length() && (tape[head] == 'X' || tape[head] == 'Y' || tape[head] == 'Z')) {
                head++;
            }

            // If we hit the end, check if everything is marked
            if (head == tape.length()) {
                return isFullyMarked();
            }

            // If we found something that is not 'a' (and not XYZ), it's an error (e.g., 'b' before 'a' finished)
            if (tape[head] != 'a') {
                return false; 
            }

            // Mark 'a' -> 'X'
            tape[head] = 'X';

            // Step 2: Find matching 'b'
            while (head < tape.length() && tape[head] != 'b') {
                // While moving right, we should only see 'a's (unmarked) or 'Y's (marked bs) or 'Z' (marked cs? no, shouldn't see Z yet if strict)
                // For simplicity, just scan for 'b'.
                head++;
            }

            if (head == tape.length()) return false; // No matching 'b' found

            // Mark 'b' -> 'Y'
            tape[head] = 'Y';

            // Step 3: Find matching 'c'
            while (head < tape.length() && tape[head] != 'c') {
                head++;
            }

            if (head == tape.length()) return false; // No matching 'c' found

            // Mark 'c' -> 'Z'
            tape[head] = 'Z';

            // Step 4: Loop continues, simulating moving back to start implicitly
        }
    }

private:
    std::string tape;
    size_t head;

    bool isFullyMarked() {
        for (char c : tape) {
            if (c != 'X' && c != 'Y' && c != 'Z') {
                return false;
            }
        }
        return true;
    }
};

int main() {
    std::cout << "--- LBA Language Recognizer (a^n b^n c^n) ---" << std::endl;
    std::cout << "This LBA machine checks if the input string is of the form a^n b^n c^n." << std::endl;
    std::cout << "Logic: It simulates marking symbols on a tape (a->X, b->Y, c->Z)." << std::endl;

    std::cout << "\nInput how many test cases you want to run: ";
    int test_cases;
    if (!(std::cin >> test_cases)) return 0;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    while (test_cases-- > 0) {
        std::cout << "\nInput string (e.g., aaabbbccc): ";
        std::string test;
        std::getline(std::cin, test);

        LBA machine(test);
        
        std::cout << "Testing '" << test << "': " 
             << (machine.run() ? "Accepted" : "Rejected") << std::endl;
    }
    return 0;
}