/*
Course ID: CSC1060X02
Name Convention: CSC1060_tdickson_M3Project

The following program generates a truth table in binary format based on a
logical connective. I've spent quite a bit of time learning about tokens and
token parsing. Initially I thought this would be a straightforward program.
However, when I realized C++ does not allow you to parse user input to be
evaluated as logical connectives, the complexity of the assignment increased
quite a bit for me.

Currently, the code is able to evaluate 2 variable logical connectives
accurately using &&, and ||, and most instances of !. Currently, the !
expression is unable to acurately be parsed if it's the second expression
immediately following a separate expression i.e. A && !B. However, !A && B is
able to be parsed.

I had to do research to figure out how to parse tokens, and I took inspiration
from code used to build a calculator program from this site:
https://www.stroustrup.com/3rd_code.html

Next steps:

-Code logic to loop until user enters -1 (quits program)

-Explore defining logical connectives as enums, and passing the enum functions
into the gernerateFor<Two>Variables functions.

-Expanding the code to take up to at least 8 variables (this will force me to
write all 256 possible outcomes) into the code.

- Create Logic circuit program after logic is fully written to generate the
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
enum TokenValue { NONE, END, NUMBER, DISPLAY, AND, OR, NOT, IMPLIES, LP, RP };

TokenValue currTok;
string stringValue;
/*The getToken function recieves iser inpue passed from OR, AND, and NOT
 * functions. The orEvaluation, andEvaluation, and NotEvaluation use this
 * function to confirm what expression was passed. */
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
/*The following function generates a truth table in binary. Ones and zeros for readability.
First, all possible values are stored in a vector. The first number in the bracket relates to the fact that truth table's always display constants for the first two colums.*/

/*Within the for loop, possible the combinations are looped through and the results of the inputed logical expression are appended onto the end of the constants for the first two rows of the truth table via the push_back command*/
/* First, the table header is printed, then we loop through the possibleCombinations vector expressed above, and print out the results, followed by a newline.*/
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

      // cout << "left par: " << leftParenCount
      //      << " right par: " << rightParenCount << endl;

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

      // cout << "left par: " << leftParenCount << "right par: " << rightParenCount
      //      << endl;
      if (leftParenCount != rightParenCount) {
        error("Mismatched parentheses");
      }

      cout << combo[0] << " " << combo[1] << " " << combo[2] << " " << combo[3]
           << " " << combo[4] << " | " << result << "\n";
    }
  }
};
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
      cout << "Invalid choice. Please enter 2, 3, 4, or 0 to exit.\n";
      continue;
    }

    while (true) {
      cout << "Enter " << firstInput
           << " Variable Logical Expression (i.e a && b) \n";
      getline(cin, userInput);

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
