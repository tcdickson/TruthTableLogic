enum class Variable {
    A, B, C 
};

enum class LogicalOp {
    AND, OR, NOT
};


#include <iostream>
#include <vector>
#include <functional>
#include <map>

using namespace std;

using BinOpFunc = std::function<bool(bool, bool)>;
std::map<LogicalOp, BinOpFunc> binOpMap = {
    {LogicalOp::AND, [](bool a, bool b) { return a && b; }},
    {LogicalOp::OR, [](bool a, bool b) { return a || b; }}
};
using UnaryOpFunc = std::function<bool(bool)>;

std::map<LogicalOp, UnaryOpFunc> unaryOpMap = {
    {LogicalOp::NOT, [](bool a) { return !a; }}
};

class TruthTable {
public:
    // Method to generate a truth table for two variables
    void generateForTwoVariables() {
        vector<vector<bool>> combinations = {
            {0, 0}, {0, 1}, {1, 0}, {1, 1}
        };
        vector<bool> results;

        for (const auto& combo : combinations) {
            bool A = combo[0];
            bool B = combo[1];
            results.push_back(A || !B); // Modify this line for different expressions
        }

        cout << "A B | Result\n";
        cout << "---------\n";
        for (size_t i = 0; i < combinations.size(); ++i) {
            cout << combinations[i][0] << " " << combinations[i][1] << " | " << results[i] << "\n";
        }
    }

    // Method to generate a truth table for three variables
    void generateForThreeVariables() {
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
};

int main() {
    TruthTable table;
    int choice;
    cout << "Enter 2 for a truth table with two variables, or 3 for three variables: ";
    cin >> choice;

    if (choice == 2) {
        table.generateForTwoVariables();
    } else if (choice == 3) {
        table.generateForThreeVariables();
    } else {
        cout << "Invalid choice. Please enter 2 or 3." << endl;
    }

    return 0;
}


