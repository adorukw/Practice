#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n, m;
    while (cin >> n >> m) {
        if (n == 0 && m == 0) break;
        bool appended = false;

        for (int i = 0; i < n; i++) {
            int num;
            cin >> num;
            if (num >= m && !appended) {
                cout << m << ' ';
                appended = true;
            }
            cout << num << ' ';
        }
        if(!appended) cout << m << '\n';
    }

    return 0;
}