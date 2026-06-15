#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string s;
    while (getline(cin, s)) {
        if (s[0] >= 'a') s[0] = s[0] - 'a' + 'A';
        for (int i = 1; i < s.length(); i++) {
            if (s[i - 1] == ' ' && s[i] >= 'a') s[i] = s[i] - 'a' + 'A';
        }
        cout << s;
    }

    return 0;
}