#include <cstring>
#include <iostream>

using namespace std;

const int MAX_N = 1000000;
const int CACHE_SIZE = 1000005;

int cycle[CACHE_SIZE];

int Calculate(long long n) {
    int count = 1;

    while (n != 1) {
        if (n < CACHE_SIZE && cycle[n] != 0) return count + cycle[n] - 1;

        if (n % 2 == 0)
            n = n / 2;
        else
            n = 3 * n + 1;
        count++;
    }

    return count;
}
int main() {
    memset(cycle, 0, sizeof(cycle));
    cycle[1] = 1;

    for (int i = 2; i <= MAX_N; i++) {
        if (cycle[i] == 0) cycle[i] = Calculate(i);
    }

    int i, j;
    while (cin >> i >> j) {
        int a = i, b = j;
        if (a > b) swap(a, b);
        int maxCycle = 0;
        for (int k = a; k <= b; k++) {
            if (cycle[k] > maxCycle) maxCycle = cycle[k];
        }

        cout << i << " " << j << " " << maxCycle << '\n';
    }

    return 0;
}