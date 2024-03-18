#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <map>
#include <functional>

using namespace std;

// Utility function to print a truth table header
void printHeader(const vector<char>& variables) {
    for (auto var : variables) {
        cout << var << " ";
    }
    cout << "Result" << endl;
}

// Evaluate the expression based on the current values of the variables
bool evaluateExpression(const string& expression, const map<char, bool>& values) {
    // Example evaluation function, replace with actual logic to evaluate your expression
    // This example only handles a simple AND operation between two variables
    if (expression == "p&&q") { // Assuming expression "p&&q" for demonstration
        return values.at('p') && values.at('q');
    }
    // Add more cases as needed for other expressions
    return false;
}

int main() {
    // Example: User inputs an expression involving p, q (like "p&&q")
    string expression;
    cout << "Enter a logical expression (e.g., p&&q): ";
    cin >> expression;

    vector<char> variables = {'p', 'q'}; // Update based on the expression
    int numRows = pow(2, variables.size());
    printHeader(variables);

    for (int i = 0; i < numRows; ++i) {
        map<char, bool> values;
        for (int j = 0; j < variables.size(); ++j) {
            values[variables[j]] = ((i >> j) & 1);
        }

        // Print current combination of true/false for variables
        for (auto var : variables) {
            cout << values[var] << " ";
        }

        // Evaluate the expression for the current combination of variables
        bool result = evaluateExpression(expression, values);
        cout << result << endl; // Print the result of the expression
    }

    return 0;
}
