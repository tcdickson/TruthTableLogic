#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<vector<bool>> twoVarTable = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

vector<vector<bool>> threeVarTable = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0},
                                      {0, 1, 1}, {1, 0, 0}, {1, 0, 1},
                                      {1, 1, 0}, {1, 1, 1}};

string userInput;

int currentRow = 0; 
int no_of_errors = 0;

double error(const char *s) {
  no_of_errors++;
  cerr << "error: " << s << '\n';
  return 1;
}

enum TokenValue { NONE, END, DISPLAY, AND, OR, NOT = '!', LP = '(', RP = ')' };

class TruthTable {
public:
  string stringValue;
  vector<vector<bool>> *currentTable = nullptr;
  TruthTable() {}

  void generateForTwoVariables() {
    currentTable = &twoVarTable; 

    evaluateCurrentTable(); 
  }

  void generateForThreeVariables() {
    currentTable = &threeVarTable; 
    evaluateCurrentTable();     
  }

  TokenValue currTok;
  TokenValue getToken(istream &input) {
    char ch;
    while (input.get(ch) && (ch == '\n' || isspace(ch))); 

    cout << "After read, EOF: " << input.eof() << ", Good: " << input.good()
         << ", Char: " << ch << endl;

    // if (input.eof()) {
    //   cout << "Reached EOF, returning END." << endl;
    //   return currTok = END;
    // } else if (!input.good()) {
    //   cout << "Stream not good, returning END. CurrTok: " << currTok << endl;
    //   return currTok = END;
    // }
    cout << "right before switch " << endl;

    switch (ch) {
    case ';':
      cout << "Semicolon encountered, ending expression parsing." << endl;
      return currTok = END;
    case '(':
    case ')':
    case '!':
      // return currTok = TokenValue(ch);  
    case '&':
      cout << "encountered &" << endl;

      if (input.get(ch) && ch == '&')
        return currTok = AND; 
      else {
        error("Bad token for &");
        if (ch != EOF)
          input.putback(ch);
        return currTok = NONE;
      }
    case '|':
          cout << "encountered |" << endl;

      if (input.get(ch) && ch == '|')
        return currTok = OR; 
      else {
        error("Bad token for |");
        if (ch != EOF)
          input.putback(ch);
        return currTok = NONE;
      }
    default:
      if (isalpha(ch)) {
        stringValue = ch;
        while (input.get(ch) && isalnum(ch))
          stringValue.push_back(ch);
        input.putback(ch);          
        return currTok = DISPLAY;
      } else {
        error("Bad token");
        return currTok = END;
      }
    }

    return currTok = NONE; 
  }

  bool primaryExpression(istream &input, bool get) {
    if (get)
      getToken(input);

    switch (currTok) {
    case NOT: {
      cout << "About to perform NOT operation.\n";
      return !primaryExpression(input, true);
    }
    case LP: {
      bool e = orEvaluation(input, true);
      if (currTok != RP)
        return error(") expected");
      getToken(input); // Consume ')'
      return e;
    }
    case DISPLAY: {
      cout << "Evaluating display variable: " << stringValue << ", Value: "
           << "\n";

      int varIndex =
          stringValue[0] - 'A'; 
      cout << "currentRow: " << currentRow << ", varIndex: " << varIndex
           << ", vector size: " << currentTable->at(currentRow).size() << "\n";

      if (varIndex < 0 || varIndex >= currentTable->at(currentRow).size()) {
        return error("Unknown variable");
      }
      bool v = currentTable->at(currentRow)[varIndex];
      cout << "Value: " << v << "\n"; 
      return v;
    }
    }
    return error("Primary expected");
  }

  bool andEvaluation(istream &input, bool get) {
    if (get)
      getToken(input);
    bool left = primaryExpression(input, false); 
    while (currTok == AND) {
      cout << "About to perform AND operation.\n";
      getToken(input); 
      bool right = primaryExpression(input, true); 
      left = left && right; 
    }
    return left;
  }

  bool orEvaluation(istream &input, bool get) {
    cout << "entered orEvaluation " << userInput << "\n";
   

    if (get)
      getToken(input);

    bool left = andEvaluation(input, false); // Evaluate the first operand

    while (currTok == OR) {
      getToken(input);
      bool right = andEvaluation(input, true);
      left = left || right;
    }
    return left; 
  }

private:
  void evaluateCurrentTable() {

    if (!currentTable) {
      cout << "currentTable is nullptr.\n";
      return; 
    }
    cout << "A B | OUTCOME\n"
         << "---------\n";

    for (currentRow = 0; currentRow < currentTable->size(); ++currentRow) {
      // cout << "Entering current table. User input: " << userInput << "\n";
      // cout << "testing pointer output: " << currentTable->size() << endl;

  
      istringstream exprStream(userInput);

      bool result = orEvaluation(exprStream, true);

      for (bool val : (*currentTable)[currentRow]) {
        cout << val << " ";
      }
      cout << "| " << result << "\n";
    }
  }
};

int main(int argc, char *argv[]) {
  no_of_errors = 0;
  TruthTable table;
  int firstInput;

  cout << "Enter 2 for two variables, 3 for three variables: ";
  cin >> firstInput;
  cin.ignore(
      numeric_limits<streamsize>::max(),
      '\n'); 

  if (argc > 1) {
  
    userInput = argv[1];
  } else {
    string prompt = (firstInput == 2) ? "Enter 2 Variable Logical Expression: "
                                      : "Enter 3 Variable Logical Expression: ";
    cout << prompt;
    getline(cin,
            userInput); 
  }

  cout << "User input: " << userInput << "\n"; 
  if (firstInput == 2) {
    table.generateForTwoVariables();
  } else if (firstInput == 3) {
    table.generateForThreeVariables();
  } else {
    cout << "Invalid choice. Please enter 2 or 3.\n";
  }

  return no_of_errors;
}
