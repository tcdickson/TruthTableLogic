/*

The following program generates a truth table in binary format based on a logical connective. Currently, the code is set up to evaluate either 1 or two variable connectives. The code can easily be adapted to take more. I'm keeping the code smaller until I fully am able to acomplish all of the program goals. Also, the logical connective is currently hardcoded into the functions. 

I'm running into issues with dynamic eval and generation of connectives due to the nature of the C++ compiler at run time. I cannot pass strings into the function to evaluate as code.

Next steps: 

-Code logic to loop until user enters -1 (quits program)

-Explore defining logical connectives as enums, and passing the enum functions into the gernerateFor<Two>Variables functions. 

-Expanding the code to take up to at least 8 variables (this will force me to write all 256 possible outcomes) into the code.

- Create Logic circuit program after logic is fully written to generate the table. 
*/


/*The below enums represents the initial steps to mapping the variables and operators to enums that can be called within functions.*/
enum class Variable {
    A, B, C 
};

enum class LogicalOperator {
    AND, OR, NOT
};


#include <iostream>
#include <vector>
#include <functional>
#include <map>

using namespace std;

class TruthTable {
public:
/*The following function generates a truth table in binary. Ones and zeros for readability.*/
    void generateForTwoVariables() {
        /*First, all possible values are stored in a vector. The first number in the bracket relates to the fact that truth table's always display constants for the first two colums. */
        vector<vector<bool>> possibleCombinations = {
            {0, 0}, {0, 1}, {1, 0}, {1, 1}
        };
        // stores the results of the expression into the following vector
        vector<bool> results;

        /* Within the for loop, possible the combonations are looped through and the results of the inputed logical expression are appended onto the end of the constants for the first two rows of the truth table via the push_back command*/
        for (const auto& combo : possibleCombinations) {
            bool A = combo[0];
            bool B = combo[1];
            results.push_back(A && !B); 
        }

        /* First, the table header is printed, then we loop through the possibleCombinations vector expressed above, and print out the results, followed by a newline.*/
        cout << "A B | OUTCOME\n" << "---------\n";
        for (size_t i = 0; i < possibleCombinations.size(); ++i) {
            cout << possibleCombinations[i][0] << " " << possibleCombinations[i][1] << " | " << results[i] << "\n";
        }
    }
/*The following function is a duplicate funtion of the above function. The only difference is that this function generates a table for three variables.*/
    void generateForThreeVariables() {
        vector<vector<bool>> combinations = {
            {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
            {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}
        };
        vector<bool> results;

        for (const auto& combo : combinations) {
            bool A = combo[0];
            bool B = combo[1];
            bool C = combo[2];
            results.push_back((A && B )|| C);
        }

        cout << "A B C | OUTCOME\n"<< "-----------\n";
        for (size_t i = 0; i < combinations.size(); ++i) {
            cout << combinations[i][0] << " " << combinations[i][1] << " " << combinations[i][2] << " | " << results[i] << "\n";
        }
    }
};

int main() {
    TruthTable table;
    int userInput;
    cout << "Enter 2, followed by enter if your logical expression uses two variables. \nEnter 3, followed by enter if your logical expression uses three variables: ";
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


