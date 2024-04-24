/*
The following program generates a truth table in binary format based on a logical expression. 
I took inspiration from code used to build a calculator program from this site:
https://www.stroustrup.com/3rd_code.html

The program fully parses up to 5 variable logical connectives.

The only exeption is when two identical parenthesis are encountered before a closing parenthesis is encountered. For example:
 (!a || !b) -> (c && d) && ((c -> e) && !e)

Current program shortfalls: 

* Currently, the left and right parenthesis evaluate only what is in between one left, and one right parenthesis. For example, the following logical expression would present an incorrect truth table:

(!a || (c || !b))

The parser evaluates the expressions one at a time. Therefore when a left parenthesis is encountered, it expects the next parenthesis to be a closing one. 

* Absence of the biconditional if and only if (<->)
* Absence of the Exclusive Or (XOR)

Next steps:
*Fix parenthesis bug
*Addition of biconditional and exclisive or
*Create Logic circuit program after logic is fully written to generate the
table.
*/

#include <cctype>
#include <cmath>
#include <functional>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/*Initial global parameters are established. binary representations are stored
 * in a map in order to be easily associated with char values*/

int no_of_errors = 0;
map<string, bool> table;

string userInput;
int leftParenCount = 0;
int rightParenCount = 0;

double error(const char *s) {
  no_of_errors++;
  cerr << "error: " << s << '\n';
  return 1;
}
/*Tokens for logical operators are defined in TokenValue enum */
enum TokenValue {NONE, END, DISPLAY, AND, OR, NOT, IMPLIES, LP, RP };

TokenValue currTok;
string stringValue;
/*The getToken function is called to determine which tokens were identified. Each row is cycled through, and is evaluated if they reach a recognized function. Specific itentifiers are recognized as Char values, and subsequently parsed into Token values. OR, AND, IMPLIES, NOT, and left and right Parentheses all are token values recognized be respective functions. */

TokenValue getToken(istream &input) {
  char ch;
  while (input.get(ch) && (ch == '\n' || isspace(ch)))
    ;

  if (input.eof()) {

    return currTok = END;
  } else if (!input.good()) {

    return currTok = END;
  }

  switch (ch) {
  case ';':

    return currTok = DISPLAY;
  case '(':
    leftParenCount++;
    return currTok = LP;

  case ')':
    rightParenCount++;
    return currTok = RP;

  case '!':
    return currTok = NOT;

  case '&':

    if (input.get(ch) && ch == '&') {

      return currTok = AND;

    } else {
      error("Bad token for &");
      if (ch != EOF)
        input.putback(ch);

      return currTok = NONE;
    }
  case '|':

    if (input.get(ch) && ch == '|')
      return currTok = OR;
    else {
      error("Bad token for |");
      if (ch != EOF)
        input.putback(ch);
      return currTok = NONE;
    }
  case '-':

    if (input.get(ch) && ch == '>')

      return currTok = IMPLIES;
    else {
      error("Bad token for ->");
      if (ch != EOF)
        input.putback(ch);
      return currTok = NONE;
    }
  default:

    if (isalpha(ch)) {
      stringValue = ch;
      while (input.peek() && isalnum(input.peek())) {
        input.get(ch);

        stringValue.push_back(ch);
      }
      return currTok = DISPLAY;
    } else {
      error("Bad token");
      return currTok = END;
    }
  }

  return currTok = NONE;
}
/*Initialization of functions handling parsing of tokens*/
bool orEvaluation(istream &input, bool get);
bool notEvaluation(istream &input, bool get);
bool andEvaluation(istream &input, bool get);
bool impliesEvaluation(istream &input, bool get);
bool parenthesisEvaluation(istream &input, bool get);
bool expression(istream &input, bool get);

bool primaryExpression(istream &input, bool get) {
  if (get)
    getToken(input);

  switch (currTok) {
  case DISPLAY: {

    if (table.find(stringValue) == table.end()) {
      return error("Unknown variable");
    }
    bool v = table[stringValue];
    getToken(input);
    return v;
  }
  case LP: {
    bool e = expression(input, true);
    // if (currTok != RP)
    //   return error("')' expected - primary expression");
    getToken(input);
    return e;
  }
  case END: {
    return false;
  }
  default:
    return error("Primary expected");
  }
}

bool notEvaluation(istream &input, bool get) {

  while (currTok == NOT) {

    return !primaryExpression(input, true);
  }
  return primaryExpression(input, false);
}

bool andEvaluation(istream &input, bool get) {

  bool left = notEvaluation(input, false);

  while (currTok == AND) {

    getToken(input);
    bool right = notEvaluation(input, true);
    left = left && right;
  }
  return left;
}

