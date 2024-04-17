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


bool evaluateExpression(const string& expression, const map<char, bool>& values) {
 
    if (expression == "p&&q") { 
        return values.at('p') && values.at('q');
    }
    return false;
}

int main() {
  
    string expression;
    cout << "Enter a logical expression (e.g., p&&q): ";
    cin >> expression;

    vector<char> variables = {'p', 'q'}; 
    int numRows = pow(2, variables.size());
    printHeader(variables);

    for (int i = 0; i < numRows; ++i) {
        map<char, bool> values;
        for (int j = 0; j < variables.size(); ++j) {
            values[variables[j]] = ((i >> j) & 1);
        }

        
        for (auto var : variables) {
            cout << values[var] << " ";
        }

        bool result = evaluateExpression(expression, values);
        cout << result << endl; 
    }

    return 0;
}
