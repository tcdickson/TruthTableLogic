#include<iostream>
using namespace std;

bool getBit(unsigned int uint, int position) { 
     return (uint >> position) & 0x1;
}

int main(int argc, char* argv[]) {

    bool p, q, r, s, t;
    
    cout << "p  q  r  s  t  (p -> q) <-> [( r ^ t ) v (not) s] \n";
    for (unsigned int i = 0; i < 32; i++) {
        p = getBit(i, 0);
        q = getBit(i, 1);
        r = getBit(i, 2);
        s = getBit(i, 3);
        t = getBit(i, 4);
        
        cout << p << "  ";
        cout << q << "  ";
        cout << r << "  ";
        cout << s << "  ";
        cout << t << "  ";
        
   
        cout << ((((p == 0) || (q == 1)) == (((r ^ t) == 1) || (s == 0))) ? 1 : 0);
        
        cout << endl;
    }
    
    return 0;
}
