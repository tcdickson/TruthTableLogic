
#include <cctype>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
// istream *input;

int no_of_errors = 0;
map<string, bool> table;
string userInput;

double error(const char *s) {
  no_of_errors++;
  cerr << "error: " << s << '\n';
  return 1;
}

enum TokenValue {
  NONE,
  END,
  //   RESET = ';',
  NUMBER,  // Placeholder for boolean values
  DISPLAY, // Placeholder for variables like 'A' and 'B'
  AND,     // Logical AND represented without using '&&'
  OR,      // Logical OR represented without using '||'
  NOT = '!',
  LP = '(',
  RP = ')'
};

TokenValue currTok;
bool boolValue;     // To hold the boolean value of the expression
string stringValue; // To hold variable names

TokenValue getToken(istream &input) {
    char ch;
    if (!(input >> ws >> ch)) { // Skip whitespace and attempt to read a character
        return NONE; // No more input or an error occurred
    }

    switch (ch) {
        case '&':
            if (input.get(ch) && ch == '&') return AND; // Logical AND
            input.putback(ch); // In case it's not a logical AND
            break;
        case '|':
            if (input.get(ch) && ch == '|') return OR; // Logical OR
            input.putback(ch);
            break;
        case '(':
            return LP;
        case ')':
            return RP;
        case '!':
            return NOT;
        default:
            if (isalpha(ch)) { // Check if it's a variable
                stringValue = ch;
                while (input.get(ch) && isalnum(ch)) stringValue.push_back(ch);
                input.putback(ch); // Put back the last character read that's not part of the variable
                return DISPLAY;
            } else {
                // Handle unexpected characters
                error("Bad token");
                return NONE; // You might want to handle this case differently
            }
    }
    return NONE; // Return NONE if none of the cases match
}


bool orEvaluation(istream &input, bool get);

bool primaryExpression(istream &input, bool get) {
  if (get)
    getToken(input);

  if (currTok == NOT) {
    cout << "About to perform NOT operation, left value: " << left
         << endl; // Debug print current
    return !primaryExpression(input, true);

  } else if (currTok == LP) {
    cout << "About to perform LP operation, left value: " << left
         << endl; // Debug print current
    bool e = orEvaluation(input, true);
    if (currTok != RP)
      return error(") expected");
    getToken(input);
    return e;

  } else if (currTok == DISPLAY) {
    cout << "Evaluating display variable: " << stringValue
         << " Current Token: " << currTok << endl; // Updated debug print
    if (table.find(stringValue) == table.end()) {
      return error("Unknown variable");
    }
    bool v = table[stringValue];
    getToken(input);
    return v;
    
  } else {
    return error("Primary expected");
  }
}

bool andEvaluation(istream &input, bool get) {
  if (get)
    getToken(input);
  bool left = primaryExpression(input, get);

  while (currTok == AND) {
    cout << "About to perform AND operation, left value: " << left
         << endl; // Debug print current
    getToken(
        input); // Move to the next token before evaluating the right operand
    left = left && primaryExpression(input, true); // Evaluate the AND operation
  }
  return left; // Return the result of AND operations or the result of prim if
               // no AND
}

bool orEvaluation(istream &input, bool get) {
  if (get)
    getToken(input);

  bool left = andEvaluation(
      input,
      get); // Evaluate the left side (which might include AND operations)

  while (currTok == OR) { // Only continue if an OR operation is present
    cout << "About to perform OR operation, left value: " << left
         << endl; // Debug print current
    getToken(
        input); // Move to the next token before evaluating the right operand
    left = left ||
           andEvaluation(input,
                         true); // Evaluate the OR operation with the next term
  }
  return left; // Return the result of the expression, including any OR
               // operations
}

void processInput(istream &inputStream) {
  ostringstream expressionStream;
  char inputChar;
  while (inputStream.get(inputChar)) {
    if (inputChar == '\n' || inputChar == ';') {
      break;
    }
    expressionStream << inputChar;
  }
  userInput = expressionStream.str();
}

class TruthTable {
public:
  void generateForTwoVariables() {
    vector<vector<bool>> possibleCombinations = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}};
    cout << "A B | OUTCOME\n"
         << "---------\n";
    for (const auto &combo : possibleCombinations) {
      table["A"] = combo[0];
      table["B"] = combo[1];

      // Set up input stream from the user's expression
      istringstream exprStream(userInput);

    

      // Evaluate the expression with updated 'A' and 'B' values
      bool result = orEvaluation(exprStream, true);

      cout << combo[0] << " " << combo[1] << " | " << result << "\n";
    }
  }
};

int main(int argc, char *argv[]) {
  no_of_errors = 0;

  TruthTable table;

  if (argc > 1) {
    // If an expression is provided as a command-line argument
    istringstream argInput(argv[1]);
    processInput(argInput);
  } else {
    // Otherwise, read the expression from standard input
    processInput(std::cin);
  }

  table.generateForTwoVariables();

  return no_of_errors;
}
