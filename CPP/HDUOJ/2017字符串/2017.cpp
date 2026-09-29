#include <iostream>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        for (int i = 0; i < n; i++) {
            string s;
            cin >> s;
            int count = 0;
            for (char c : s) {
                if (c >= '0' && c <= '9') count++;
            }
            cout << count << '\n';
        }
    }

    return 0;
}