#include <iostream>
#include <vector>
using namespace std;

void generateTruthTableForTwoVariables() {
    vector<vector<bool>> combinations = {
        {0, 0}, {0, 1},
        {1, 0}, {1, 1}
    };
    vector<bool> results;

    for (const auto& combo : combinations) {
        bool A = combo[0];
        bool B = combo[1];
        results.push_back(A || !B); 
    }

    cout << "A B | Result\n";
    cout << "---------\n";
    for (size_t i = 0; i < combinations.size(); ++i) {
        cout << combinations[i][0] << " " << combinations[i][1] << " | " << results[i] << "\n";
    }
}


void generateTruthTableForThreeVariables() {
    vector<vector<bool>> combinations = {
        {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
        {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}
    };
    vector<bool> results;

    for (const auto& combo : combinations) {
        bool A = combo[0];
        bool B = combo[1];
        bool C = combo[2];
        results.push_back((A && B) || (B && C)); 
    }

    cout << "A B C | Result\n";
    cout << "-----------\n";
    for (size_t i = 0; i < combinations.size(); ++i) {
        cout << combinations[i][0] << " " << combinations[i][1] << " " << combinations[i][2] << " | " << results[i] << "\n";
    }
}
int main() {
    int choice;
    cout << "Enter 2 for a truth table with two variables, or 3 for three variables: ";
    cin >> choice;

    if (choice == 2) {
        generateTruthTableForTwoVariables();
    } else if (choice == 3) {
        generateTruthTableForThreeVariables();
    } else {
        cout << "Invalid choice. Please enter 2 or 3." << endl;
    }

    return 0;
}

