/*
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
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/*Initial global parameters are established. binary representations are stored
 * in a map in order to be easily associated with char values*/

int no_of_errors = 0;
map<string, bool> table;

string userInput;

double error(const char *s) {
  no_of_errors++;
  cerr << "error: " << s << '\n';
  return 1;
}
/*Tokens for logical operators are defined in TokenValue enum */
enum TokenValue { NONE, END, IMPLIES, DISPLAY, AND, OR, NOT, LP, RP };

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
    return currTok = LP;
  case ')':
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

/* primaryExpression receives the user inputted value after the OR, AND, and NOT
 * expressions have been evaluated, and receives the values after they've been
 * evaluated from the global map, and returns the map for that particular value.
 * The cycle repeats until all mapped values are evaluated.  */
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
    bool e = expression(
        input,
        true); // Recursively evaluate the expression inside the parentheses
    if (currTok != RP)
      return error("')' expected - primary expression");
    getToken(input);
    return e;
  }
  case END: {
    return false;
  }
  default:
    return error("Primary expected");
    return error(" ");
  }
}
/*notEvaluation takes the input pased from andEvaluation, and imediately passes
 * it into the getPrimary function, performs steps neccessary to evaluate the
 * expression. */
bool notEvaluation(istream &input, bool get) {
  while (currTok == NOT) {
    return !primaryExpression(input, true);
  }
  return primaryExpression(input, false);
}
/*andEvalustion takes the input pased from OrEvalution, and imediately passes it
 * into the getToken function, which checks which operator, or char value was
 * inputted. If And function is called, it's evaluated against the previous
 * value.*/


bool andEvaluation(istream &input, bool get) {
  bool left = notEvaluation(input, false);
  while (currTok == AND) {
    getToken(input);
    bool right = notEvaluation(input, true);
    left = left && right;
  }
  return left;
}
/*The orEvaluation accepts user inputed expression as a stream, and immediately
 * calls the getToken function in order to establish which logical operator the
 * user inputed. Then, the orEvaluation then establishes positioning by pasing
 * the input to andEvaluation, then notEvaluation. This is based on logical
 * expression order of precedence. If it reaches the orEvaluation, it's
 * evaluated against the previous value. */

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
    cout << "About to perform IMPLIES operation.\n";

    getToken(input);
    bool right =
        orEvaluation(input, true); 
    left = !left || right;  
  }
  return left;
}

bool parenthesisEvaluation(istream &input, bool get) {
  cout << "just entered parenthesisEvaluation" << endl;

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



/* The truthTable class holds different constant combinations for standard 2 and
 * 3 variable combinations. Additionally, user input is passed into a stream,
 * then subsequently passed into the orEvaluation function. orEvaluation takes a
 * boolean value to generate truth table representations of 1s and 0s */
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

      istringstream exprStream(userInput);

      bool result = orEvaluation(exprStream, true);

      cout << combo[0] << " " << combo[1] << " " << combo[2] << " | " << result
           << "\n";
    }
  }
};

/*the main function allows users to input the expression. user input is passed
 * as an argument into the main function, and subsequently, values are generated
 * based on various tables. I still need to build more appropriate error
 * handling for user input. */
int main(int argc, char *argv[]) {
  no_of_errors = 0;
  TruthTable table;
  int firstInput;

  cout << "Enter 2 for a logical expression involving two variables " << endl;
  cout << "Enter 3 for a logical expression involving three variables " << endl;
  cin >> firstInput;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  if (argc > 1) {

    userInput = argv[1];
  } else {

    string userPrompt = (firstInput == 2)
                            ? "Enter 2 Variable Logical Expression ending with "
                              "a space and semicolon like such: A && B ; "
                            : "Enter 3 Variable Logical Expression ending with "
                              "a semicolon like such: A && B || C ; ";
    cout << userPrompt;
    getline(cin, userInput);
  }

  if (firstInput == 2) {
    table.generateForTwoVariables();
  } else if (firstInput == 3) {
    table.generateForThreeVariables();
  } else {
    cout << "Invalid choice. Please enter 2 or 3.\n";
  }

  return no_of_errors;
}