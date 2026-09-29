#include <iostream>
#include <string>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        for (int i = 0; i < n; i++) {
            string s;
            cin >> s;
            bool isPalindrome = true;
            for (int i = 0; i < s.length() / 2; i++) {
                if (s[i] != s[s.length() - 1 - i]) isPalindrome = false;
            }
            if (isPalindrome)
                cout << "yes" << endl;
            else
                cout << "no" << endl;
        }
    }

    return 0;
}