bool orEvaluation(istream &input, bool get) {

  bool left = andEvaluation(input, false);

  while (currTok == OR) {

    getToken(input);
    bool right = andEvaluation(input, true);
    left = left || right;
  }
  return left;
}

bool impliesEvaluation(istream &input, bool get) {

  bool left = orEvaluation(input, get);

  while (currTok == IMPLIES) {

    getToken(input);
    bool right = orEvaluation(input, true);
    left = !left || right;
  }
  return left;
}

/*/*The parenthesisEvaluation accepts user inputed expression as a stream, and immediately
 * calls the getToken function in order to establish which logical operator the
 * user inputed. Then, the parenthesisEvaluation establishes positioning by passing
 * the input to the impliesEvaluation, which passes it to orEvaluation, andEvaluation, and notEvaluation. This is based on logical expression order of precedence. If a token is identified, it moves into the while loop for evaluation*/

 /*if current token equals LP,, we need to evauate the variable on the left, with the variable on the right. That means we must check first for any other TOKEN value by callint the getToken function, then we need to check it for every possible value. Therefore we call the next function again, and it cycles performs a check for every possible outcome*/

bool parenthesisEvaluation(istream &input, bool get) {
  if (get)
    getToken(input);

  while (currTok == LP) {
    bool exprValue = impliesEvaluation(input, true);

    getToken(input);

    return exprValue;
  }
  return impliesEvaluation(input, false);
}



bool expression(istream &input, bool get) {
  return parenthesisEvaluation(input, get);
}

/* The truthTable class holds different constant combinations for standard 2,
 * 3, 4 and 5 variable combinations. Additionally, user input is passed into a stream,
 * then subsequently passed into the orEvaluation function. orEvaluation takes a
 * boolean value to generate truth table representations of 1s and 0s */

class TruthTable {
public:
  void generateForTwoVariables() {
     vector<vector<bool>> possibleCombinations = {
        {1, 1},{1, 0}, {0, 1}, {0, 0} };


    leftParenCount = 0;
    rightParenCount = 0;
    cout << "A B | OUTCOME\n"
         << "---------\n";
    for (const auto &combo : possibleCombinations) {
      table["a"] = combo[0];
      table["b"] = combo[1];

      istringstream exprStream(userInput);

      bool result = expression(exprStream, true);

      if (leftParenCount != rightParenCount) {
        error("Mismatched parentheses");
      }

      cout << combo[0] << " " << combo[1] << " | " << result << "\n";
    }
  }

  void generateForThreeVariables() {
    vector<vector<bool>> possibleCombinations = {  {1, 1, 1}, {1, 1, 0},  {1, 0, 1}, {1, 0, 0}, {0, 1, 1}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}};
    cout << "A B C | OUTCOME\n"
         << "---------\n";
    for (const auto &combo : possibleCombinations) {
      table["a"] = combo[0];
      table["b"] = combo[1];
      table["c"] = combo[2];

      istringstream exprStream(userInput);

      bool result = expression(exprStream, true);
      if (leftParenCount != rightParenCount) {
        error("Mismatched parentheses");
      }

      cout << combo[0] << " " << combo[1] << " " << combo[2] << " | " << result
           << "\n";
    }
  }
  void generateForFourVariables() {
   vector<vector<bool>> possibleCombinations = {
    {1, 1, 1, 1}, {1, 1, 1, 0}, {1, 1, 0, 1}, {1, 1, 0, 0},
    {1, 0, 1, 1}, {1, 0, 1, 0}, {1, 0, 0, 1}, {1, 0, 0, 0},
    {0, 1, 1, 1}, {0, 1, 1, 0}, {0, 1, 0, 1}, {0, 1, 0, 0},
    {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 1}, {0, 0, 0, 0}
};

    cout << "A B C D | OUTCOME\n"
         << "-----------------\n";
    for (const auto &combo : possibleCombinations) {

      table["a"] = combo[0];
      table["b"] = combo[1];
      table["c"] = combo[2];
      table["d"] = combo[3];

      istringstream exprStream(userInput);

      bool result = expression(exprStream, true);



      if (leftParenCount != rightParenCount) {
        error("Mismatched parentheses");
      }

      cout << combo[0] << " " << combo[1] << " " << combo[2] << " " << combo[3]
           << " | " << result << "\n";
    }
  }
  void generateForFiveVariables() {
    vector<vector<bool>> possibleCombinations = {
    {1, 1, 1, 1, 1}, {1, 1, 1, 1, 0}, {1, 1, 1, 0, 1}, {1, 1, 1, 0, 0},
    {1, 1, 0, 1, 1}, {1, 1, 0, 1, 0}, {1, 1, 0, 0, 1}, {1, 1, 0, 0, 0},
    {1, 0, 1, 1, 1}, {1, 0, 1, 1, 0}, {1, 0, 1, 0, 1}, {1, 0, 1, 0, 0},
    {1, 0, 0, 1, 1}, {1, 0, 0, 1, 0}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1}, {0, 1, 1, 1, 0}, {0, 1, 1, 0, 1}, {0, 1, 1, 0, 0},
    {0, 1, 0, 1, 1}, {0, 1, 0, 1, 0}, {0, 1, 0, 0, 1}, {0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1}, {0, 0, 1, 1, 0}, {0, 0, 1, 0, 1}, {0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1}, {0, 0, 0, 1, 0}, {0, 0, 0, 0, 1}, {0, 0, 0, 0, 0}
};

    cout << "A B C D E | OUTCOME\n"
         << "--------------------\n";
    leftParenCount = 0;
    rightParenCount = 0;
    for (const auto &combo : possibleCombinations) {
      table["a"] = combo[0];
      table["b"] = combo[1];
      table["c"] = combo[2];
      table["d"] = combo[3];
      table["e"] = combo[4];

      istringstream exprStream(userInput);

      bool result = expression(exprStream, true);

      if (leftParenCount != rightParenCount) {
        error("Mismatched parentheses");
      }

      cout << combo[0] << " " << combo[1] << " " << combo[2] << " " << combo[3]
           << " " << combo[4] << " | " << result << "\n";
    }
  }
};
/*The validateInput function ensures the correct input has been entered for whichever truth table generation function has been called. For example, if the user chooses input of 2, and they enter 3 variables, it prints a custom error message and allows for an additional entry.*/

