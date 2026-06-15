#include <iomanip>
#include <iostream>

using namespace std;

int Gcd(int a, int b) {
    while (b) {
        int temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

int main() {
    int STEP, MOD;
    while (cin >> STEP >> MOD) {
        cout << setw(10) << "STEP" << setw(10) << "MOD";

        if (Gcd(STEP, MOD) == 1)
            cout << "    Good Choice" << endl;
        else
            cout << "    Bad Choice" << endl;

        cout << '\n';
    }

    return 0;
}