
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <vector>
using namespace std;

// class LogicalExpression {
//     public:
//     virtual bool evaluate(const vector<bool>& inputs) = 0;
// };

// class OrExpression : public LogicalExpression {
// public:
//     bool evaluate(const vector<bool>& inputs) override {
//         return inputs[0] || inputs[1];
//     }
// };

// class NotExpression : public LogicalExpression {
// public:
//     bool evaluate(const vector<bool>& inputs) override {
//         return !inputs[0];
//     }
// };

class ExpressionEvaluator {
public:
  bool evaluate(const string &expression,
                const unordered_map<string, bool> &variables) {

    vector<string> tokens = tokenize(expression);
    stack<bool> evalStack;

    for (const string &token : tokens) {
      if (token == "AND" || token == "OR") {
        bool right = evalStack.top();
        evalStack.pop();
        bool left = evalStack.top();
        evalStack.pop();
        bool result = token == "AND" ? left && right : left || right;
        evalStack.push(result);
      } else if (token == "NOT") {
        bool top = evalStack.top();
        evalStack.pop();
        evalStack.push(!top);
      } else { // Variable
        evalStack.push(variables.at(token));
      }
    }

    return evalStack.top();
  }

private:
  vector<string> tokenize(const string &expression) {

    vector<string> tokens;
    size_t start = 0, end = 0;
    while ((end = expression.find(' ', start)) != string::npos) {
      tokens.push_back(expression.substr(start, end - start));
      start = end + 1;
    }
    tokens.push_back(expression.substr(start));
    return tokens;
  }
};

class TruthTable {
public:
  void generateForTwoVariables(const string &expression) {
    vector<vector<bool>> possibleCombinations = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}};
    vector<bool> results;
    ExpressionEvaluator evaluator;

    for (const auto &combo : possibleCombinations) {
      unordered_map<string, bool> variables = {{"A", combo[0]},
                                               {"B", combo[1]}};
      bool result = evaluator.evaluate(expression, variables);
      results.push_back(result);
    }

    cout << "A B | OUTCOME\n"
         << "---------\n";
    for (size_t i = 0; i < possibleCombinations.size(); ++i) {
      cout << possibleCombinations[i][0] << " " << possibleCombinations[i][1]
           << " | " << results[i] << "\n";
    }
  }
};


// class TruthTable {
// public:

//     void generateForTwoVariables() {

//         vector<vector<bool>> possibleCombinations = {
//             {0, 0}, {0, 1}, {1, 0}, {1, 1}
//         };
       
//         vector<bool> results;
//         for (const auto& combo : possibleCombinations) {
//             bool A = combo[0];
//             bool B = combo[1];
//             results.push_back(A || !B); 
//         }

//         cout << "A B | OUTCOME\n" << "---------\n";
//         for (size_t i = 0; i < possibleCombinations.size(); ++i) {
//             cout << possibleCombinations[i][0] << " " << possibleCombinations[i][1] << " | " << results[i] << "\n";
//         }
//     }

    void generateForThreeVariables() {
  vector<vector<bool>> combinations = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0},
                                       {0, 1, 1}, {1, 0, 0}, {1, 0, 1},
                                       {1, 1, 0}, {1, 1, 1}};
  vector<bool> results;

  for (const auto &combo : combinations) {
    bool A = combo[0];
    bool B = combo[1];
    bool C = combo[2];
    results.push_back((A && B) || (B && C));
  }

  cout << "A B C | OUTCOME\n"
       << "-----------\n";
  for (size_t i = 0; i < combinations.size(); ++i) {
    cout << combinations[i][0] << " " << combinations[i][1] << " "
         << combinations[i][2] << " | " << results[i] << "\n";
  }
};
int main() {
    TruthTable table;
    string userInput;

    cout << "Enter your logical expression using A and B (e.g., 'A AND B', 'A OR NOT B'): ";
    getline(cin, userInput); 

    table.generateForTwoVariables(userInput);

    return 0;
}

// int main() {
//   TruthTable table;
//   int userInput;
//   cout << "Enter 2, followed by enter if your logical expression uses two "
//           "variables. \nEnter 3, followed by enter if your logical expression "
//           "uses three variables: ";
//   cin >> userInput;

//   if (userInput == 2) {
//     table.generateForTwoVariables();
//   } else if (userInput == 3) {
//     table.generateForThreeVariables();
//   } else {
//     cout << "Invalid choice. Please enter 2 or 3." << endl;
//   }

//   return 0;
// }

// int main() {
//     shared_ptr<LogicalExpression> expression;
//     int numVariables, expressionType;
//     cout << "Enter the number of variables: ";
//     cin >> numVariables;

//     cout << "Choose an expression (1=OR, 2=NOT): ";
//     cin >> expressionType;

//     switch (expressionType) {
//         case 1:
//             expression = make_shared<OrExpression>();
//             break;
//         case 2:
//             expression = make_shared<NotExpression>();
//             break;
//         default:
//             cout << "Invalid expression type." << endl;
//             return 1;
//     }

//     TruthTable table;
//     table.generate(expression, numVariables);

//     return 0;
// }
