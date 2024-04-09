
#include <cctype>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

using namespace std;
istream *input;

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
  NUMBER, // Placeholder for boolean values
  NAME,   // Placeholder for variables like 'A' and 'B'
  AND,    // Logical AND represented without using '&&'
  OR,     // Logical OR represented without using '||'
  NOT = '!',
  LP = '(',
  RP = ')'
};

TokenValue currTok;
;
bool boolValue;     // To hold the boolean value of the expression
string stringValue; // To hold variable names

TokenValue getToken() {
  char ch;

  if (!input->get(ch)) {

    return currTok = END;
  }

  while (ch != '\n' && isspace(ch)) {

    
  switch (ch) {
  case ';':
  case '\n':
    // return currTok = RESET;
  case '(':
  case ')':
  case '!':
    return currTok = TokenValue(ch);
  case '&':
    if (input->get(ch) && ch == '&') {
      cout << "ch value: " << ch << endl; // debug
      return currTok = AND;               // Correctly identified as logical AND

    } else {
      error("Bad token for &");
      if (ch != EOF)
        input->putback(ch); // Safely put back the character if it's not EOF
      return currTok = NONE;
    }
  case '|':
    if (input->get(ch) && ch == '|') {
      return currTok = OR; // Correctly identified as logical OR
    } else {
      error("Bad token for |");
      if (ch != EOF)
        input->putback(ch); // Safely put back the character if it's not EOF
      return currTok = NONE;
    }
  default:
    if (isalpha(ch)) {
      stringValue = ch;
      while (input->get(ch) && isalnum(ch))
        stringValue.push_back(ch);
      input->putback(ch);
      return currTok = NAME;
    }
    error("Bad token");
    return currTok = END;
  }
}

bool expr(bool);
bool prim(bool get) { // Handle primary expressions including NOT
  if (get)
    getToken();

  switch (currTok) {
  case NOT: // Logical NOT operation
    cout << "About to perform NOT operation, left value: " << left
         << endl; // Debug print current

    return !prim(true);
  case LP: {
    cout << "About to perform LP operation, left value: " << left
         << endl; // Debug print current

    bool e = expr(true);
    if (currTok != RP)
      return error(") expected");
    getToken(); // Consume ')'
    return e;
  }
  case NAME: {
    cout << "Evaluating variable: " << stringValue
         << " Current Token: " << currTok << endl; // Updated debug print
    if (table.find(stringValue) == table.end()) {
      return error("Unknown variable");
    }
    bool v = table[stringValue];
    getToken();
    return v;
  }

  default:
    return error("Primary expected");
  }
}

bool term(bool get) {
  if (get)
    getToken();
  bool left = prim(get);

  while (currTok == AND) { // Directly check if the next operation is AND
    cout << "About to perform AND operation, left value: " << left
         << endl; // Debug print current
    getToken();   // Move to the next token before evaluating the right operand
    left = left && prim(true); // Evaluate the AND operation
  }
  return left; // Return the result of AND operations or the result of prim if
               // no AND
}

bool expr(bool get) {
  if (get)
    getToken();

  bool left =
      term(get); // Evaluate the left side (which might include AND operations)

  while (currTok == OR) { // Only continue if an OR operation is present
    cout << "About to perform OR operation, left value: " << left
         << endl; // Debug print current
    getToken();   // Move to the next token before evaluating the right operand
    left = left || term(true); // Evaluate the OR operation with the next term
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
      input = &exprStream; // Set the global input pointer to our new stream

      // Reset parsing state if necessary
      // currTok = RESET; // Reset the current token

      // Evaluate the expression with updated 'A' and 'B' values
      bool result = expr(true);

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
