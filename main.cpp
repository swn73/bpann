#include <iostream>
#include <stdlib.h>

using namespace std;

int main() {
    int a;
    while (cin >> a) {
        for (int i = 0; i < a; ++i) {
            cout << i << "  ";
        }
        cout << endl;
    }
    return 0;
}