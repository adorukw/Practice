#include <iostream>
using namespace std;

int main() {
    int M;
    cin >> M;

    for (int i = 0; i < M; i++) {
        double A, B, C;
        cin >> A >> B >> C;

        if (A + B > C && A + C > B && B + C > A) {
            cout << "YES" << endl;
        }
        else {
            cout << "NO" << endl;
        }
    }

    return 0;
}