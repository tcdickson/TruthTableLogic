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
// vector<vector<bool>> table;

string userInput;

double error(const char *s) {
  no_of_errors++;
  cerr << "error: " << s << '\n';
  return 1;
}

enum TokenValue { NONE, END, NUMBER, DISPLAY, AND, OR, NOT, LP, RP };

TokenValue currTok;
string stringValue; 

TokenValue getToken(istream &input) {
  char ch;
  while (input.get(ch) && (ch == '\n' || isspace(ch)))
    ; 

  cout << "After read, EOF: " << input.eof() << ", Good: " << input.good()
       << ", Char: " << ch << endl;

  if (input.eof()) {
    cout << "Reached EOF, returning END." << endl;
    return currTok = END;
  } else if (!input.good()) {
    cout << "Stream not good, returning END. CurrTok: " << currTok << endl;
    return currTok = END;
  }
  switch (ch) {
  case ';':
    cout << "Semicolon encountered, ending expression parsing." << endl;
    return currTok = DISPLAY; 
  case '(':
  case ')':
  case '!':
    cout << "encountered !" << endl;
    return currTok = NOT;
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
    cout << "default hit" << endl;
    cout << " ----------------------------------" << endl;
    if (isalpha(ch)) { 
      stringValue = ch;
      cout << " value of ch here: " << ch << endl;

      // Peek at the next character without removing it from the stream
      while (input.peek() && isalnum(input.peek())) {
        input.get(ch);             // Now, actually read the character
        stringValue.push_back(ch); // Accumulate full variable name
      }

  
      cout << " stringValue after accumulation: " << stringValue << endl;


      cout << " value of currTok here: " << currTok << endl;
      cout << " ----------------------------------" << endl;

      return currTok = DISPLAY;
    } else {
      error("Bad token");
      return currTok = END;
    }
  }

  return currTok = NONE; 
}

bool orEvaluation(istream &input, bool get);

bool primaryExpression(istream &input, bool get) {
  if (get)
    getToken(input);
  switch (currTok) {
  case DISPLAY: {
    cout << "Evaluating display variable: " << stringValue
         << ", Value: " << table[stringValue] << "\n";

    if (table.find(stringValue) == table.end()) {
      return error("Unknown variable");
    }
    bool v = table[stringValue];
    getToken(input); 
    return v;
  }
  case END: {
    return false;
  }
  default:
    return error("Primary expected");
  }
}
bool notEvaluation(istream &input, bool get) {
  if (get)
    getToken(input);
  cout << "Current Token after getToken: " << currTok << endl; 
 while (currTok == NOT) {
    cout << "About to perform NOT operation.\n";
    return !primaryExpression(
        input, true); 
      cout << "currTok must not == NOT" << endl;
  }
  return primaryExpression(input,
                           false);
}

bool andEvaluation(istream &input, bool get) {
  if (get)
    getToken(input);
  bool left = notEvaluation(input, false);

  while (currTok == AND) {
    cout << "About to perform AND operation.\n";
    getToken(input);
    bool right = notEvaluation(input, true); 
    left = left && right;                  
  }
  return left;
}

bool orEvaluation(istream &input, bool get) {
  if (get)
    getToken(input);

  bool left = andEvaluation(input, false);

  while (currTok == OR) { 
    cout << "About to perform OR operation.\n";

    getToken(input);
    bool right = andEvaluation(input, true);
    left = left || right;                  
  }
  return left; 
}

void processInput(istream &inputStream) {
  ostringstream expressionStream;
  char inputChar;
  while (inputStream.get(inputChar)) {
    if (inputChar == '\n') {
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

      cout << "user input: " << userInput << endl;
      istringstream exprStream(userInput);

     
      bool result = orEvaluation(exprStream, true);

      cout << combo[0] << " " << combo[1] << " | " << result << "\n";
    }
  }


  void generateForThreeVariables() {
    vector<vector<bool>> possibleCombinations = {
          {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
            {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
    cout << "A B C | OUTCOME\n"
         << "---------\n";
    for (const auto &combo : possibleCombinations) {
      table["A"] = combo[0];
      table["B"] = combo[1];
      table["C"] = combo[2];

      cout << "user input: " << userInput << endl;
      istringstream exprStream(userInput);

   
      bool result = orEvaluation(exprStream, true);

     cout << combo[0] << " " << combo[1] << " " << combo[2] << " | " << result
           << "\n";
    }
  }
};

// int main(int argc, char *argv[]) {
//   no_of_errors = 0;

//   TruthTable table;

//   if (argc > 1) {
//     // If an expression is provided as a command-line argument
//     istringstream argInput(argv[1]);
//     processInput(argInput);
//   } else {
//     // Otherwise, read the expression from standard input
//     processInput(std::cin);
//   }

//   table.generateForTwoVariables();

//   return no_of_errors;
// }
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