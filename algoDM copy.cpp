

enum class Variable { A, B, C };

enum class LogicalOperator { AND, OR, NOT };

#include <functional>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class TruthTable {
public:
  
  void generateForTwoVariables() {
 
    vector<vector<bool>> possibleCombinations = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}};
    vector<bool> results;

    for (const auto &combo : possibleCombinations) {
      bool A = combo[0];
      bool B = combo[1];
      results.push_back(A || !B);
    }

   
    cout << "A B | OUTCOME\n"
         << "---------\n";
    for (size_t i = 0; i < possibleCombinations.size(); ++i) {
      cout << possibleCombinations[i][0] << " " << possibleCombinations[i][1]
           << " | " << results[i] << "\n";
    }
  }

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
  }
};

int main() {
  TruthTable table;
  int userInput;
  cout << "Enter 2, followed by enter if your logical expression uses two "
          "variables. \nEnter 3, followed by enter if your logical expression "
          "uses three variables: ";
  cin >> userInput;

  if (userInput == 2) {
    table.generateForTwoVariables();
  } else if (userInput == 3) {
    table.generateForThreeVariables();
  } else {
    cout << "Invalid choice. Please enter 2 or 3." << endl;
  }

  return 0;
}