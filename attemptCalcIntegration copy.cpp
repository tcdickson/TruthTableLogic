
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
  NUMBER,  
  NAME,      
  AND,       
  OR,       
  NOT = '!',
  LP = '(',
  RP = ')'
};


TokenValue currTok;;
bool boolValue;     
string stringValue; 

TokenValue getToken() {
    char ch;
    do {
        if (!input->get(ch)) {
            

         return currTok = END;
             

        }
    } while (ch != '\n' && isspace(ch));
    

    switch (ch) {
    case ';':
    case '\n':
        
    case '(':
    case ')':
    case '!':
        return currTok = TokenValue(ch);
    case '&':
        if (input->get(ch) && ch == '&') {
         cout << "ch value: " << ch << endl; //debug
            return currTok = AND; 
        } else {
            error("Bad token for &");
            if (ch != EOF) input->putback(ch); 
            return currTok = NONE;
        }
    case '|':
        if (input->get(ch) && ch == '|') {
            return currTok = OR; 
        } else {
            error("Bad token for |");
            if (ch != EOF) input->putback(ch); 
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
bool prim(bool get) { 
  if (get)
    getToken();

  switch (currTok) {
  case NOT: 
    cout << "About to perform NOT operation, left value: " << left << endl;     

    return !prim(true);
  case LP: {
      cout << "About to perform LP operation, left value: " << left << endl;    

    bool e = expr(true);
    if (currTok != RP)
      return error(") expected");
    getToken(); 
    return e;
  }
case NAME: {
    cout << "Evaluating variable: " << stringValue << " Current Token: " << currTok << endl; 
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
  if (get) getToken();
  bool left = prim(get);

  while (currTok == AND) { 
  cout << "About to perform AND operation, left value: " << left << endl;   
    getToken(); 
    left = left && prim(true); 
  }
  return left; 
}

bool expr(bool get) {
  if (get) getToken();

  bool left = term(get); 

  while (currTok == OR) {
  cout << "About to perform OR operation, left value: " << left << endl;    // Debug print current 
    getToken(); 
    left = left || term(true); 
  }
  return left;
}



void processInput(istream& inputStream) {
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
            {0, 0}, {0, 1}, {1, 0}, {1, 1}
        };
        cout << "A B | OUTCOME\n" << "---------\n";
        for (const auto& combo : possibleCombinations) {
            table["A"] = combo[0];
            table["B"] = combo[1];

          
            istringstream exprStream(userInput);
            input = &exprStream; 

           
            bool result = expr(true);

            cout << combo[0] << " " << combo[1] << " | " << result << "\n";
        }
    }
};


int main(int argc, char* argv[]) {
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
