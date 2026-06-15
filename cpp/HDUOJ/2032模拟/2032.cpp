#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    vector<vector<int>> a(31, vector<int>(31, 0));
    for (int i = 1; i < 31; i++) {
        a[i][1] = 1;
        a[i][i] = 1;
    }
    for (int i = 2; i < 31; i++) {
        for (int j = 2; j < i; j++) {
            a[i][j] = a[i - 1][j] + a[i - 1][j - 1];
        }
    }
    while (cin >> n) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= i; j++) {
                cout << a[i][j] << ' ';
            }
            cout << endl;
        }
    }

    return 0;
}