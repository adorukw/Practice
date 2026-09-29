#include <iostream>
#include <vector>

using namespace std;

long long Permutation(int a, int b) {
    if (b == 0) return 1;
    long long res = 1;
    for (int i = 0; i < b; i++) {
        res *= (a - i);
    }

    return res;
}

void Dfs(int n, long long m, vector<bool>& used) {
    int remain = 0;
    for (int i = 1; i <= n; i++) {
        if (!used[i]) remain++;
    }

    for (int x = 1; x <= n; x++) {
        if (used[x]) continue;

        long long count = 0;
        for (int k = 0; k < remain; k++) {
            count += Permutation(remain - 1, k);
        }

        if (m > count) {
            m -= count;
            continue;
        }

        cout << x;
        used[x] = true;
        if (m == 1) return;
        cout << ' ';
        Dfs(n, m - 1, used);
        return;
    }
}

int main() {
    int n;
    long long m;
    while (cin >> n >> m) {
        vector<bool> used(n + 1, false);
        Dfs(n, m, used);
        cout << endl;
    }

    return 0;
}