#include <iostream>

using namespace std;

int main() {
    int N;
    while (cin >> N) {
        int res = 0, num;
        for (int i = 0; i < N; i++) {
            cin >> num;
            res += num;
        }
        cout << res << '\n';
    }

    return 0;
}