bool validateInput(const string &input, int varCount) {
  set<char> variables;
  for (char ch : input) {
    if (isalpha(ch)) {
      variables.insert(ch);
    }
  }
  if (variables.size() != varCount) {
    error("\nNumber of variables does not match the expected count.");
    cout << "The Expected Variable count is: " << varCount << "\nYou entered "
         << userInput << ", which is an expression with " << variables.size()
         << " variables. \n"
         << endl;
    return false;
  }
  return true;
}


/*the main function allows users to input the expression. user input is passed
 * as an argument into the main function, and subsequently, values are generated
 * based on various tables. */
int main(int argc, char *argv[]) {
  TruthTable table;
  int firstInput;


// Instructions to the user.
cout << "\nWelcome to the Truth Table Generator. This program receives a logical expression as a series of a connectives, and parses the output as a binary representation of a truth table. Each logical connective is replaced by universal programming logical operators in accordance with the following map: \n ¬ = !\n V = ||\n ∧ = &&\n → = -> \n\n All variables are replaced with a,b,c,d, and e for simplicity.\n Therefore, the following statement: (¬ R V ¬F) →(S ∧ L) is entered as: (!a || !b) -> (c && d)\n" << endl;

  do {
  
    cout << "Enter 2 for a logical expression involving two variables\n";
    cout << "Enter 3 for a logical expression involving three variables\n";
    cout << "Enter 4 for a logical expression involving four variables\n";
    cout << "Enter 5 for a logical expression involving five variables\n";

    cout << "Enter 0 to exit\n";
    cin >> firstInput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (firstInput == 0) {
      cout << "Exiting truth table generation.\n";
      break;
    }

    if (firstInput < 2 || firstInput > 5) {
      cout << "Invalid choice. Please enter 2, 3, 4, 5, or 0 to exit.\n";
      continue;
    }

    while (true) {
      cout << "Enter " << firstInput
           << " Variable Logical Expression: i.e: a && b \n";
      getline(cin, userInput);

// Specific functions are invoked within the TruthTable class based on user input (denoted by a switch statement)
      if (validateInput(userInput, firstInput)) {
        switch (firstInput) {
        case 2:
          table.generateForTwoVariables();
          break;
        case 3:
          table.generateForThreeVariables();
          break;
        case 4:
          table.generateForFourVariables();
          break;
        case 5:
          table.generateForFiveVariables();
          break;
        }
        break;
      } else {
        cout << "Invalid expression. Please re-enter the expression "
                "correctly.\n";
      }
    }

    while (true) {
      cout << "Would you like to enter another expression? (yes/no):\n";
      string decision;
      getline(cin, decision);

      if (decision == "no" || decision == "n") {
        cout << "Exiting truth table generation.\n";
        return 0;
      } else if (decision == "yes" || decision == "y") {
        break;
      } else {
        cout << "Invalid entry. Please type 'yes' or 'no'.\n";
      }
    }
  } while (true);

  return 0;
}