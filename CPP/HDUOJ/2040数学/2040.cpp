#include <iostream>

using namespace std;

int DivisorSum(int n) {
    int sum = 0;
    for (int i = 1; i <= n / 2; i++) {
        if (n % i == 0) sum += i;
    }

    return sum;
}

int main() {
    int M;
    while (cin >> M) {
        for (int i = 0; i < M; i++) {
            int A, B;
            cin >> A >> B;
            if (DivisorSum(A) == B && DivisorSum(B) == A)
                cout << "YES" << endl;
            else
                cout << "NO" << endl;
        }
    }

    return 0;
}