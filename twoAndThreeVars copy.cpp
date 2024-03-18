enum class Variable {
    A, B, C // Extend as needed
};

enum class LogicalOp {
    AND, OR, NOT // Basic logical operations; extend as needed
};


#include <iostream>
#include <vector>
#include <functional>
#include <map>
using namespace std;

class TruthTable {
public:
    void generateForTwoVariables(const std::function<bool(bool, bool)>& expr) {
        vector<vector<bool>> combinations = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
        cout << "A B | Result\n";
        cout << "-----------\n";
        for (const auto& combo : combinations) {
            bool result = expr(combo[0], combo[1]);
            cout << combo[0] << " " << combo[1] << " | " << result << "\n";
        }
    }

    void generateForThreeVariables(const std::function<bool(bool, bool, bool)>& expr) {
        vector<vector<bool>> combinations = {
            {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
            {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
        cout << "A B C | Result\n";
        cout << "----------------\n";
        for (const auto& combo : combinations) {
            bool result = expr(combo[0], combo[1], combo[2]);
            cout << combo[0] << " " << combo[1] << " " << combo[2] << " | " << result << "\n";
        }
    }
};

int main() {
    TruthTable tt;
    int choice;

    cout << "Enter 2 for a truth table with two variables, or 3 for three variables: ";
    cin >> choice;

    if (choice == 2) {
        auto expr = [](bool A, bool B) -> bool { return A || !B; }; // Example expression
        tt.generateForTwoVariables(expr);
    } else if (choice == 3) {
        auto expr = [](bool A, bool B, bool C) -> bool { return (A && B) || (B && C); }; // Example expression
        tt.generateForThreeVariables(expr);
    } else {
        cout << "Invalid choice. Please enter 2 or 3." << endl;
    }

    return 0;
}

