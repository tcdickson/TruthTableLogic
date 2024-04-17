#include <iostream>
#include <vector>
using namespace std;


void printTruthTable(const vector<vector<bool>>& combinations, const vector<bool>& results) {
    cout << "A B C | Result\n";
    cout << "-------------\n";
    for (size_t i = 0; i < combinations.size(); ++i) {
        cout << combinations[i][0] << " " << combinations[i][1] << " " << combinations[i][2] << " | " << results[i] << "\n";
    }
}


int main() {
    vector<vector<bool>> combinations = { {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
    {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
    

    vector<bool> results;


    for (const auto& combo : combinations) {
        bool A = combo[0];
        bool B = combo[1];
         bool C = combo[2];
        
        
    results.push_back((A && B) || (B && C) || (!C));    }


    printTruthTable(combinations, results);

    return 0;
}
