#include <iostream>
#include <functional>
using namespace std;

bool getBit(unsigned int uint, int position) {
    return (uint >> position) & 0x1;
}

void generateTruthTable(function<bool(bool, bool, bool)> logicFunction) {
    bool p, q, r;

    for (unsigned int i = 0; i < 8; i++) {
        p = getBit(i, 0);
        q = getBit(i, 1);
        r = getBit(i, 2);

        cout << p << "  ";
        cout << q << "  ";
        cout << r << "  ";
        
        bool result = logicFunction(p, q, r);

        cout << result;
        cout << endl;
    }
}

int main() {
    auto logicExpr = [](bool p, bool q, bool r) -> bool {
        // Example logic: (p || !r) && (!p || (q || !r))
        return (p || !r) && (!p || (q || !r));
    };

    generateTruthTable(logicExpr);

    return 0;
}
