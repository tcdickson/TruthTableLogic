
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
  NUMBER,  
  DISPLAY, 
  AND,     
  OR,     
  NOT = '!',
  LP = '(',
  RP = ')'
};

TokenValue currTok;
bool boolValue;     
string stringValue; // To hold variable names

TokenValue getToken(istream &input) {
    char ch;
    if (!(input >> ws >> ch)) { 
        return NONE; 
    }

    switch (ch) {
        case '&':
            if (input.get(ch) && ch == '&') return AND;
            input.putback(ch); 
            break;
        case '|':
            if (input.get(ch) && ch == '|') return OR; 
            input.putback(ch);
            break;
        case '(':
            return LP;
        case ')':
            return RP;
        case '!':
            return NOT;
        default:
            if (isalpha(ch)) { 
                stringValue = ch;
                while (input.get(ch) && isalnum(ch)) stringValue.push_back(ch);
                input.putback(ch); 
                return DISPLAY;
            } else {
               
                error("Bad token");
                return NONE; 
            }
    }
    return NONE; 
}


bool orEvaluation(istream &input, bool get);

bool primaryExpression(istream &input, bool get) {
  if (get)
    getToken(input);

  if (currTok == NOT) {
    cout << "About to perform NOT operation, left value: " << left
         << endl; 
    return !primaryExpression(input, true);

  } else if (currTok == LP) {
    cout << "About to perform LP operation, left value: " << left
         << endl; 
    bool e = orEvaluation(input, true);
    if (currTok != RP)
      return error(") expected");
    getToken(input);
    return e;

  } else if (currTok == DISPLAY) {
    cout << "Evaluating display variable: " << stringValue
         << " Current Token: " << currTok << endl;
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
         << endl;
    getToken(
        input); 
    left = left && primaryExpression(input, true); 
  }
  return left; 
}

bool orEvaluation(istream &input, bool get) {
  if (get)
    getToken(input);

  bool left = andEvaluation(
      input,
      get); 

  while (currTok == OR) { 
    cout << "About to perform OR operation, left value: " << left
         << endl; 
    getToken(
        input); 
    left = left ||
           andEvaluation(input,
                         true); 
  }
  return left; 
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

      istringstream exprStream(userInput);

      bool result = orEvaluation(exprStream, true);

      cout << combo[0] << " " << combo[1] << " | " << result << "\n";
    }
  }
};

int main(int argc, char *argv[]) {
  no_of_errors = 0;

  TruthTable table;

  if (argc > 1) {

    istringstream argInput(argv[1]);
    processInput(argInput);
  } else {
    processInput(std::cin);
  }

  table.generateForTwoVariables();

  return no_of_errors;
}
