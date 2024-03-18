#include <iostream>
#include <vector>
using namespace std;

// Function to print the truth table for a given logical expression
void printTruthTable(const vector<vector<bool>>& combinations, const vector<bool>& results) {
    cout << "A B C | Result\n";
    cout << "-------------\n";
    for (size_t i = 0; i < combinations.size(); ++i) {
        cout << combinations[i][0] << " " << combinations[i][1] << " " << combinations[i][2] << " | " << results[i] << "\n";
    }
}


int main() {
    // Define all combinations of A and B for 2 variables
    vector<vector<bool>> combinations = { {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
    {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
    
    // Vector to store the results of the logical expression for each combination
    vector<bool> results;

    // Evaluate a specific logical expression for each combination of A and B
    for (const auto& combo : combinations) {
        bool A = combo[0];
        bool B = combo[1];
         bool C = combo[2];
        
        // Example logical expression: (A AND B) OR (NOT A)
        // Replace the expression inside the push_back with your desired logical operation
    results.push_back((A && B) || (B && C) || (!C));    }

    // Print the generated truth table
    printTruthTable(combinations, results);

    return 0;
}
