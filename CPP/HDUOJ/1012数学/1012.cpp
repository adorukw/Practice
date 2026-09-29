#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    cout << "n e\n- -----------\n";

    double e = 0.0;
    double term = 1.0;

    for (int n = 0; n <= 9; n++) {
        if (n > 0) term /= n;
        e += term;

        if (n <= 1) {
            cout << n << " " << static_cast<int>(e) << endl;
        }
        else if (n == 2) {
            cout << n << " " << fixed << setprecision(1) << e << endl;
        }
        else {
            cout << n << " " << fixed << setprecision(9) << e << endl;
        }
    }

    return 0;
}