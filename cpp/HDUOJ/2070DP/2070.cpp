#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 55;

vector<long long> fibbonacci(MAXN, 0);

int main() {
    fibbonacci[0] = 0;
    fibbonacci[1] = 1;
    for (int i = 2; i < MAXN; i++) {
        fibbonacci[i] = fibbonacci[i - 1] + fibbonacci[i - 2];
    }
    int n;
    while (cin >> n) {
        if (n == -1) break;
        cout << fibbonacci[n] << endl;
    }
    return 0;
}