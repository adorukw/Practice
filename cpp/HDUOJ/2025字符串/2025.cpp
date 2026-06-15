#include <iostream>

using namespace std;

int main() {
    string s;
    while (cin >> s) {
        int len = s.length();
        char maxChar = s[0];
        for (int i = 1; i < len; i++) {
            if (maxChar < s[i]) maxChar = s[i];
        }
        for (int i = 0; i < len; i++) {
            cout << s[i];
            if (s[i] == maxChar) cout << "(max)";
        }
        cout << '\n';
    }

    return 0;
}